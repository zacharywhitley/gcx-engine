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
 *     @brief Header file for projectiondfastate.cpp.
 *     @details Header file specifying constructors, destructor and functions for projectiondfastate.cpp.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#ifndef PROJECTIONDFASTATE_H
#define PROJECTIONDFASTATE_H

#include <iostream>
#include <vector>
#include "projectiondfatransitions.h"
#include "projectiontreenode.h"
#include "passiveprojectiontreenode.h"
#include "passiveprojectiontreeconfiguration.h"
#include "tokenconfiguration.h"
#include "role.h"

/*! @class ProjectionDFAState
 *     @brief Represents a single state of the ProjectionDFA.
 *     @details A ProjectionDFAState basically stores role information and a set of
 *              transitions into other states. It is matched by the StreamTracker
 *              against the input stream, in order to decide whether incoming tokens
 *              are appended to the buffer (including the associated roles) or projected
 *              away.
 *     @note Note that this DFA is tree-shaped, i.e. there is always a parent state
 *           (which is NULL for the root, of course).
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
class ProjectionDFAState {

public:
    /*! @brief Constructor for the root node.
     *  @details Constructor - creates a new ProjectionDFAState object associated
     *           to the root node of the input _pt object.
     *  @param[in] _pt The Projection tree. It forms the basis for the construction
     *                 of new ProjectionDFAState objects.
     */
    ProjectionDFAState(ProjectionTree* _pt,PassiveProjectionTree* _ppt);

    /*! @brief Constructor for non-root nodes.
     *  @details Constructor - creates a new ProjectionDFAState. This constructor
     *           is typically used for TEXT-states without roles (i.e. such induced
     *           by output nodes), which have no further outgoing transitions. 
     *  @param[in] _parent Pointer to the parent state.
     *  @param[in] _pconfig Pointer to the passive projection tree configuration.
     */ 
    ProjectionDFAState(ProjectionDFAState* _parent,PassiveProjectionTreeConfiguration* _pconf);

    /*! @brief Constructor for non-root nodes.
     *  @details Constructor - creates a new ProjectionDFAState associated to the
     *           passed token configuration.
     *  @param[in] _parent Pointer to the parent state.
     *  @param[in] _config The configuration of the new state (must be calculated).
     *  @param[in] _pconfig Pointer to the passive projection tree configuration.
     */ 
    ProjectionDFAState(ProjectionDFAState* _parent,
                        TokenConfiguration* _config,
                        PassiveProjectionTreeConfiguration* _pconfig);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ProjectionDFAState();

    /*! @brief Brings several member variables of this class up to date.
     *  @details The following member variables are updated: keep_node,
     *           keep_subtree, skip_subtree, cumulative_roles, and
     *           non_cumulative roles.
     *  @retval void
     */ 
    void update();

    /*! @brief Returns the state reached through the transition defined for tag t.
     *  @details May return NULL is the transition is not defined. The second parameter
     *           t_type is also set by the method, see the parameter description for
     *           more information.
     *  @param[in] t The tag for which the transition is searched for.
     *  @param[in] t_type The transition argument must be provided and is set
     *                    by this method. It will take one of the followin
     *                    values: TRANSITION_KEEP_SUBTREE means that, for
     *                    tag t, the whole subtree must be remembered;
     *                    TRANSITION_SKIP_SUBTREE means that, for tag t, the
     *                    whole substree must be skipped; TRANSITION_REGULAR
     *                    means that, for tag t, there is a regular transition
     *                    into the state that is passed as return value).
     *  @retval ProjectionDFAState* The state reached through the transition
     *                              defined for tag t.
     */
    ProjectionDFAState* takeTransition(TAG t,short& t_type);

    /*! @brief Returns the state reached through the text transition.
     *  @details Returns NULL if there is no text transition defined.
     *  @retval ProjectionDFAState* The state reached through the text transition. 
     */
    ProjectionDFAState* takeTextTransition();

    /*! @brief Returns the predecessor state of the current state.
     *  @details The ProjectionDFA is tree-shaped, so each state has
     *           exactly one predecessor, i.e. its parent. Note that
     *           the predecessor may also be NULL (for the inital state).
     *  @retval Returns the predecessor state.
     */
    ProjectionDFAState* traceBack() { return parent; }

    /*! @brief Returns true if the state forces to keep matching input
     *         document nodes.
     *  @details The associated member is computed through the update() method.
     *  @retval bool
     */
    inline bool keepNode() { return keep_node; }

    /*! @brief Returns true if the state forces to keep the subtree of matching
     *         input document nodes and there will (for sure) no roles be
     *         assigned to descendant nodes.
     *  @details The associated member is computed through the update() method.
     *  @retval bool
     */
    inline bool keepSubtree() { return keep_subtree; }

    /*! @brief Returns true if the state forces to keep the subtree of matching
     *         input document nodes.
     *  @details The associated member is computed through the update() method.
     *  @retval bool
     */
    inline bool isOutput() { return is_output; }

    /*! @brief Returns true if the state forces to skip the subtree of matching
     *         input document nodes.
     *  @details The associated member is computed through the update() method.
     *  @retval bool
     */
    inline bool skipSubtree() { return skip_subtree; }

    /*! @brief Returns true for the root state, false otherwise.
     *  @details
     *  @retval bool
     */ 
    inline bool isRoot() { return parent==NULL; }

    /*! @brief Returns the token configuration associated with this state.
     *  @details The TokenConfiguration specifies state in the ProjectionTree
     *           that are "matched" by this projection DFA state.
     *  @retval TokenConfiguration* The associated token configuration.
     */
    inline TokenConfiguration* getTokenConfiguration() { return config; }

    /*! @brief Returns the passive token configuration associated with this state.
     *  @details
     */
    inline PassiveProjectionTreeConfiguration* getPassiveProjectionTreeConfiguration()
        { return pconfig; }

    /*! @brief Debug prints the projection DFA state to STDOUT.
     *  @details
     *  @retval void
     */
    void print();

    /*! @brief Help method for print(), used for pretty printing.
     *  @details
     *  @param[in] tabs The number of tabs to be printed (pretty printing).
     *  @param[in] is_text True if the current state is a text state.
     *  @retval void
     */
    void print(unsigned tabs,bool is_text=false);

    /*! @brief Returns the cumulative roles associated with this state.
     *  @details The associated member is computed through the update() method.
     *  @retval vector<unsigned>* Pointer to the cumulative role vector.
     */
    vector<unsigned>* getCumulativeRoles() { return &cumulative_roles; }

    /*! @brief Returns the non-cumulative roles associated with this state.
     *  @details The associated member is computed through the update() method.
     *  @retval vector<unsigned>* Pointer to the non-cumulative role vector.
     */
    vector<unsigned>* getNonCumulativeRoles() { return &non_cumulative_roles; }

