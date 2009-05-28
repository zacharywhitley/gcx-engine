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
 *     @brief Header file for passiveprojectiotreeconfiguration.cpp.
 *     @details Header file specifying constructors, destructor and functions
 *              for passiveprojectiontreeconfiguration.cpp.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#ifndef PASSIVEPROJECTIONTREECONFIGURATION_H
#define PASSIVEPROJECTIONTREECONFIGURATION_H

#include <vector>
#include "passiveprojectiontree.h"

//! @namespace std
using namespace std;

/*! @class PassiveProjectionTreeConfiguration
 *     @brief Represents a token configuration on top of a passive projection tree.
 *     @details A token configuration touroughly describes a set of active and passive
 *               state of a PassiveProjectionTreel.
 *     @author Michael Schmidt
 *     @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
class PassiveProjectionTreeConfiguration {
  
public:
    /*! @brief Constructor.
     *  @details Constructor - creates passive projection tree configuration, with
     *           one token placed at the root node.
     */
    PassiveProjectionTreeConfiguration(PassiveProjectionTree* ppt);

    /*! @brief Constructor.
     *  @details Constructor - creates a configuration with given passive_nodes vector.
     *  @param[in] vector<PassiveProjectionTreeNode*> The desired passive token vector.
     */
    PassiveProjectionTreeConfiguration(vector<PassiveProjectionTreeNode*> _passive_nodes);


    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~PassiveProjectionTreeConfiguration();

    /*! @brief Returns true if the configuration has any active states.
     *  @retval bool
     */
    inline bool hasActiveNodes() { return active_nodes.size()>0; }

    /*! @brief Compute the configuration that results when movingtokens along tag t.
      * @details
      * @param[in] TAG t The tag to be applied.
      * @retval PassiveProjectionTreeConfiguration* The resulting token configuration.
      */
    PassiveProjectionTreeConfiguration* applyTag(TAG t);

    /*! @brief Compute the configuration that results when PCDATA content is read. 
      * @details
      * @retval PassiveProjectionTreeConfiguration* The resulting token configuration.
      */
    PassiveProjectionTreeConfiguration* applyText();

    /*! @fn addActiveNode(PassiveProjectionTreeNode* n)
     *  @brief Adds an active node to a configuration.
     *  @param[in] PassiveProjectionTreeNode* n The node to be added.
     *  @retval void
     */
    inline void addActiveNode(PassiveProjectionTreeNode* n) { active_nodes.push_back(n); }

    /*! @fn addPassiveNode(PassiveProjectionTreeNode* n)
     *  @brief Adds a passivenode to a configuration.
     *  @param[in] PassiveProjectionTreeNode* n The node to be added.
     *  @retval void
     */
    inline void addPassiveNode(PassiveProjectionTreeNode* n) { passive_nodes.push_back(n); }

private:
    /*! @brief The active ProjectionTreeNode objects, i.e. those nodes currently matched.
     */
    vector<PassiveProjectionTreeNode*> active_nodes;

    /*! @brief The active ProjectionTreeNode objects, i.e. those nodes currently matched.
     */
    vector<PassiveProjectionTreeNode*> passive_nodes;

};

#endif // PASSIVEPROJECTIONTREECONFIGURATION_H
