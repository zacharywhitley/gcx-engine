
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
 *  @brief Header file for projectiontreenode.cpp.
 *  @details Header file specifying constructors, destructor and functions for projectiontreenode.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PROJECTIONTREENODE_H
#define PROJECTIONTREENODE_H

#include "pathexpression.h"
#include "passiveprojectiontree.h"
#include "outputstreammanip.h"

/*! @class ProjectionTreeNode
 *  @brief Represents a single node of the constructed projection tree.
 *  @details Represents a single node of the constructed projection tree, i.e. of the constructed ProjectionTree,
 *     	which takes a variable as parent node, a variable as child node of this parent node, a PathExpression,
 *     	an indicator if this projection tree node derives from a dependency, a Role and an
 *  		indicator if this projection tree node or the corresponding ForExpression respectively
 *  		produces direct output, as arguments.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class ProjectionTreeNode {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a variable as parent, a variable as child node of this parent node,
     *                      a PathExpression, an indicator if this projection tree node derives from a dependency, a Role and an
     *                      indicator if this projection tree node or the corresponding ForExpression respectively produces direct output.
     *      @param[in] _parent Pointer to a ProjectionTreeNode object.
     *  @param[in] _var New Variable or child node respectively which this projection tree node referes to (-1 if none). 
     *  @param[in] _path Pointer to a PathExpression object.
     *  @param[in] _dep Indicator if this projection tree node derives from a dependency.
     *  @param[in] _role Pointer to a Role object.
     *  @param[in] _direct_output <tt>true</tt> if the associated for-loop directly produces output,
     *              <tt>false</tt> otherwise.
     */
    ProjectionTreeNode(ProjectionTreeNode * _parent, int _var,
                       PathExpression * _path, bool _dep, Role * _role,
                       bool _direct_output);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ ProjectionTreeNode();

    /*! @fn bool insertNode(int parent_var, int introduced_var, PathExpression* path, bool dep, Role* role, bool direct_output)
     *  @brief Inserts a new node into the projection tree.
     *  @details Inserts a new node into the projection tree, which is appended as child of the first argument of this method.
     *  @param[in] parent_var Parent node of a new node.
     *  @param[in] introduced_var New node or variable respectively to be inserted.
     *  @param[in] path Pointer to a PathExpression object.
     *  @param[in] dep Indicator if this projection tree node derives from a dependency.
     *  @param[in] role Pointer to a Role object.
     *  @param[in] direct_output <tt>true</tt> if the associated for-loop directly produces output,
     *                  <tt>false</tt> otherwise.
     *  @retval bool <tt>true</tt> if the new node was successfully inserted, <tt>false</tt> otherwise.
     */
    bool insertNode(int parent_var, int introduced_var,
                    PathExpression * path, bool dep, Role * role,
                    bool direct_output);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints this single ProjectionTreeNode.
     *  @details Prints this single ProjectionTreeNode.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline ProjectionTreeNode* getParent()
     *  @brief Returns the parent node of a node.
     *  @details Returns the parent node of a node, i.e. returns the member variable.
     *  @retval ProjectionTreeNode Pointer to a ProjectionTreeNode object.
     */
    inline ProjectionTreeNode *getParent() {
        return parent;
    }
    /*! @fn inline int getVar()
     *  @brief Returns the variable of a node.
     *  @details Returns the variable of a node, to which this node referes to (-1 if none).
     *  @retval int Variable to which this node referes to (-1 if none).
     */ inline int getVar() {
        return var;
    }

    /*! @fn inline PathExpression* getPath()
     *  @brief Returns the path of a node.
     *  @details Returns the path of a node, i.e. returns the member variable.
     *  @retval PathExpression Pointer to a PathExpression object.
     */
    inline PathExpression *getPath() {
        return path;
    }

    /*! @fn inline Role* getRole()
     *  @brief Returns the role of a node.
     *  @details Returns the role of a node, i.e. returns the member variable.
     *  @retval Role Pointer to a Role object.
     */
    inline Role *getRole() {
        return role;
    }

    /*! @fn inline vector<ProjectionTreeNode*>* getChildren()
     *  @brief Returns all children of a node.
     *  @details Returns all children of a node, i.e. returns the member variable.
     *  @retval vector<ProjectionTreeNode*>* Pointer to a vector containing ProjectionTreeNode objects.
     */
    inline vector < ProjectionTreeNode * >*getChildren() {
        return &children;
    }

    /*! @fn PathExpression* getRootPath()
     *  @brief Returns the path from the root node of the projection tree to this node (including its own path).
     *  @details Returns the path from the root node of the projection tree to this node (including its own path),
     *                  Although <tt>NULL</tt> paths are ignore. The returned path is a fresh copy (with all path
     *                  steps cloned) and must be deleted externally.
     *  @retval PathExpression Pointer to a PathExpression object.
     */
    PathExpression *getRootPath();

    /*! @fn void getPathsFromRoot(vector<PathExpression*>& paths)
     *  @brief Returns the collection of paths from the root node of the projection tree down to this node.
     *  @details Returns the collection of paths from the root node of the projection tree down to this node.
     *                  The passed vector is filled with pointers to the original paths, nothing must be deleted.
     *  @param[in,out] paths Pointer to a vector containing Pointer to PathExpression objects. 
     *  @retval void
     */
    void getPathsFromRoot(vector < PathExpression * >&paths);

    /*! @fn inline bool isRoot()
     *  @brief Returns if this projection tree node is the root node of the projection tree.
     *  @details Returns if this projection tree node is the root node of the projection tree. 
     *  @retval bool <tt>true</tt> if this projection tree node is the root node of the projection tree,
     *                  <tt>false</tt> otherwise.
     */
    inline bool isRoot() {
        return parent == NULL;
    }

    /*! @fn inline bool isDepNode()
     *  @brief Returns if this projection tree node derives from a dependency.
     *  @details Returns if this projection tree node derives from a dependency. 
     *  @retval bool <tt>true</tt> if this projection tree node derives from a dependency,
     *                  <tt>false</tt> otherwise.
     */
    inline bool isDepNode() {
        return dep;
    }

    /*! @fn inline bool isPosDepNode()
     *  @brief Returns if this projection tree node derives from a dependency with a <tt>[position</tt><tt>()=1]</tt> PathStepAttributePosition.
     *  @details Returns if this projection tree node derives from a dependency with a <tt>[position</tt><tt>()=1]</tt> PathStepAttributePosition. 
     *  @retval bool <tt>true</tt> if this projection tree node derives from a dependency with a <tt>[position</tt><tt>()=1]</tt> PathStepAttributePosition,
     *                  <tt>false</tt> otherwise.
     */
    inline bool isPosDepNode() {
        return role && role->isPosRole();
    }

    /*! @fn inline bool isDosDepNode()
     *  @brief Returns if this projection tree node derives from a dependency with a <tt>dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt> PathStepExpression.
     *  @details Returns if this projection tree node derives from a dependency with a <tt>dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt> PathStepExpression. 
     *  @retval bool <tt>true</tt> if this projection tree node derives from a dependency with a <tt>dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt> PathStepExpression,
     *                  <tt>false</tt> otherwise.
     */
    inline bool isDosDepNode() {
        return role && role->isDosRole();
    }

    /*! @fn inline void clearChildren()
     *  @brief Removes/Deletes all children of this projection tree node.
     *  @details Removes/Deletes all children of this projection tree node. 
     *  @retval void
     */
    inline void clearChildren() {
        children.clear();
    }

    /*! @fn void removeUnneededNodes(PassiveProjectionTree* ppt)
     *  @brief Removes/Deletes all unneeded nodes of the projection tree.
     *  @details Removes/Deletes all unneeded nodes of the projection tree.
     *  @param[in] ppt Pointer to a PassiveProjectionTree object (to register dropped nodes).
     *  @retval void
     */
    void removeUnneededNodes(PassiveProjectionTree * ppt);

    /*! @fn void removeRedundantRoles(PassiveProjectionTree* ppt)
     *  @brief Removes/Deletes all redundant roles/nodes of the projection tree.
     *  @details Removes/Deletes all redundant roles/nodes of the projection tree.
     *  @param[in] ppt Pointer to a PassiveProjectionTree object (to register dropped nodes).
     *  @retval void
     */
    void removeRedundantRoles(PassiveProjectionTree * ppt);

    /*! @fn void registerToPassiveProjectionTree(PassiveProjectionTree* ppt)
     *  @brief Registers a projection tree node to the passive projection tree.
     *  @details Registers a projection tree node to the passive projection tree.
     *                  This method is called whenever a projection tree node is deleted.
     *  @param[in] ppt Pointer to a PassiveProjectionTree object (to register dropped nodes).
     *  @retval void
     */
    void registerToPassiveProjectionTree(PassiveProjectionTree * ppt);

  private:
    /*! @fn void print(OutputStream& dos, unsigned indents)
     *  @brief Prints this single ProjectionTreeNode with whitespace indenting.
     *  @details Prints this single ProjectionTreeNode with whitespace indenting.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @param[in] indents Number of whitespaces to put before output or of a ProjectionTreeNode respectively.
     *  @retval void
     */
    void print(OutputStream & dos, unsigned indents);

    /*! @fn void removeSubtreeInclSelf(PassiveProjectionTree* ppt)
     *  @brief Removes/Deletes a subtree (<tt>including</tt> the calling node itself) from a projection tree node on.
     *  @details Removes/Deletes a subtree (<tt>including</tt> the calling node itself) from a projection tree node on.
     *  @param[in] ppt Pointer to a PassiveProjectionTree object (to register dropped nodes).
     *  @retval void
     */
    void removeSubtreeInclSelf(PassiveProjectionTree * ppt);

    /*! @fn void removeSubtreeExclSelf(PassiveProjectionTree* ppt)
     *  @brief Removes/Deletes a subtree (<tt>excluding</tt> the calling node itself) from a projection tree node on.
     *  @details Removes/Deletes a subtree (<tt>excluding</tt> the calling node itself) from a projection tree node on.
     *  @param[in] ppt Pointer to a PassiveProjectionTree object (to register dropped nodes).
     *  @retval void
     */
    void removeSubtreeExclSelf(PassiveProjectionTree * ppt);

    /*! @fn void setRedundantRoleSelf()
     *  @brief Sets a role of a projection tree as redundant.
     *  @details Sets a role of a projection tree as redundant, i.e. deletes this Role.
     *  @retval void
     */
    void setRedundantRoleSelf();

    /*! @var ProjectionTreeNode* parent
     *  @brief The entered ProjectionTreeNode (parent node).
     *  @details The entered ProjectionTreeNode (parent node), which is the first argument of the constructor.
     */
    ProjectionTreeNode *parent;

    /*! @var int var
     *  @brief The entered variable to which the projection tree node referes to (-1 if none).
     *  @details The entered variable to which the projection tree node referes to (-1 if none),
     *                  which is the second argument of the constructor.
     */
    int var;

    /*! @var PathExpression* path
     *  @brief The entered path.
     *  @details The entered path, which is the third argument of the constructor.
     */
    PathExpression *path;

    /*! @var bool dep
     *  @brief The entered indicator if this projection tree node derives from a dependency.
     *  @details The entered indicator if this projection tree node derives from a dependency,
     *                  which is the fourth argument of the constructor.
     */
    bool dep;

    /*! @var Role* role
     *  @brief The entered role.
     *  @details The entered role, which is the fifth argument of the constructor.
     */
    Role *role;

    /*! @var vector<ProjectionTreeNode*> children
     *  @brief All child nodes of a ProjectionTreeNode.
     *  @details All child nodes of a ProjectionTreeNode.
     */
    vector < ProjectionTreeNode * >children;

    /*! @var bool direct_output
     *  @brief The entered indicator if the associated for-loop of a node directly produces output.
     *  @details The entered indicator if the associated for-loop of a node directly produces output
     *                  such as node construct or constant string output.
     */
    bool direct_output;
};

#endif // PROJECTIONTREENODE_H