private:
    /*! @brief Sets up the transition for tag t.
     *  @details The transition is stored in transitions member variable.
     *  @param[in] t The tag for which the transition is to be calculated.
     */
    void computeTransition(TAG t);

    /*! @var ProjectionDFAState* parent
     *  @brief Pointer to the parent (=predecessor) state.
     *  @details
     */
    ProjectionDFAState* parent;

    /*! @var TokenConfiguration* config
     *  @brief The TokenConfiguration associated with this state.
     *  @details The TokenConfiguration object stores the position of all tokens
     *           that are currently placed placed in the associated ProjectionTree.
     *           Intuitively, these tokens identify the states in the ProjectionTree
     *           that are associated with this projection DFA state.
     */
    TokenConfiguration* config;

    /*! @var PassiveProjectionTreeConfiguration* pconfig
     *  @brief The configuration of the passive projection tree.
     */
    PassiveProjectionTreeConfiguration* pconfig;

    /*! @var ProjectionDFATransitions transitions
     *  @brief Stores the set of (currently computed) transitions into follow-up states.
     *  @details The ProjectionDFATransitions object also contains a lookup function
     *           that can be used to find transitions.
     */
    ProjectionDFATransitions transitions;

    /*! @var bool keep_node
     *  @brief True if the state forces to keep matching input document nodes.
     *  @details The member is computed through the update() method.
     */
    bool keep_node;

    /*! @var bool keep_subtree
     *  @brief True if the state forces to keep the subtree of matching
     *         input document nodes.
     *  @details The member is computed through the update() method.
     *           Keep_subtree states are similar to is_output states.
     *           The difference is that, in an output state, we might
     *           still be interested in matching descendants, as they
     *           get assigned roles. The keep_subtree flag is only set
     *           if descendant nodes will not be assigned any roles.
     */
    bool keep_subtree;

    /*! @var bool is_output 
     *  @brief True if the state describes an output state. 
     *  @details The member is computed through the update() method.
     *           Output states are similar to keep_subtree states.
     *           The difference is that, in an output state, we might
     *           still be interested in matching descendants, as they
     *           get assigned roles. The keep_subtree flag is only set
     *           if descendant nodes will not be assigned any roles.
     */
    bool is_output;

    /*! @var bool skip_subtree
     *  @brief Returns true if the state forces to skip the subtree of matching
     *         input document nodes.
     *  @details The associated member is computed through the update() method.
     */
    bool skip_subtree;

    /*! @var vector<Role*> roles
     *  @brief Contains roles associated with matching document nodes
     *  @details The member is computed through the update() method.
     *           The counts for these roles are stored in member variable
     *           role_counts (they are linked through the indices).
     */
    vector<Role*> roles;

    /*! @var vector<unsigned> role_counts
     *  @brief Contains role counts for the roles specified in member roles.
     *  @details The member is computed through the update() method.
     *           The associated roles are stored in member variable roles
     *           and are linked through the indices.
     */
    vector<unsigned> role_counts; 

    /*! @var vector<unsigned> cumulative_roles
     *  @brief The cumulative roles associated with this state.
     *  @details The associated member is computed through the update() method.
     *           The same information is also stored in the members roles and
     *           role_counts. This guy is for fast access only.
     */
    vector<unsigned> cumulative_roles;

    /*! @var vector<unsigned> non_cumulative_roles
     *  @brief The non-cumulative roles associated with this state.
     *  @details The associated member is computed through the update() method.
     *           The same information is also stored in the members roles and
     *           role_counts. This guy is for fast access only.
     */
    vector<unsigned> non_cumulative_roles;
};

#endif // PROJECTIONDFASTATE_H
