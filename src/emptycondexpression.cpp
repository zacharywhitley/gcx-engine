
/*
 | Author: Michael Schmidt;
 |         Gunnar Jehl 
 |
 | ************************* SOFTWARE LICENSE AGREEMENT ***********************
 | This source code is published under the BSD License.
 |
 | See file 'LICENSE.txt' that comes with this distribution or
 | http://dbis.informatik.uni-freiburg.de/index.php?project=GCX/license.php
 | for the full license agreement.
 |
 | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 | AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 | IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 | ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 | LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 | CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 | SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 | INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 | CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 | ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 | POSSIBILITY OF SUCH DAMAGE.
 | ****************************************************************************
*/

/*! @file
 *  @brief Implementations of header file emptycondexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file emptycondexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "emptycondexpression.h"

EmptyCondExpression::EmptyCondExpression(VarExpression * _var):
CondExpression(ect_empty), var(_var), varstep(NULL), ncond(NULL) {
}

EmptyCondExpression::EmptyCondExpression(VarStepExpression * _varstep):
CondExpression(ect_empty), var(NULL), varstep(_varstep), ncond(NULL) {
}

EmptyCondExpression::~EmptyCondExpression() {
    delete ncond;
}

void EmptyCondExpression::scopeCheck(vector < unsigned >&def_vars,
                                     vector < unsigned >&introduced_vars,
                                     vector < unsigned >&violating_vars) {
    if (var) {
        var->scopeCheck(def_vars, introduced_vars, violating_vars);
    } else {
        varstep->scopeCheck(def_vars, introduced_vars, violating_vars);
    }
}

void EmptyCondExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    if (var) {
        var->replaceVarId(old_id, new_id);
    } else {
        varstep->replaceVarId(old_id, new_id);
    }
}

void EmptyCondExpression::extractFSAMap(FSAMap * fsamap, unsigned parent_var) {
    ncond->extractFSAMap(fsamap, parent_var);
}

void EmptyCondExpression::extractParVarMap(ParVarMap * parvarmap) {
    ncond->extractParVarMap(parvarmap);
}

void EmptyCondExpression::extractDependencies(vector < DependencySet * >*depset) {
    ncond->extractDependencies(depset);
}

Expression *EmptyCondExpression::placeSignOffs(vector <
                                               SignOffExpression * >&signoffs) {
    ncond = (NotCondExpression *) ncond->placeSignOffs(signoffs);

    return this;
}

void EmptyCondExpression::rewriteEmptyFuncts() {
    if (var) {
        ncond = new NotCondExpression(new ExistsCondExpression(var));
    } else {
        ncond = new NotCondExpression(new ExistsCondExpression(varstep));
    }
    setType(ect_not);
}

void EmptyCondExpression::rewriteAggregateFuncts() {
    ncond->rewriteAggregateFuncts();
}

void EmptyCondExpression::print(OutputStream & dos) const {
    if (ncond) {
        dos << (*ncond);
    } else {
        if (var) {
            dos << "fn:empty(" << (*var) << ")";
        } else {
            dos << "fn:empty(" << (*varstep) << ")";
        }
    }
}

void EmptyCondExpression::init(BufferNode * root) {
    ncond->init(root);
}

void EmptyCondExpression::eval(OutputStream & eos, Environment * env,
                               unsigned modus) {
    ncond->eval(eos, env, modus);
}

bool EmptyCondExpression::evalCond(OutputStream & eos, Environment * env,
                                   unsigned modus) {
    return ncond->evalCond(eos, env, modus);
}
