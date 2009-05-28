
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
 *  @brief Implementations of header file rolelist.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file rolelist.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "rolelist.h"
#include "pathenvironment.h"

RoleList *RoleList::instance = NULL;

void RoleList::initInstance(PathEnvironment * _penv) {
    instance = new RoleList(_penv);
}

RoleList *RoleList::getInstance() {
    return instance;
}

RoleList::RoleList(PathEnvironment * _penv):
penv(_penv), max_role_id(0), max_role_id_computed(false) {
}

RoleList::~RoleList() {
    for (unsigned i = 0; i < roles.size(); i++) {
        delete roles[i];
    }
}

Role *RoleList::getFreshRole(unsigned basing_var, ROLE_TYPE type,
                             PathExpression * from_var) {
    unsigned basing_fsa = FSAMap::getInstance()->getFSA(basing_var);
    PathExpression *path_between = penv->getPathBetween(basing_var, basing_fsa);

    Role *role =
        new Role(roles.size(), type, basing_var, basing_fsa, path_between,
                 from_var);

    roles.push_back(role);

    return role;
}

unsigned RoleList::getMaxRoleId() {
    if (!max_role_id_computed) {
        computeMaxRoleId();
    }

    return max_role_id;
}

bool RoleList::removeRole(Role * role) {
    if (role) {
        for (unsigned i = 0; i < roles.size(); i++) {
            if (roles[i] == role) {
                delete roles[i];

                roles.erase(roles.begin() + i);
                return true;
            }
        }
    }

    return false;
}

void RoleList::sortRolesForSignOffs() {
    if (roles.size() > 0) {
        // DO NOT merge this two bubblesorts into one (bubble-)sort because
        // sorting by fsa-variables will first push roles/fsa-variables in
        // correct order of variables definitions of for-loops!
        unsigned sort_count = 0;
        Role *role_tmp;

        // sort roles by fsa-variables using bubblesort
        for (unsigned i = 0; i < roles.size(); i++) {
            for (unsigned j = 0; j < (roles.size() - (i + 1)); j++) {
                // this comparison can be done because of the fact that
                // string values of variables are mapped to numeric values
                if (roles[j]->getBasingFSA() < roles[j + 1]->getBasingFSA()) {
                    role_tmp = roles[j];
                    roles[j] = roles[j + 1];
                    roles[j + 1] = role_tmp;
                    sort_count++;
                }
            }
            if (sort_count == 0) {
                break;
            } else {
                sort_count = 0;
            }
        }

        sort_count = 0;
        // sort roles by path weights using bubblesort
        for (unsigned i = 0; i < roles.size(); i++) {
            for (unsigned j = 0; j < (roles.size() - (i + 1)); j++) {
                // it is only allowed to swap roles if the fsa-variables are equal
                if ((roles[j]->getBasingFSA() == roles[j + 1]->getBasingFSA())
                    && (roles[j]->getConcatenationPath()->getWeight() <
                        roles[j + 1]->getConcatenationPath()->getWeight())) {
                    role_tmp = roles[j];
                    roles[j] = roles[j + 1];
                    roles[j + 1] = role_tmp;
                    sort_count++;
                }
            }
            if (sort_count == 0) {
                break;
            } else {
                sort_count = 0;
            }
        }
    }
}

void RoleList::computeMaxRoleId() {
    for (unsigned i = 0; i < roles.size(); i++) {
        if (roles[i]->getId() > max_role_id)
            max_role_id = roles[i]->getId();
    }

    max_role_id_computed = true;
}
