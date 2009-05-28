
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
 *  @brief Implementations of header file ifexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file ifexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "ifexpression.h"

IfExpression::IfExpression(CondExpression * _cond, Expression * _thenexp, Expression * _elseexp):
Expression(et_if), cond(_cond), thenexp(_thenexp), elseexp(_elseexp) {
}

IfExpression::~IfExpression() {
    delete cond;
    delete thenexp;
    delete elseexp;
}

void IfExpression::scopeCheck(vector < unsigned >&def_vars,
                              vector < unsigned >&introduced_vars,
                              vector < unsigned >&violating_vars) {
    cond->scopeCheck(def_vars, introduced_vars, violating_vars);
    thenexp->scopeCheck(def_vars, introduced_vars, violating_vars);
    elseexp->scopeCheck(def_vars, introduced_vars, violating_vars);
}

void IfExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    cond->replaceVarId(old_id, new_id);
    thenexp->replaceVarId(old_id, new_id);
    elseexp->replaceVarId(old_id, new_id);
}

void IfExpression::mergeSequences() {
    cond->mergeSequences();
    thenexp->mergeSequences();
    elseexp->mergeSequences();
}

void IfExpression::extractFSAMap(FSAMap * fsamap, unsigned parent_var) {
    cond->extractFSAMap(fsamap, parent_var);
    thenexp->extractFSAMap(fsamap, parent_var);
    elseexp->extractFSAMap(fsamap, parent_var);
}

void IfExpression::extractParVarMap(ParVarMap * parvarmap) {
    cond->extractParVarMap(parvarmap);
    thenexp->extractParVarMap(parvarmap);
    elseexp->extractParVarMap(parvarmap);
}

void IfExpression::extractDependencies(vector < DependencySet * >*depset) {
    cond->extractDependencies(depset);
    thenexp->extractDependencies(depset);
    elseexp->extractDependencies(depset);
}

Expression *IfExpression::placeSignOffs(vector < SignOffExpression * >&signoffs) {
    cond = (CondExpression *) cond->placeSignOffs(signoffs);
    thenexp = thenexp->placeSignOffs(signoffs);
    elseexp = elseexp->placeSignOffs(signoffs);

    return this;
}

void IfExpression::rewriteWhereExps() {
    cond->rewriteWhereExps();
    thenexp->rewriteWhereExps();
    elseexp->rewriteWhereExps();
}

void IfExpression::rewriteEmptyFuncts() {
    cond->rewriteEmptyFuncts();
    thenexp->rewriteEmptyFuncts();
    elseexp->rewriteEmptyFuncts();
}

void IfExpression::rewriteVarstepCondExps() {
    cond->rewriteVarstepCondExps();
    thenexp->rewriteVarstepCondExps();
    elseexp->rewriteVarstepCondExps();
}

void IfExpression::rewriteAggregateFuncts() {
    cond->rewriteAggregateFuncts();
    thenexp->rewriteAggregateFuncts();
    elseexp->rewriteAggregateFuncts();
}

void IfExpression::rewriteVarsteps() {
    if (thenexp->getType() == et_varstep) {
        VarStepExpression *sub = ((VarStepExpression *) thenexp)->clone();
        delete thenexp;
        VarExpression *tmpvar =
            new VarExpression(VarName::getInstance()->getFreshVarname(),
                              true);
        ForExpression *forexp = new ForExpression(tmpvar, sub, tmpvar->clone());

        thenexp = forexp;
    } else {
        thenexp->rewriteVarsteps();
    }

    if (elseexp->getType() == et_varstep) {
        VarStepExpression *sub = ((VarStepExpression *) elseexp)->clone();
        delete elseexp;
        VarExpression *tmpvar =
            new VarExpression(VarName::getInstance()->getFreshVarname(),
                              true);
        ForExpression *forexp = new ForExpression(tmpvar, sub, tmpvar->clone());

        elseexp = forexp;
    } else {
        elseexp->rewriteVarsteps();
    }
}

void IfExpression::print(OutputStream & dos) const {
    dos << "if (" << (*cond) << ")" << NEWLINE << incrementIndents() <<
        writeIndents();

    dos << "then " << NEWLINE << incrementIndents() << writeIndents();

    dos << (*thenexp) << NEWLINE << decrementIndents() << writeIndents();

    dos << "else " << NEWLINE << incrementIndents() << writeIndents();

    dos << (*elseexp) << decrementIndents() << decrementIndents();
}

void IfExpression::init(BufferNode * root) {
    cond->init(root);
    thenexp->init(root);
    elseexp->init(root);
}

void IfExpression::eval(OutputStream & eos, Environment * env, unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
            // we evaluate the correct one and distribute signoff
            // statements for the other one; this simulates
            // if-rewriting as described in the GCX ICDE'07 paper
            if (cond->evalCond(eos, env, modus)) {
                thenexp->eval(eos, env, EVAL_QUERY);
                elseexp->eval(eos, env, EVAL_SIGNOFF);
            } else {
                elseexp->eval(eos, env, EVAL_QUERY);
                thenexp->eval(eos, env, EVAL_SIGNOFF);
            }
            break;

        case EVAL_SIGNOFF:
            cond->eval(eos, env, EVAL_SIGNOFF);
            thenexp->eval(eos, env, EVAL_SIGNOFF);
            elseexp->eval(eos, env, EVAL_SIGNOFF);
            break;

        default:
            throw RuntimeException("IfExpression: Illegal Evaluation Mode",
                                   eid_runtime_illegalmode);
            break;
    }
}

bool IfExpression::containsDirectOutput() {

    // check for direct output in thenexp
    switch (thenexp->getType()) {
        case et_nodeconstr:
        case et_stringconst:
            return true;
        case et_numericconst:
            return true;
        case et_sequence:
        case et_if:
            if (thenexp->containsDirectOutput()) {
                return true;
            }
        default:
            break;
    }

    // check for direct output in elsexp
    switch (elseexp->getType()) {
        case et_nodeconstr:
        case et_stringconst:
            return true;
        case et_numericconst:
            return true;
        case et_sequence:
        case et_if:
            if (elseexp->containsDirectOutput()) {
                return true;
            }
        default:
            break;
    }

    // no direct output present
    return false;
}
