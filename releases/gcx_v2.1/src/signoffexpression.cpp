
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
 *  @brief Implementations of header file signoffexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file signoffexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "signoffexpression.h"

SignOffExpression::SignOffExpression(VarExpression * _var, Role * _role):
Expression(et_signoff), var(_var), varstep(NULL), exec_var(var->clone()),
exec_varstep(NULL), role(_role), bit(NULL) {
}

SignOffExpression::SignOffExpression(VarStepExpression * _varstep, Role * _role):
Expression(et_signoff), var(NULL), varstep(_varstep), exec_var(NULL),
exec_varstep(varstep->cloneWithoutFinalDosNodeAndAttributes()), role(_role),
bit(NULL) {
    // avoid null paths for varsteps and create varexp instead
    if (exec_varstep->getPath()->isEmptyPath()) {
        exec_var = new VarExpression();
        exec_var->setId(exec_varstep->getId());
        delete exec_varstep;

        exec_varstep = NULL;
    }
}

SignOffExpression::~SignOffExpression() {
    delete var;
    delete varstep;
    delete exec_var;
    delete exec_varstep;

    // we do not delete member Role* role: will be done by RoleList
    delete bit;
}

void SignOffExpression::scopeCheck(vector < unsigned >&def_vars,
                                   vector < unsigned >&introduced_vars,
                                   vector < unsigned >&violating_vars) {
    if (var) {
        var->scopeCheck(def_vars, introduced_vars, violating_vars);
    } else {
        varstep->scopeCheck(def_vars, introduced_vars, violating_vars);
    }
}

void SignOffExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    if (var) {
        var->replaceVarId(old_id, new_id);
        exec_var->replaceVarId(old_id, new_id);
    } else {
        varstep->replaceVarId(old_id, new_id);
        if (exec_var) {
            exec_var->replaceVarId(old_id, new_id);
        } else {
            exec_varstep->replaceVarId(old_id, new_id);
        }
    }
}

void SignOffExpression::print(OutputStream & dos) const {
    if (var) {
        dos << "signOff(" << (*var) << "," << (*role) << ")";
        dos << " [" << (*exec_var) << "]";
    } else {
        dos << "signOff(" << (*varstep) << "," << (*role) << ")";
        if (exec_var) {
            dos << " [" << (*exec_var) << "]";
        } else {
            dos << " [" << (*exec_varstep) << "]";
        }
    }
}

void SignOffExpression::init(BufferNode * root) {
    delete bit;

    bit =
        exec_var ? new BufferIterator(root, NULL) : new BufferIterator(root,
                                                                       exec_var
                                                                       ? NULL
                                                                       :
                                                                       exec_varstep->
                                                                       getPath
                                                                       ());
}

void SignOffExpression::eval(OutputStream & eos, Environment * env,
                             unsigned modus) {

    // we bind to exec_varstep here; this means we do not consider the
    // final /dos::node() step and atribute [1] when issuing signOffs.
    // This implements the idea of aggregated (also called cumulative)
    // roles: only the top level node gets assigned the role, so the
    // signoff is only send to this one.
    exec_var ? bit->init(env->getNodeBinding(exec_var->getId())) :
        bit->init(env->getNodeBinding(exec_varstep->getId()));

    while (BufferNode * cur =
           bit->getNext(READ_UP_TO_CLOSE_NONE, LOCK_CONTEXT_ALWAYS_CLEAR)) {
        cur->removeRoleFromNode(role);
    }
    bit->clear();
}

unsigned SignOffExpression::getVar() {
    if (var) {
        return var->getId();
    } else {
        return varstep->getId();
    }
}
