
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
 *  @brief Header file for projectiontree.cpp.
 *  @details Header file specifying constructors, destructor and functions for projectiontree.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PROJECTIONTREE_H
#define PROJECTIONTREE_H

#include "projectiontreenode.h"
#include "variabletree.h"
#include "dependencyset.h"
#include "projectiontreelabels.h"

/*! @class ProjectionTree
 *  @brief Represents the constructed projection tree.
 *  @details Represents the constructed projection tree, which is constructed through a VariableTree and a DependencySet.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class ProjectionTree {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object from a VariableTree and a vector of DependencySet.
     *  @param[in] _vt Pointer to a VariableTree object.
     *  @param[in] _depset Pointer to a vector containing pointers to all DependencySet objects.
     */
    ProjectionTree(VariableTree * _vt, vector < DependencySet * >*_depset);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ ProjectionTree();

    /*! @fn void insertNode(int parent_var, int introduced_var, PathExpression* path, bool dep, Role* role, bool direct_output)
     *  @brief Inserts a new node into the projection tree.
     *  @details Inserts a new node into the projection tree, which is appended as child of the first argument of this method.
     *  @param[in] parent_var Parent node of the new node.
     *  @param[in] introduced_var New node or variable respectively to be inserted.
     *  @param[in] path Pointer to a PathExpression object, which is linked to the variable.  
     *  @param[in] dep Indicator if new node or variable respectively derives from a dependency,
     *    i.e. <tt>true</tt> if it derives from a DependencySet, <tt>false</tt> otherwise.
     *  @param[in] role Pointer to a Role object, which is associated with this new node or variable respectively.
     *  @param[in] direct_output Indicator if new node or variable respectively has direct output in its corresponding ForExpression,
     *    i.e. <tt>true</tt> if direct output is present, <tt>false</tt> otherwise.
     *  @retval void
     */
    void insertNode(int parent_var, int introduced_var,
                    PathExpression * path, bool dep, Role * role,
                    bool direct_output);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints the complete projection tree.
     *  @details Prints the complete projection tree.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline ProjectionTreeNode* getRootNode()
     *  @brief Returns the root node of the projection tree.
     *  @details Returns the root node of the projection tree, i.e. returns the member variable.
     *  @retval ProjectionTreeNode* Pointer to a ProjectionTreeNode object.
     */
    inline ProjectionTreeNode *getRootNode() {
        return root;
    }
    /*! @fn inline ProjectionTreeLabels* getLabels()
     *  @brief Returns the labels of the projection tree.
     *  @details Returns the labels of the projection tree, i.e. returns the member variable.
     *  @retval ProjectionTreeLabels* Pointer to a ProjectionTreeLabels object.
     */ inline ProjectionTreeLabels *getLabels() {
        return labels;
    }

    /*! @fn void labelTree()
     *  @brief Labels the whole projection tree.
     *  @details Labels the whole projection tree.
     *  @note Apply this method after all optimization!
     *  @retval void
     */
    void labelTree();

    /*! @fn void removeUnneededNodes(PassiveProjectionTree* ppt)
     *  @brief Removes all unneeded nodes in the projection tree.
     *  @details Removes all unneeded nodes in the projection tree.
     *  @param[in] ppt Pointer to a PassiveProjectionTree object, which keeps all dropped nodes.
     *  @retval void
     */
    void removeUnneededNodes(PassiveProjectionTree * ppt);

    /*! @fn void removeRedundantRoles(PassiveProjectionTree* ppt)
     *  @brief Removes all redundant roles/nodes in the projection tree.
     *  @details Removes all redundant roles/nodes in the projection tree.
     *  @param[in] ppt Pointer to a PassiveProjectionTree object, which keeps all dropped nodes.
     *  @retval void
     */
    void removeRedundantRoles(PassiveProjectionTree * ppt);

  private:
    /*! @fn void createProjectionTree(VariableTreeNode* varnode, vector<DependencySet*>* depset)
     *  @brief Creates the projection tree.
     *  @details Creates the projection tree.
     *  @param[in] varnode Pointer to a VariableTreeNode object (root node of the variable tree).
     *  @param[in] depset Pointer to a vector containing pointers to all DependencySet objects.
     *  @retval void
     */
    void createProjectionTree(VariableTreeNode * varnode,
                              vector < DependencySet * >*depset);

    /*! @var ProjectionTreeNode* root
     *  @brief The root node of the projection tree.
     *  @details The root node of the projection tree.
     */
    ProjectionTreeNode *root;

    /*! @var RoleList* roleinfo
     *  @brief The list of roles containing all necessary Role information of the projection tree.
     *  @details The list of roles containing all necessary Role information of the projection tree.
     */
    RoleList *roleinfo;

    /*! @var ProjectionTreeLabels* labels
     *  @brief Containing all ProjectionTreeLabels of the projection tree.
     *  @details Containing all ProjectionTreeLabels of the projection tree.
     */
    ProjectionTreeLabels *labels;
};

#endif // PROJECTIONTREE_H
