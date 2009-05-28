
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
 *  @brief Implementations of header file varstepcondexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file varstepcondexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "varstepcondexpression.h"

VarstepCondExpression::VarstepCondExpression(VarExpression * _var):
CondExpression(ect_varstep), var(_var), varstep(NULL), econd(NULL) {
}

VarstepCondExpression::VarstepCondExpression(VarStepExpression * _varstep):
CondExpression(ect_varstep), var(NULL), varstep(_varstep), econd(NULL) {
}

VarstepCondExpression::~VarstepCondExpression() {
    delete econd;
}

void VarstepCondExpression::scopeCheck(vector < unsigned >&def_vars,
                                       vector < unsigned >&introduced_vars,
                                       vector < unsigned >&violating_vars) {
    if (var) {
        var->scopeCheck(def_vars, introduced_vars, violating_vars);
    } else {
        varstep->scopeCheck(def_vars, introduced_vars, violating_vars);
    }
}

void VarstepCondExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    if (var) {
        var->replaceVarId(old_id, new_id);
    } else {
        varstep->replaceVarId(old_id, new_id);
    }
}

void VarstepCondExpression::extractDependencies(vector <
                                                DependencySet * >*depset) {
    econd->extractDependencies(depset);
}

void VarstepCondExpression::rewriteVarstepCondExps() {
    econd =
        var ? new ExistsCondExpression(var) : new ExistsCondExpression(varstep);
    setType(ect_exists);
}

void VarstepCondExpression::print(OutputStream & dos) const {
    if (econd) {
        dos << (*econd);
    } else if (var) {
        dos << (*var);
    } else {
        dos << (*varstep);
    }
}

void VarstepCondExpression::init(BufferNode * root) {
    econd->init(root);
}

bool VarstepCondExpression::evalCond(OutputStream & eos, Environment * env,
                                     unsigned modus) {
    return econd->evalCond(eos, env, modus);
}
