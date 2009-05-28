
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
 *  @brief Implementations of header file sequenceexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file sequenceexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "sequenceexpression.h"

SequenceExpression::SequenceExpression(Expression * _left, Expression * _right):
Expression(et_sequence), delete_recursively(true) {
    exps.push_back(_left);
    exps.push_back(_right);
}

SequenceExpression::~SequenceExpression() {
    for (unsigned i = 0; i < exps.size() && delete_recursively; i++) {
        delete exps[i];
    }
}

void SequenceExpression::scopeCheck(vector < unsigned >&def_vars,
                                    vector < unsigned >&introduced_vars,
                                    vector < unsigned >&violating_vars) {
    for (unsigned i = 0; i < exps.size(); i++) {
        vector < unsigned >def_vars_cpy = def_vars;

        exps[i]->scopeCheck(def_vars_cpy, introduced_vars, violating_vars);
    }
}

void SequenceExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    for (unsigned i = 0; i < exps.size(); i++) {
        exps[i]->replaceVarId(old_id, new_id);
    }
}

void SequenceExpression::mergeSequences() {
    for (unsigned i = 0; i < exps.size(); i++) {
        exps[i]->mergeSequences();
    }

    vector < Expression * >new_exps;
    for (unsigned i = 0; i < exps.size(); i++) {
        if (exps[i]->getType() == et_sequence) {
            SequenceExpression *se_sub = (SequenceExpression *) exps[i];

            vector < Expression * >*sub = se_sub->getExps();
            for (unsigned i = 0; i < sub->size(); i++) {
                new_exps.push_back((*sub)[i]);
            }
            se_sub->unsetRecursiveDelete();
            delete se_sub;
        } else {
            new_exps.push_back(exps[i]);
        }
    }
    exps = new_exps;
}

void SequenceExpression::extractFSAMap(FSAMap * fsamap, unsigned parent_var) {
    for (unsigned i = 0; i < exps.size(); i++) {
        exps[i]->extractFSAMap(fsamap, parent_var);
    }
}

void SequenceExpression::extractParVarMap(ParVarMap * parvarmap) {
    for (unsigned i = 0; i < exps.size(); i++) {
        exps[i]->extractParVarMap(parvarmap);
    }
}

void SequenceExpression::extractDependencies(vector < DependencySet * >*depset) {
    for (unsigned i = 0; i < exps.size(); i++) {
        exps[i]->extractDependencies(depset);
    }
}

Expression *SequenceExpression::placeSignOffs(vector <
                                              SignOffExpression * >&signoffs) {
    for (unsigned i = 0; i < exps.size(); i++) {
        exps[i] = exps[i]->placeSignOffs(signoffs);
    }

    return this;
}

void SequenceExpression::rewriteWhereExps() {
    for (unsigned i = 0; i < exps.size(); i++) {
        exps[i]->rewriteWhereExps();
    }
}

void SequenceExpression::rewriteEmptyFuncts() {
    for (unsigned i = 0; i < exps.size(); i++) {
        exps[i]->rewriteEmptyFuncts();
    }
}

void SequenceExpression::rewriteVarstepCondExps() {
    for (unsigned i = 0; i < exps.size(); i++) {
        exps[i]->rewriteVarstepCondExps();
    }
}

void SequenceExpression::rewriteAggregateFuncts() {
    for (unsigned i = 0; i < exps.size(); i++) {
        exps[i]->rewriteAggregateFuncts();
    }
}

void SequenceExpression::rewriteVarsteps() {
    for (unsigned i = 0; i < exps.size(); i++) {
        if (exps[i]->getType() == et_varstep) {
            VarStepExpression *sub = ((VarStepExpression *) exps[i])->clone();
            delete exps[i];
            VarExpression *tmpvar =
                new VarExpression(VarName::getInstance()->getFreshVarname(),
                                  true);
            ForExpression *forexp =
                new ForExpression(tmpvar, sub, tmpvar->clone());
            exps[i] = forexp;
        } else {
            exps[i]->rewriteVarsteps();
        }
    }
}

void SequenceExpression::print(OutputStream & dos) const {
    bool bprint = false;

    for (unsigned i = 0; i < exps.size(); i++) {
        if (!bprint && exps[i]->getType() != et_comment
            && exps[i]->getType() != et_doc) {
            dos << "(" << NEWLINE << incrementIndents();
            bprint = true;
        }
        dos << writeIndents() << (*exps[i]);
        if (i < exps.size() - 1 && exps[i]->getType() != et_comment
            && exps[i]->getType() != et_doc) {
            dos << ",";
        }
        if (exps[i]->getType() != et_comment && exps[i]->getType() != et_doc) {
            dos << NEWLINE;

        }
    }

    if (bprint) {
        dos << decrementIndents();
    }

    dos << writeIndents();

    if (bprint) {
        dos << ")";
    }
}

void SequenceExpression::init(BufferNode * root) {
    for (unsigned i = 0; i < exps.size(); i++) {
        exps[i]->init(root);
    }
}

void SequenceExpression::eval(OutputStream & eos, Environment * env,
                              unsigned modus) {
    switch (modus) {

        case EVAL_QUERY:
        case EVAL_SIGNOFF:
            for (unsigned i = 0; i < exps.size(); i++) {
                exps[i]->eval(eos, env, modus);
            }
            break;

        default:
            throw
                RuntimeException("SequenceExpression: Illegal Evaluation Mode",
                                 eid_runtime_illegalmode);
            break;
    }
}

bool SequenceExpression::containsDirectOutput() {
    for (unsigned i = 0; i < exps.size(); i++) {
        switch (exps[i]->getType()) {
            case et_nodeconstr:
            case et_stringconst:
                return true;
            case et_numericconst:
                return true;
            case et_sequence:
            case et_if:
                if (exps[i]->containsDirectOutput()) {
                    return true;
                }
            default:
                break;          // check next subexpression
        }
    }

    return false;
}
