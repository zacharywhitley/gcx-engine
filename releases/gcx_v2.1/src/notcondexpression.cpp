
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
 *  @brief Implementations of header file notcondexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file notcondexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "notcondexpression.h"

NotCondExpression::NotCondExpression(CondExpression * _exp):
CondExpression(ect_not), exp(_exp) {
}

NotCondExpression::~NotCondExpression() {
    delete exp;
}

void NotCondExpression::scopeCheck(vector < unsigned >&def_vars,
                                   vector < unsigned >&introduced_vars,
                                   vector < unsigned >&violating_vars) {
    exp->scopeCheck(def_vars, introduced_vars, violating_vars);
}

void NotCondExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    exp->replaceVarId(old_id, new_id);
}

void NotCondExpression::extractFSAMap(FSAMap * fsamap, unsigned parent_var) {
    exp->extractFSAMap(fsamap, parent_var);
}

void NotCondExpression::extractParVarMap(ParVarMap * parvarmap) {
    exp->extractParVarMap(parvarmap);
}

void NotCondExpression::extractDependencies(vector < DependencySet * >*depset) {
    exp->extractDependencies(depset);
}

Expression *NotCondExpression::placeSignOffs(vector <
                                             SignOffExpression * >&signoffs) {
    exp = (CondExpression *) exp->placeSignOffs(signoffs);

    return this;
}

void NotCondExpression::rewriteEmptyFuncts() {
    exp->rewriteEmptyFuncts();
}

void NotCondExpression::rewriteVarstepCondExps() {
    exp->rewriteVarstepCondExps();
}

void NotCondExpression::rewriteAggregateFuncts() {
    exp->rewriteAggregateFuncts();
}

void NotCondExpression::print(OutputStream & dos) const {
    dos << "fn:not(" << (*exp) << ")";
}

void NotCondExpression::init(BufferNode * root) {
    exp->init(root);
}

void NotCondExpression::eval(OutputStream & eos, Environment * env,
                             unsigned modus) {
    // the method is used to forward signOff execution requests
    switch (modus) {

        case EVAL_SIGNOFF:
            exp->eval(eos, env, EVAL_SIGNOFF);
            break;

        default:
            throw
                RuntimeException("NotCondExpression: Illegal Evaluation Mode",
                                 eid_runtime_illegalmode);
            break;
    }
}

bool NotCondExpression::evalCond(OutputStream & eos, Environment * env,
                                 unsigned modus) {
    return !exp->evalCond(eos, env, modus);
}
