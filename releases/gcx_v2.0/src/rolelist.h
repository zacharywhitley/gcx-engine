/*
 | Author: Michael Schmidt;
 |         Gunnar Jehl (multi-step paths/aggregate functions/optimizations) 
 ===========================================================================
 ===========================================================================
 | Software License Agreement (BSD License)
 | 
 | Copyright (c) 2006-2007, Saarland University Database Group 
 | All rights reserved.
 |
 | Redistribution and use of this software in source and binary forms,
 | with or without modification, are permitted provided that the following
 | conditions are met:
 | 
 | * Redistributions of source code must retain the above
 |   copyright notice, this list of conditions and the
 |   following disclaimer.
 |
 | * Redistributions in binary form must reproduce the above
 |   copyright notice, this list of conditions and the
 |   following disclaimer in the documentation and/or other
 |   materials provided with the distribution.
 |
 | * Neither the name the of Saarland University Database Group nor the names 
 |   of its contributors may be used to endorse or promote products derived 
 |   from this software without specific prior written permission of the
 |   Saarland University Database Group.
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
*/
/*! @file
 *     @brief Header file for rolelist.cpp.
 *     @details Header file specifying constructors, destructor and functions for rolelist.cpp.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#ifndef ROLELIST_H
#define ROLELIST_H

#include <vector>
#include "role.h"

// forward declarations
class PathEnvironment;

/*! @class RoleList
 *     @brief Represents a list of all used roles.
 *     @details Represents a list of all used roles, i.e. of all used Role, e.g. <tt>r<sub>i</sub></tt> or <tt></tt>r<sub>j</sub></tt> (whereas <tt>i != j</tt>),
 *             which takes a PathEnvironment as arguments.
 *  @note Variables are internally represented by numbers!
 *  @note SINGLETON PATTERN!
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class RoleList {

public:
    /*! @fn static void initInstance(PathEnvironment* _penv)
     *     @brief Initializes instance of RoleList.
     *     @details Initializes instance of RoleList, i.e. initializes this object.
     *  @note CALL THIS METHOD BEFORE METHOD getInstance()!
     *  @param[in] _penv Pointer to a PathEnvironment object.
     *     @retval void
     */
    static void initInstance(PathEnvironment* _penv);
    
    /*! @fn static RoleList* getInstance()
     *     @brief Returns instance of RoleList.
     *     @details Returns instance of RoleList, i.e. of this object.
     *  @note DO NOT CALL THIS METHOD BEFORE METHOD initInstance(PathEnvironment* _penv) WAS CALLED!
     *     @retval RoleList* Pointer to a RoleList object.
     */
    static RoleList* getInstance();
    
    /*! @brief Destructor.
     *     @details Destructor.
     */
    virtual ~RoleList();

    /*! @fn Role* getFreshRole(unsigned basing_var, ROLE_TYPE type, PathExpression* from_var)
     *     @brief Returns a fresh unused role for use.
     *     @details Returns a fresh unused role for use.
     *  @param[in] basing_var The underlying (numeric value of a) variable for the PathExpression, which is the third argument of this method.
     *  @param[in] type ROLE_TYPE.
     *  @param[in] from_var Pointer to a PathExpression object.
     *     @retval Role* Pointer to a Role object.
     */
    Role* getFreshRole(unsigned basing_var, ROLE_TYPE type, PathExpression* from_var);
    
    /*! @fn bool removeRole(Role* role)
     *     @brief Removes a single role.
     *     @details Removes a single role in this list, i.e. removes a single role in this object.
     *  @param[in] role Pointer to a Role object (role which should be removed).
     *     @retval bool <tt>true</tt> if Role was successfully removed, <tt>false</tt> otherwise. 
     */
    bool removeRole(Role* role);
    
    // TODO: document this!!!
    void sortRolesForSignOffs();
    
    /*! @fn inline vector<Role*>* getRoles()
     *     @brief Returns all used roles.
     *     @details Returns all used roles, i.e. returns the member variable.
     *     @retval vector<Role*>* Pointer to a vector object containing pointer of Role objects.
     */
    inline vector<Role*>* getRoles() { return &roles; }

    /*! @fn inline unsigned getMaxRoleId()
     *  @brief returns the role with maximum id.
     *  @details
     *  @retval unsigned The role with maximum id.
     */
    unsigned getMaxRoleId();

private:    
    /*! @fn inline void computeMaxRoleId()
     *  @brief Computes the max_role_id member.
     *  @details
     *  @retval void 
     */
    void computeMaxRoleId();

    /*! @var static RoleList* instance
     *  @brief Instance of RoleList.
     *  @details Instance of RoleList to avoid duplicate existence of this object.
     */
    static RoleList* instance;
    
    /*! @brief Constructor.
     *     @details Constructor - creating object for a PathEnvironment.
     *  @param[in] _penv Pointer to a PathEnvironment object.
     */
    RoleList(PathEnvironment* _penv);

    /*! @var vector<Role*> roles
     *     @brief All used roles.
     *     @details All used roles, i.e. all used Role.
     */
    vector<Role*> roles;
    
    /*! @var PathEnvironment* penv
     *     @brief The entered PathEnvironment.
     *     @details The entered PathEnvironment, which is the argument of the constructor.
     */
    PathEnvironment* penv;

    /*! @var unsigned max_role_id
	 *  @brief The maximum role id in the role list.
     */
    unsigned max_role_id;

	/*! @var bool max_role_id_computed
	 *  @brief True if the maximum role id has been comoputed. 
	 */
	bool max_role_id_computed;
};

#endif // ROLELIST_H
