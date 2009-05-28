
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
 *  @brief Implementations of header file condoperandexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file condoperandexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "condoperandexpression.h"
#include "operandexpression.h"

CondOperandExpression::CondOperandExpression(OperandExpression * _left,
                                             COMP_TYPE _op,
                                             OperandExpression *
                                             _right):CondExpression
    (ect_operand),
left(_left),
op(_op),
right(_right) {
}

CondOperandExpression::~CondOperandExpression() {
    delete left;
    delete right;
}

void CondOperandExpression::scopeCheck(vector < unsigned >&def_vars,
                                       vector < unsigned >&introduced_vars,
                                       vector < unsigned >&violating_vars) {
    left->scopeCheck(def_vars, introduced_vars, violating_vars);
    right->scopeCheck(def_vars, introduced_vars, violating_vars);
}

void CondOperandExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    left->replaceVarId(old_id, new_id);
    right->replaceVarId(old_id, new_id);
}

void CondOperandExpression::extractFSAMap(FSAMap * fsamap, unsigned parent_var) {
    left->extractFSAMap(fsamap, parent_var);
    right->extractFSAMap(fsamap, parent_var);
}

void CondOperandExpression::extractParVarMap(ParVarMap * parvarmap) {
    left->extractParVarMap(parvarmap);
    right->extractParVarMap(parvarmap);
}

void CondOperandExpression::extractDependencies(vector <
                                                DependencySet * >*depset) {
    left->extractDependencies(depset);
    right->extractDependencies(depset);
}

Expression *CondOperandExpression::placeSignOffs(vector <
                                                 SignOffExpression *
                                                 >&signoffs) {
    left = (OperandExpression *) left->placeSignOffs(signoffs);
    right = (OperandExpression *) right->placeSignOffs(signoffs);

    return this;
}

void CondOperandExpression::rewriteEmptyFuncts() {
    left->rewriteEmptyFuncts();
    right->rewriteEmptyFuncts();
}

void CondOperandExpression::rewriteVarstepCondExps() {
    left->rewriteVarstepCondExps();
    right->rewriteVarstepCondExps();
}

void CondOperandExpression::rewriteAggregateFuncts() {
    left->rewriteAggregateFuncts();
    right->rewriteAggregateFuncts();
}

void CondOperandExpression::print(OutputStream & dos) const {
    dos << (*left) << " ";
    switch (op) {
        case ct_lt:
            dos << "<";
            break;
        case ct_leq:
            dos << "<=";
            break;
        case ct_eq:
            dos << "=";
            break;
        case ct_gt:
            dos << ">";
            break;
        case ct_geq:
            dos << ">=";
            break;
        case ct_neq:
            dos << "!=";
            break;
    }
    dos << " " << (*right);
}

void CondOperandExpression::init(BufferNode * root) {
    left->init(root);
    right->init(root);
}

bool CondOperandExpression::evalCond(OutputStream & eos, Environment * env,
                                     unsigned modus) {


    switch (modus) {

        case EVAL_QUERY:
          {
              // optimization: single-valued operands always in outer loop
              OperandExpression *lop = NULL;
              OperandExpression *rop = NULL;
              COMP_TYPE ct;

              if (right->isSingleValued()) {
                  lop = right;
                  rop = left;
                  ct = invertOperator(op);
              } else {
                  lop = left;
                  rop = right;
                  ct = op;
              }

              // initialize structures for iteration, where necessary 
              lop->prepareOperand(eos, env, modus);
              rop->prepareOperand(eos, env, modus);

              Value *lval = lop->getNextValue();

              while (lval != NULL) {
                  Value *rval = rop->getNextValue();

                  while (rval != NULL) {
                      if (evalRelOp(lval, ct, rval)) {
                          lop->unprepareOperand();
                          rop->unprepareOperand();
                          return true;
                      }
                      rval = rop->getNextValue();
                  }
                  rop->unprepareOperand();
                  lval = lop->getNextValue();
              }

              lop->unprepareOperand();
              rop->unprepareOperand();

              break;
          }

        case EVAL_SIGNOFF:
            break;

        default:
            throw
                RuntimeException
                ("CondOperandExpression: Illegal Evaluation Mode",
                 eid_runtime_illegalmode);
            break;
    }

    return false;               // never reached
}

bool CondOperandExpression::evalRelOp(Value * left, COMP_TYPE comp,
                                      Value * right) {

    if (left->type == xsd_numeric || right->type == xsd_numeric) {
        switch (comp) {
            case ct_lt:
                return left->getNumVal() < right->getNumVal();
            case ct_leq:
                return left->getNumVal() <= right->getNumVal();
            case ct_eq:
                return left->getNumVal() == right->getNumVal();
            case ct_gt:
                return left->getNumVal() > right->getNumVal();
            case ct_geq:
                return left->getNumVal() >= right->getNumVal();
            case ct_neq:
                return left->getNumVal() != right->getNumVal();
            default:           // should never happen
                return false;
        }
    } else {
        switch (comp) {
            case ct_lt:
                return strcmp(left->getStrVal(), right->getStrVal()) < 0;
            case ct_leq:
                return strcmp(left->getStrVal(), right->getStrVal()) <= 0;
            case ct_eq:
                return strcmp(left->getStrVal(), right->getStrVal()) == 0;
            case ct_gt:
                return strcmp(left->getStrVal(), right->getStrVal()) > 0;
            case ct_geq:
                return strcmp(left->getStrVal(), right->getStrVal()) >= 0;
            case ct_neq:
                return strcmp(left->getStrVal(), right->getStrVal()) != 0;
            default:           // should never happen
                return false;
        }
    }
}

COMP_TYPE CondOperandExpression::invertOperator(COMP_TYPE ot) {
    switch (ot) {
        case ct_lt:
            return ct_gt;
        case ct_leq:
            return ct_geq;
        case ct_gt:
            return ct_lt;
        case ct_geq:
            return ct_leq;
        case ct_eq:
        case ct_neq:
        default:
            return ot;
    }
}
