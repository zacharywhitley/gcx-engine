
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
 *  @brief Implementations of header file role.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file role.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "role.h"
#include "pathexpression.h"
#include "signoffexpression.h"

Role::Role(unsigned _id, ROLE_TYPE _type, unsigned _basing_var,
           unsigned _basing_fsa, PathExpression * _rel_path,
           PathExpression * _var_path):id(_id),
type(_type),
basing_var(_basing_var),
basing_fsa(_basing_fsa),
rel_path(_rel_path),
var_path(_var_path) {

    var_path = var_path ? var_path->clone() : NULL;

    concat_path = rel_path ? rel_path->clone() : new PathExpression();
    if (var_path) {
        vector < PathStepExpression * >*ps = var_path->getPathSteps();
        for (unsigned i = 0; i < ps->size(); i++) {
            concat_path->addPathStep((*ps)[i]->clone());
        }
    }
}

Role::~Role() {
    delete rel_path;
    delete var_path;
    delete concat_path;
}

void Role::print(OutputStream & dos) const {
    dos << "r" << id;
}

void Role::printExtended(OutputStream & dos) {
    dos << "role=r" << id << ", base_var=$"
        << VarName::getInstance()->getVarname(basing_var) << ", base_fsa=$"
        << VarName::getInstance()->getVarname(basing_fsa);
    (rel_path == NULL || rel_path->isEmptyPath())?
        dos << ", rel_path=%" : dos << ", rel_path=" << *(rel_path);
    (var_path == NULL || var_path->isEmptyPath())?
        dos << ", var_path=%" : dos << ", var_path=" << *(var_path);
}

SignOffExpression *Role::getSignOffExpression() {

    if (concat_path && !concat_path->isEmptyPath()) {
        return new SignOffExpression(new
                                     VarStepExpression(basing_fsa,
                                                       concat_path->clone()),
                                     this);
    } else {
        VarExpression *var = new VarExpression();

        var->setId(basing_fsa);
        return new SignOffExpression((VarExpression *) var, this);
    }
}

bool Role::isPosRole() {
    return var_path && var_path->getTailPathStep()->hasAttribute()
        && var_path->getTailPathStep()->getAttribute()->getType() ==
        at_position;
}

bool Role::isDosRole() {
    return var_path && !var_path->isEmptyPath()
        && var_path->getTailPathStep()->isDosNodeStep();
}
