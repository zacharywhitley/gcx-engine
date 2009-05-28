
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
 *  @brief Header file for passiveprojectiontree.cpp.
 *  @details Header file specifying constructors, destructor and functions for passiveprojectiontree.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PASSIVEPROJECTIONTREE_H
#define PASSIVEPROJECTIONTREE_H

#include "passiveprojectiontreenode.h"
#include "pathenvironment.h"

/*! @class PassiveProjectionTree
 *  @brief Class for the passive projection tree.
 *  @details The passive projection tree is kept up to avoid the interleaving of path
 *    steps that have been optimized away. Basically, each node represents a
 *    single path step, and each path from the root describes a complete path.
 *    Nodes matched on this path during preprojection must not be interleaved.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class PassiveProjectionTree {

  public:
    /*! @brief Constructor. 
     *  @details Constructs - creating object for a PathEnvironment.
     *  @param[in] _penv Pointer to the PathEnvironment object.
     */
    PassiveProjectionTree(PathEnvironment * _penv);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ PassiveProjectionTree();

    /*! @fn void registerPath(PathExpression* path)
     *  @brief Adds a path to the passive projection tree.
     *  @details The nodes of the path are inserted at the correct positions.
     *  @param[in] path The path to be registered.
     *  @retval void
     */
    void registerPath(PathExpression * path);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints ths passive projection tree node.
     *  @details This method is for debug purposes only.
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline PassiveProjectionTreeNode* getRootNode()
     *  @brief Returns the root node of the PassiveProjectionTree.
     *  @details The root node of the PassiveProjectionTree 
     *    corresponds to the virtual XML document root node.
     *  @retval PassiveProjectionTreeNode* Pointer to the root node of
     *    PassiveProjectionTree.
     *  
     */
    inline PassiveProjectionTreeNode *getRootNode() {
        return root;
    }
    /*! @fn inline PathEnvironment* getPathEnv()
     *  @brief Returns the path environment.
     *  @details The path environment stores variable-to-path mappings.
     *  @retval PathEnvironment* Pointer to the PathEnvironment object.
     */ inline PathEnvironment *getPathEnv() {
        return penv;
    }

  private:
    /*! @var PassiveProjectionTreeNode* root
     *  @brief The root node of the tree.
     *  @details The root node of the PassiveProjectionTree 
     *    corresponds to the virtual XML document root node.
     */
    PassiveProjectionTreeNode * root;

    /*! @var PathEnvironment* penv
     *  @brief The path environment.
     *  @details The path environment stores variable-to-path mappings.
     */
    PathEnvironment *penv;
};

#endif // PASSIVEPROJECTIONTREE_H
