
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
 *  @brief Implementations of header file existscondexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file existscondexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "existscondexpression.h"

ExistsCondExpression::ExistsCondExpression(VarExpression * _var):
CondExpression(ect_exists), var(_var), varstep(NULL), bit(NULL) {
}

ExistsCondExpression::ExistsCondExpression(VarStepExpression * _varstep):
CondExpression(ect_exists), var(NULL), varstep(_varstep), bit(NULL) {
}

ExistsCondExpression::~ExistsCondExpression() {
    delete var;
    delete varstep;
    delete bit;
}

void ExistsCondExpression::scopeCheck(vector < unsigned >&def_vars,
                                      vector < unsigned >&introduced_vars,
                                      vector < unsigned >&violating_vars) {
    if (var) {
        var->scopeCheck(def_vars, introduced_vars, violating_vars);
    } else {
        varstep->scopeCheck(def_vars, introduced_vars, violating_vars);
    }
}

void ExistsCondExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    if (var) {
        var->replaceVarId(old_id, new_id);
    } else {
        varstep->replaceVarId(old_id, new_id);
    }
}

void ExistsCondExpression::extractDependencies(vector <
                                               DependencySet * >*depset) {
    if (varstep && varstep->getPath() && !varstep->getPath()->isEmptyPath()) {
        PathExpression *p = varstep->getPath();
        unsigned var = varstep->getId();

        for (unsigned i = 0; i < depset->size(); i++) {
            if ((*depset)[i]->getVar() == var) {
                (*depset)[i]->insertTuple(p, true, false, true);
                break;
            }
        }
    }
}

void ExistsCondExpression::print(OutputStream & dos) const {
    if (var) {
        dos << "fn:exists(" << (*var) << ")";
    } else {
        dos << "fn:exists(" << (*varstep) << ")";
    }
}

void ExistsCondExpression::init(BufferNode * root) {
    delete bit;

    bit =
        var ? new BufferIterator(root, NULL) : new BufferIterator(root,
                                                                  varstep->
                                                                  getPath());
}

bool ExistsCondExpression::evalCond(OutputStream & eos, Environment * env,
                                    unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
          {
              var ? bit->init(env->getNodeBinding(var->getId())) :
                  bit->init(env->getNodeBinding(varstep->getId()));

              BufferNode *cur = bit->getNext();

              bit->reset();

              return cur ? true : false;
          }

        case EVAL_SIGNOFF:
            break;

        default:
            throw
                RuntimeException
                ("ExistsCondExpression: Illegal Evaluation Mode",
                 eid_runtime_illegalmode);
            break;
    }

    return false;               // never reached
}
