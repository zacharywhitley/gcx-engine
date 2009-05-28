
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
 *  @brief Implementations of header file whereexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file whereexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "whereexpression.h"

WhereExpression::WhereExpression(CondExpression * _cond, Expression * _exp):
Expression(et_where), cond(_cond), exp(_exp), ifexp(NULL) {
}

WhereExpression::~WhereExpression() {
    delete ifexp;
}

void WhereExpression::scopeCheck(vector < unsigned >&def_vars,
                                 vector < unsigned >&introduced_vars,
                                 vector < unsigned >&violating_vars) {
    cond->scopeCheck(def_vars, introduced_vars, violating_vars);
    exp->scopeCheck(def_vars, introduced_vars, violating_vars);
}

void WhereExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    cond->replaceVarId(old_id, new_id);
    exp->replaceVarId(old_id, new_id);
}

void WhereExpression::mergeSequences() {
    ifexp->mergeSequences();
}

void WhereExpression::extractFSAMap(FSAMap * fsamap, unsigned parent_var) {
    ifexp->extractFSAMap(fsamap, parent_var);
}

void WhereExpression::extractParVarMap(ParVarMap * parvarmap) {
    ifexp->extractParVarMap(parvarmap);
}

void WhereExpression::extractDependencies(vector < DependencySet * >*depset) {
    ifexp->extractDependencies(depset);
}

Expression *WhereExpression::placeSignOffs(vector <
                                           SignOffExpression * >&signoffs) {
    ifexp = (IfExpression *) ifexp->placeSignOffs(signoffs);

    return this;
}

void WhereExpression::rewriteWhereExps() {
    ifexp = new IfExpression(cond, exp, new EmptyExpression());
    setType(et_if);
    ifexp->rewriteWhereExps();
}

void WhereExpression::rewriteEmptyFuncts() {
    ifexp->rewriteEmptyFuncts();
}

void WhereExpression::rewriteVarstepCondExps() {
    ifexp->rewriteVarstepCondExps();
}

void WhereExpression::rewriteAggregateFuncts() {
    ifexp->rewriteAggregateFuncts();
}

void WhereExpression::rewriteVarsteps() {
    ifexp->rewriteVarsteps();
}

void WhereExpression::print(OutputStream & dos) const {
    if (ifexp) {
        dos << (*ifexp);
    } else {
        dos << "where (" << (*cond) << ") return " << NEWLINE;
        dos << incrementIndents() << writeIndents() << (*exp) << NEWLINE;
        dos << decrementIndents();
    }
}

void WhereExpression::init(BufferNode * root) {
    ifexp->init(root);
}

void WhereExpression::eval(OutputStream & eos, Environment * env,
                           unsigned modus) {
    ifexp->eval(eos, env, modus);
}

bool WhereExpression::containsDirectOutput() {
    return ifexp->containsDirectOutput();
}
