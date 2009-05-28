
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
 *  @brief Implementations of header file andcondexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file andcondexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "andcondexpression.h"

AndCondExpression::AndCondExpression(CondExpression * _left, CondExpression * _right):
CondExpression(ect_and), left(_left), right(_right) {
}

AndCondExpression::~AndCondExpression() {
    delete left;
    delete right;
}

void AndCondExpression::scopeCheck(vector < unsigned >&def_vars,
                                   vector < unsigned >&introduced_vars,
                                   vector < unsigned >&violating_vars) {
    left->scopeCheck(def_vars, introduced_vars, violating_vars);
    right->scopeCheck(def_vars, introduced_vars, violating_vars);
}

void AndCondExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    left->replaceVarId(old_id, new_id);
    right->replaceVarId(old_id, new_id);
}

void AndCondExpression::extractFSAMap(FSAMap * fsamap, unsigned parent_var) {
    left->extractFSAMap(fsamap, parent_var);
    right->extractFSAMap(fsamap, parent_var);
}

void AndCondExpression::extractParVarMap(ParVarMap * parvarmap) {
    left->extractParVarMap(parvarmap);
    right->extractParVarMap(parvarmap);
}

void AndCondExpression::extractDependencies(vector < DependencySet * >*depset) {
    left->extractDependencies(depset);
    right->extractDependencies(depset);
}

Expression *AndCondExpression::placeSignOffs(vector <
                                             SignOffExpression * >&signoffs) {
    left = (CondExpression *) left->placeSignOffs(signoffs);
    right = (CondExpression *) right->placeSignOffs(signoffs);

    return this;
}

void AndCondExpression::rewriteEmptyFuncts() {
    left->rewriteEmptyFuncts();
    right->rewriteEmptyFuncts();
}

void AndCondExpression::rewriteVarstepCondExps() {
    left->rewriteVarstepCondExps();
    right->rewriteVarstepCondExps();
}

void AndCondExpression::rewriteAggregateFuncts() {
    left->rewriteAggregateFuncts();
    right->rewriteAggregateFuncts();
}

void AndCondExpression::print(OutputStream & dos) const {
    EXP_TYPE l = left->getType();

    if (l == ect_and || l == ect_or || l == ect_not ||
        l == ect_exists || l == ect_operand || l == ect_true
        || l == ect_false) {
        dos << "(" << (*left) << ")";
    } else {
        dos << (*left);
    }

    dos << " and ";

    EXP_TYPE r = right->getType();

    if (r == ect_and || r == ect_or || r == ect_not ||
        r == ect_exists || r == ect_operand || r == ect_true
        || r == ect_false) {
        dos << "(" << (*right) << ")";
    } else {
        dos << (*right);
    }
}

void AndCondExpression::init(BufferNode * root) {
    left->init(root);
    right->init(root);
}

void AndCondExpression::eval(OutputStream & eos, Environment * env,
                             unsigned modus) {

    switch (modus) {
            // the method is used to forward signOff execution requests
        case EVAL_SIGNOFF:
            left->eval(eos, env, EVAL_SIGNOFF);
            right->eval(eos, env, EVAL_SIGNOFF);
            break;

        default:
            throw
                RuntimeException("AndCondExpression: Illegal Evaluation Mode",
                                 eid_runtime_illegalmode);
            break;
    }
}

bool AndCondExpression::evalCond(OutputStream & eos, Environment * env,
                                 unsigned modus) {
    bool left_val = left->evalCond(eos, env, modus);

    if (left_val) {
        return right->evalCond(eos, env, modus);
    } else {
        // make sure we evaluate the update in this part
        right->evalCond(eos, env, EVAL_SIGNOFF);
        return false;
    }
}
