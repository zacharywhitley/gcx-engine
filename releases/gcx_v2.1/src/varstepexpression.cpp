
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
 *  @brief Implementations of header file varstepexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file varstepexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "varstepexpression.h"

VarStepExpression::VarStepExpression(unsigned _id,
                                     PathExpression *
                                     _path):VarExpression(et_varstep, _id),
path(_path),
bit(NULL) {
}

VarStepExpression::~VarStepExpression() {
    delete path;
    delete bit;
}

void VarStepExpression::extractDependencies(vector < DependencySet * >*depset) {
    for (unsigned i = 0; i < depset->size(); i++) {
        if ((*depset)[i]->getVar() == id) {
            (*depset)[i]->insertTuple(path, false, true, false);
            break;
        }
    }
}

void VarStepExpression::print(OutputStream & dos) const {
    dos << "$" << VarName::getInstance()->getVarname(id) << (*path);
}

void VarStepExpression::init(BufferNode * root) {
    delete bit;

    bit = new BufferIterator(root, path);
}

void VarStepExpression::eval(OutputStream & eos, Environment * env,
                             unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
          {
              bit->init(env->getNodeBinding(id));
              while (BufferNode * cur = bit->getNext(READ_UP_TO_CLOSE_CONTEXT)) {
                  cur->print(eos);
              }
              bit->clear();
              break;
          }

        case EVAL_SIGNOFF:
            break;

        default:
            throw
                RuntimeException("VarStepExpression: Illegal Evaluation Mode",
                                 eid_runtime_illegalmode);
            break;
    }
}

VarStepExpression *VarStepExpression::clone() {
    return new VarStepExpression(id, path->clone());
}

VarStepExpression *VarStepExpression::cloneWithoutFinalDosNodeAndAttributes() {
    return new VarStepExpression(id,
                                 path->cloneWithoutFinalDosNodeAndAttributes());
}

void VarStepExpression::prepareOperand(OutputStream & eos, Environment * env,
                                       unsigned modus) {
    bit->init(env->getNodeBinding(id));
}

void VarStepExpression::unprepareOperand() {
    bit->reset();
}

Value *VarStepExpression::getNextValue() {
    BufferNode *cur = bit->getNext(READ_UP_TO_CLOSE_CONTEXT);

    if (cur) {
        cur_val.setStrVal(cur->getPCDataRepresentation());
        return &cur_val;
    } else {
        return NULL;
    }
}
