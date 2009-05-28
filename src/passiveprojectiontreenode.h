
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
 *  @brief Header file for passiveprojectiontreenode.cpp.
 *  @details Header file specifying constructors, destructor and functions for passiveprojectiontreenode.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PASSIVEPROJECTIONTREENODE_H
#define PASSIVEPROJECTIONTREENODE_H

#include "pathexpression.h"
#include "outputstreammanip.h"

//! @namespace std
using namespace std;

/*! @class PassiveProjectionTreeNode
 *  @brief A node of the passive projection tree.
 *  @details Each node in the passive projection tree describes exactly one path step
 *    (i.e., is labeled with a path step or NULL, for the root).
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */

class PassiveProjectionTreeNode {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for the root node.
     */
    PassiveProjectionTreeNode();

    /*! @brief Constructor.
     *  @details Constructor - creating object for a labeled passive projection tree node.
     *  @param[in] _parent The designated parent of this node. 
     *  @param[in] _label The designated label of this node. 
     */
    PassiveProjectionTreeNode(PassiveProjectionTreeNode * _parent,
                              PathStepExpression * _label);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ PassiveProjectionTreeNode();

    /*! @fn void registerPath(PathExpression* path, unsigned pos)
     *  @brief Registers a path to the passive projection tree node.
     *  @details The path is automatically inserted at the right position.
     *  @param[in] path The path to be registered.
     *  @param[in] pos The current position within the path
     *  @retval void
     */
    void registerPath(PathExpression * path, unsigned pos);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints the path expression node.
     *  @details This method is for debugging purpose only. 
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline PassiveProjectionTreeNode* getParent()
     *  @brief Returns the parent of this passive projection tree node.
     *  @details Returns NULL for the root node.
     *  @retval PassiveProjectionTreeNode* Pointer to the parent node
     *    of the current ProjectionTreeNode, if it exists, NULL otherwise.
     */
    inline PassiveProjectionTreeNode *getParent() {
        return parent;
    }
    /*! @fn inline vector<PassiveProjectionTreeNode*>* getChildren()
     *  @brief Returns the children of this PassiveProjectionTreeNode.
     *  @details The children are all stored in member childre.
     *  @retval vector<PassiveProjectionTreeNode*>* Pointer to the
     *    vector containing the children of the curren PassiveProjectionTreeNode.
     */ inline vector < PassiveProjectionTreeNode * >*getChildren() {
        return &children;
    }

    /*! @fn inline PathStepExpression* getLabel()
     *  @brief Returns the label of the passive projection tree node.
     *  @details Note that the label of the root node is NULL.
     *  @retval PathStepExpression* Pointer to the PathStepExpression label.
     */
    inline PathStepExpression *getLabel() {
        return label;
    }

  private:
    /*! @fn void print(OutputStream& dos, unsigned indents)
     *  @brief Prints the path expression node fixed indent.
     *  @details Help function of public print() method.
     *  @param[in] dos Refernece to the (debug) OutputStream.
     *  @param[in] indents Number of indent characters.
     *  @retval void
     */
    void print(OutputStream & dos, unsigned indents);

    /*! @var PassiveProjectionTreeNode* parent
     *  @brief Pointer to the parent of this passive projection tree node.
     *  @details Is NULL for the root node.
     */
    PassiveProjectionTreeNode *parent;

    /*! @var PathStepExpression* label
     *  @brief The path step expression associated with this node.
     *  @details Is NULL for the root node.
     */
    PathStepExpression *label;

    /*! @var vector<PassiveProjectionTreeNode*> children
     *  @brief The children of this passive projection tree node.
     *  @details The children of this passive projection tree node.
     */
    vector < PassiveProjectionTreeNode * >children;
};

#endif // PASSIVEPROJECTIONTREENODE_H
