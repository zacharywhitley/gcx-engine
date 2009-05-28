
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
 *  @brief Implementations of header file varexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file varexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "varexpression.h"

VarExpression::VarExpression():
OperandExpression(et_var, et_operandvar), id(TAGID_ROOT), bit(NULL) {
}

VarExpression::VarExpression(const char *_name):OperandExpression(et_var,
                                                                  et_operandvar),
id(VarName::getInstance()->insertVarname(_name)),
bit(NULL) {
    free((char *) _name);
}

VarExpression::VarExpression(const char *_name,
                             bool _force):OperandExpression(et_var,
                                                            et_operandvar),
id(VarName::getInstance()->insertVarname(_name, _force)),
bit(NULL) {
    free((char *) _name);
}

VarExpression::VarExpression(EXP_TYPE _type,
                             unsigned _id):OperandExpression(_type,
                                                             et_operandvarstep),
id(_id),
bit(NULL) {
}

VarExpression::~VarExpression() {
    delete bit;
}

void VarExpression::scopeCheck(vector < unsigned >&def_vars,
                               vector < unsigned >&introduced_vars,
                               vector < unsigned >&violating_vars) {
    for (unsigned i = 0; i < def_vars.size(); i++) {
        if (id == def_vars[i]) {
            return;
        }
    }

    violating_vars.push_back(id);
}

void VarExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    if (id == old_id) {
        id = new_id;
    }
}

void VarExpression::extractDependencies(vector < DependencySet * >*depset) {
    for (unsigned i = 0; i < depset->size(); i++) {
        if ((*depset)[i]->getVar() == id) {
            (*depset)[i]->insertTuple(NULL, false, true, false);
            break;
        }
    }
}

void VarExpression::print(OutputStream & dos) const {
    dos << "$" << VarName::getInstance()->getVarname(id);
}

void VarExpression::init(BufferNode * root) {
    delete bit;

    bit = new BufferIterator(root, NULL);
}

void VarExpression::eval(OutputStream & eos, Environment * env, unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
          {
              bit->init(env->getNodeBinding(id));
              BufferNode *cur = bit->getNext(READ_UP_TO_CLOSE_BASE);

              cur->print(eos);
              bit->clear();
              break;
          }

        case EVAL_SIGNOFF:
            break;

        default:
            throw RuntimeException("VarExpression: Illegal Evaluation Mode",
                                   eid_runtime_illegalmode);
            break;
    }
}

VarExpression *VarExpression::clone() {
    VarExpression *varexp = new VarExpression();

    varexp->setId(id);

    return varexp;
}

void VarExpression::prepareOperand(OutputStream & eos, Environment * env,
                                   unsigned modus) {
    bit->init(env->getNodeBinding(id));
}

void VarExpression::unprepareOperand() {
    bit->reset();
}

Value *VarExpression::getNextValue() {
    BufferNode *cur = bit->getNext(READ_UP_TO_CLOSE_CONTEXT);

    if (cur) {
        cur_val.setStrVal(cur->getPCDataRepresentation());
        return &cur_val;
    } else {
        return NULL;
    }
}
