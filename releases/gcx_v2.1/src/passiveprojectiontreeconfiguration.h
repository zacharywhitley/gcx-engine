
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
 *  @brief Header file for passiveprojectiotreeconfiguration.cpp.
 *  @details Header file specifying constructors, destructor and functions for passiveprojectiontreeconfiguration.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PASSIVEPROJECTIONTREECONFIGURATION_H
#define PASSIVEPROJECTIONTREECONFIGURATION_H

#include <vector>
#include "passiveprojectiontree.h"

//! @namespace std
using namespace std;

/*! @class PassiveProjectionTreeConfiguration
 *  @brief Represents a token configuration on top of a passive projection tree.
 *  @details A token configuration touroughly describes a set of active and passive
  *   state of a PassiveProjectionTreel.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class PassiveProjectionTreeConfiguration {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object with one token placed at the root node.
     *  @param[in] ppt Pointer to a PassiveProjectionTree object.
     */
    PassiveProjectionTreeConfiguration(PassiveProjectionTree * ppt);

    /*! @brief Constructor.
     *  @details Constructor - creating object for a configuration with given passive_nodes vector.
     *  @param[in] _passive_nodes The desired passive token vector.
     */
    PassiveProjectionTreeConfiguration(vector <
                                       PassiveProjectionTreeNode *
                                       >_passive_nodes);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ PassiveProjectionTreeConfiguration();

    /*! @fn inline void addActiveNode(PassiveProjectionTreeNode* n)
     *  @brief Adds an active node to a configuration.
     *  @details Simple push back applied to the active_nodes member.
     *  @param[in] n The node to be added.
     *  @retval void
     */
    inline void addActiveNode(PassiveProjectionTreeNode * n) {
        active_nodes.push_back(n);
    }
    /*! @fn inline void addPassiveNode(PassiveProjectionTreeNode* n)
     *  @brief Adds a passive node to a configuration.
     *  @details Simple push back applied to the passive_nodes member.
     *  @param[in] n The node to be added.
     *  @retval void
     */ inline void addPassiveNode(PassiveProjectionTreeNode * n) {
        passive_nodes.push_back(n);
    }

    /*! @fn PassiveProjectionTreeConfiguration* applyTag(TAG t)
     *  @brief Compute the configuration that results when moving tokens along tag t.
     *  @details Creates a fresh PassiveProjectionTreeConfiguration object.
     *  @param[in] t The tag to be applied.
     *  @retval PassiveProjectionTreeConfiguration* The resulting token configuration.
     */
    PassiveProjectionTreeConfiguration *applyTag(TAG t);

    /*! @fn PassiveProjectionTreeConfiguration* applyText()
     *  @brief Compute the configuration that results when PCDATA content is read. 
     *  @details Creates a fresh PassiveProjectionTreeConfiguration object.
     *  @retval PassiveProjectionTreeConfiguration* The resulting token configuration.
     */
    PassiveProjectionTreeConfiguration *applyText();

    /*! @fn inline bool hasActiveNodes()
     *  @brief Checks for active nodes in the current configuration.
     *  @details Checks for active nodes in the current configuration.
     *  @retval bool True if the current configuration has any active nodes,
     *    false otherwise.
     */
    inline bool hasActiveNodes() {
        return active_nodes.size() > 0;
    }

  private:
    /*! @var vector<PassiveProjectionTreeNode*> active_nodes
     *  @brief The active ProjectionTreeNode nodes.
     *  @details The active ProjectionTreeNode objects, i.e. the set of nodes
     *    that are actively matched by the current configuration.
     */
    vector < PassiveProjectionTreeNode * >active_nodes;

    /*! @var vector<PassiveProjectionTreeNode*> passive_nodes
     *  @brief The active ProjectionTreeNode objects, i.e. those nodes currently matched.
     *  @details The active ProjectionTreeNode objects, i.e. the set of nodes
     *    that are passively matched by the current configuration.
     */
    vector < PassiveProjectionTreeNode * >passive_nodes;
};

#endif // PASSIVEPROJECTIONTREECONFIGURATION_H
