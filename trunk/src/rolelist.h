
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
 *  @brief Header file for rolelist.cpp.
 *  @details Header file specifying constructors, destructor and functions for rolelist.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef ROLELIST_H
#define ROLELIST_H

#include <vector>
#include "role.h"

// forward declarations
class PathEnvironment;

/*! @class RoleList
 *  @brief Represents a list of all used roles.
 *  @details Represents a list of all used roles, i.e. of all used Role,
 *    e.g. <tt>r<sub>i</sub></tt> or <tt>r<sub>j</sub></tt> (whereas <tt>i != j</tt>),
 *    which takes a PathEnvironment as arguments.
 *  @note SINGLETON PATTERN!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class RoleList {

  public:
    /*! @fn static void initInstance(PathEnvironment* _penv)
     *  @brief Initializes instance of RoleList.
     *  @details Initializes instance of RoleList, i.e. initializes this object.
     *  @note CALL THIS METHOD BEFORE METHOD getInstance()!
     *  @param[in] _penv Pointer to a PathEnvironment object.
     *  @retval void
     */
    static void initInstance(PathEnvironment * _penv);

    /*! @fn static RoleList* getInstance()
     *  @brief Returns instance of RoleList.
     *  @details Returns instance of RoleList, i.e. of this object.
     *  @note DO NOT CALL THIS METHOD BEFORE METHOD initInstance(PathEnvironment* _penv) WAS CALLED!
     *  @retval RoleList* Pointer to a RoleList object.
     */
    static RoleList *getInstance();

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ RoleList();

    /*! @fn inline vector<Role*>* getRoles()
     *  @brief Returns all used roles.
     *  @details Returns all used roles, i.e. returns the member variable.
     *  @retval vector<Role*>* Pointer to a vector object containing pointer of Role objects.
     */
    inline vector < Role * >*getRoles() {
        return &roles;
    }
    /*! @fn Role *getFreshRole(unsigned basing_var, ROLE_TYPE type, PathExpression* from_var)
     *  @brief Returns a fresh unused role for use.
     *  @details Returns a fresh unused role for use.
     *  @param[in] basing_var The underlying (numeric value of a) variable for the PathExpression, which is the third argument of this method.
     *  @param[in] type ROLE_TYPE.
     *  @param[in] from_var Pointer to a PathExpression object.
     *  @retval Role* Pointer to a Role object.
     */ Role *getFreshRole(unsigned basing_var, ROLE_TYPE type,
                           PathExpression * from_var);

    /*! @fn unsigned getMaxRoleId()
     *  @brief Returns the maximum id of all used roles.
     *  @details Returns the maximum id of all used roles.
     *  @retval unsigned Maximum id of all used roles.
     */
    unsigned getMaxRoleId();

    /*! @fn bool removeRole(Role* role)
     *  @brief Removes a single role.
     *  @details Removes a single role in this list, i.e. removes a single role in this object.
     *  @param[in] role Pointer to a Role object (role which should be removed).
     *  @retval bool <tt>true</tt> if Role was successfully removed, <tt>false</tt> otherwise. 
     */
    bool removeRole(Role * role);

    /*! @fn void sortRolesForSignOffs()
     *  @brief Sorts roles, which belong together, for better/faster SignOffExpression execution.
     *  @details Sorts roles, which belong together, for better/faster SignOffExpression execution.
     *  @retval void 
     */
    void sortRolesForSignOffs();

  private:
    /*! @fn void computeMaxRoleId()
     *  @brief Computes the maximum id of all used roles (max_role_id).
     *  @details Computes the maximum id of all used roles (max_role_id).
     *  @retval void 
     */
    void computeMaxRoleId();

    /*! @var static RoleList* instance
     *  @brief Instance of RoleList.
     *  @details Instance of RoleList to avoid duplicate existence of this object.
     */
    static RoleList *instance;

    /*! @brief Constructor.
     *  @details Constructor - creating object for a PathEnvironment.
     *  @param[in] _penv Pointer to a PathEnvironment object.
     */
    RoleList(PathEnvironment * _penv);

    /*! @var vector<Role*> roles
     *  @brief All used roles.
     *  @details All used roles, i.e. all used Role.
     */
    vector < Role * >roles;

    /*! @var PathEnvironment* penv
     *  @brief The entered PathEnvironment.
     *  @details The entered PathEnvironment, which is the argument of the constructor.
     */
    PathEnvironment *penv;

    /*! @var unsigned max_role_id
     *  @brief The maximum id of all used roles.
     *  @details The maximum id of all used roles.
     */
    unsigned max_role_id;

    /*! @var bool max_role_id_computed
     *  @brief Indicates if the maximum id of all used roles has been computed.
     *  @details Indicates if the maximum id of all used roles has been computed,
     *                  i.e. <tt>true</tt> if it has been computed, <tt>false</tt> otherwise.
     */
    bool max_role_id_computed;
};

#endif // ROLELIST_H
