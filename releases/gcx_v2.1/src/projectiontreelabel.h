
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
 *  @brief Header file for projectiontreelabel.cpp.
 *  @details Header file specifying constructors, destructor and functions for projectiontreelabel.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PROJECTIONTREELABEL_H
#define PROJECTIONTREELABEL_H

#include "projectiontreenode.h"
#include "runtimeexception.h"

/*! @class ProjectionTreeLabel
 *  @brief Represents labels for positions in the projection trees.
 *  @details More precisely, each path step expression in the projection
 *    tree is assigned a unique label. In addition, projection tree
 *    nodes with NULL path get assigned a label, too. For each label,
 *    we store different types of successor labels, such as vertical
 *    and horizontal successors.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class ProjectionTreeLabel {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object.
     *  @param[in] _n The projection tree node the label is assigned to.
     *  @param[in] _position The position of the path step stored in node _n.
     *  @param[in] _id The id of the ProjectionTreeLabel.
     *  @param[in] _same_level_successor The ProjectionTreeLabel that 
     *    describes the path step to the right (in the same node _n) of this label. May be NULL. 
     *  @param[in] _child_successors The set of ProjectionTreeLabels in the
     *    first position of paths contained in child nodes of _n, but only if this label
     *    is set to the last position of _n's path. Otherwise, the vector is empty.
     *  @param[in] _self_successors The set of labels that describes child
     *    nodes of _n that carry no path, i.e. the NULL path, and recursively their children
     *    if these satisfy the same condition.
     */
    ProjectionTreeLabel(ProjectionTreeNode * _n,
                        unsigned _position, unsigned _id,
                        ProjectionTreeLabel * _same_level_successor,
                        vector < ProjectionTreeLabel * >_child_successors,
                        vector < ProjectionTreeLabel * >_self_successors);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ ProjectionTreeLabel();

    /*! @fn void updateParentPredecessorInformation(ProjectionTreeLabel* _parent, ProjectionTreeLabel* _predecessor)
     *  @brief Recursively brings the parent and predecessor links up to date.
     *  @details To be applied exactly once after all labels have been collected.
     *  @param[in] _parent Pointer to the parent projection tree label.
     *  @param[in] _predecessor Pointer to the predecessor label.
     *  @retval void
     */
    void updateParentPredecessorInformation(ProjectionTreeLabel * _parent,
                                            ProjectionTreeLabel * _predecessor);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints the projection tree label.
     *  @details Used for debugging purpose only.
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline ProjectionTreeLabel* getParent()
     *  @brief Returns the parent label.
     *  @details A projection tree label is called parent label if it refers the last past step in the parent
     *    projection tree node.
     *  @retval ProjectionTreeLabel* The parent label.
     */
    inline ProjectionTreeLabel *getParent() {
        return parent;
    }
    /*! @fn inline ProjectionTreeLabel* getPredecessor()
     *  @brief Returns the predecessor label.
     *  @details A projection tree label is called predecessor if it is reached by
     *    going one path step back. The predecessor (in contrast to the parent)
     *    might also be in the same level. If it is not, it equals to the parent.
     *  @retval ProjectionTreeLabel* The predecessor label.
     */ inline ProjectionTreeLabel *getPredecessor() {
        return predecessor;
    }

    /*! @fn ProjectionTreeLabel* getLeftmostSLPredecessor()
     *  @brief Returns the first label associated with the same path step.
     *  @details Steps back in the current path step and returns the label
     *     associated with the leftmost path step.
     *  @retval ProjectionTreeLabel* The first label associated with this path step.
     */
    ProjectionTreeLabel *getLeftmostSLPredecessor();

    /*! @fn unsigned getDependingVar()
     *  @brief Returns the id of the variable the label ultimately depends on.
     *  @details This is the variable that is associated with the associated projection tree node.
     *  @retval unsigned The variable id.
     */
    unsigned getDependingVar();

    /*! @fn PathExpression* getPath()
     *  @brief Return the path that is referenced by this label.
     *  @details The path may be NULL.
     *  @retval PathExpression* Pointer to the path expression.
     */
    PathExpression *getPath();

    /*! @fn PathStepExpression* getPathStep()
     *  @brief Get the path step that is references by this label.
     *  @details The path step may be NULL (if and only if the path is NULL).
     *  @retval PathStepExpression* Pointer to the path step expression.
     */
    PathStepExpression *getPathStep();

    /*! @fn TAG getTag()
     *  @brief Returns the tag carried by the associated path step expression.
     *  @details The caller must ensure that the associated path step expression differs from NULL.
     *  @retval TAG The tag id of the associated path step expression.
     *  @exception Throws a RuntmeException if the path step is (errorneously)
     *    applied to a non-tag node test.
     */
    TAG getTag();

    /*! @fn AXIS_TYPE getAxisFollowingFSA()
     *  @brief Returns the axis type that, going up from this label,
     *    immediately follows its first straight ancestor (FSA) label.
     *  @details Used to determinde the multiplicity of role assignments.
     *  @retval AXIS_TYPE The type of the computed axis.
     */
    AXIS_TYPE getAxisFollowingFSA();

    /*! @fn AXIS_TYPE getAxisFollowingFSA(unsigned fsa)
     *  @brief Returns the axis type that, going up from this label,
     *    immediately follows the given first straight ancestor (FSA) id.
     *  @details This is a help function for method getAxisFollowingFSA().
     *  @param[in] fsa The first straight ancestor (FSA).
     *  @retval AXIS_TYPE The type of the computed axis.
     */
    AXIS_TYPE getAxisFollowingFSA(unsigned fsa);

    /*! @fn ProjectionTreeLabel* getFSALabel()
     *  @brief Returns the first straight ancestor (FSA) label.
     *  @details The first straight ancestor (FSA) label is the label of the last path
     *    step of the path that is bound to the first straight ancestor (FSA) variable
     *  @retval ProjectionTreeLabel* Pointer to the FSA ProjectionTreeLabel.
     */
    ProjectionTreeLabel *getFSALabel();

    /*! @fn inline unsigned getId()
     *  @brief The ID of the label.
     *  @details The uniqu ID of the current label.
     *  @retval unsigned The ID of the label.
     */
    inline unsigned getId() {
        return id;
    }

    /*! @fn inline unsigned getPosition()
     *  @brief Returns the path step position inside the path. 
     *  @details Starts counting at 0, i.e. the first path step position is
     *    position 0.
     *  @retval unsigned The path step position inside the path.
     */
    inline unsigned getPosition() {
        return position;
    }

    /*! @fn inline ProjectionTreeLabel* getSameLevelSuccessor()
     *  @brief Returns the same level successor.
     *  @details May return NULL if no same level successor exists.
     *  @retval ProjectionTreeLabel* Pointer to the same level successor.
     */
    inline ProjectionTreeLabel *getSameLevelSuccessor() {
        return same_level_successor;
    }

    /*! @fn inline vector<ProjectionTreeLabel*>* getChildSuccessors()
     *  @brief Returns the child successors. 
     *  @details Returns the set of all child successors of the current label.
     *  @retval vector<ProjectionTreeLabel*>* Pointer to the vector containing
     *    the child successors.
     */
    inline vector < ProjectionTreeLabel * >*getChildSuccessors() {
        return &child_successors;
    }

    /*! @fn inline vector<ProjectionTreeLabel*>* getSelfSuccessors()
     *  @brief Returns the self successors. 
     *  @details Returns the set of all self-axis successors of the current label.
     *  @retval vector<ProjectionTreeLabel*>* Pointer to the vector containing
     *    the self successors.
     */
    inline vector < ProjectionTreeLabel * >*getSelfSuccessors() {
        return &self_successors;
    }

    /*! @fn inline ProjectionTreeNode* getProjectionTreeNode()
     *  @brief Returns the associated projection tree node.
     *  @details Will always be different from NULL.
     *  @retval ProjectionTreeNode* The associated projection tree node.
     */
    inline ProjectionTreeNode *getProjectionTreeNode() {
        return n;
    }

    /*! @fn bool isChildOrDescendantLabel()
     *  @brief Returns true if the associated path step carries a child or descendant axis.
     *  @details In particular, the method returns false for dos-axis path steps.
     *  @retval bool True if the associated path step carries a child or descendant
     *    axis label, false otherwise.
     */
    bool isChildOrDescendantLabel();

    /*! @fn bool isDescendantLabel()
     *  @brief Returns true if the associated path step carries a descendant axis.
     *  @details Returns true if the associated path step carries a descendant axis.
     *  @retval bool True if the associated path step carries a descendant
     *    axis label, false otherwise.
     */
    bool isDescendantLabel();

    /*! @fn bool isChildLabel()
     *  @brief Returns true if the associated path step carries a child axis.
     *  @details Returns true if the associated path step carries a child axis.
     *  @retval bool True if the associated path step carries a child axis,
     *    false otherwise.
     */
    bool isChildLabel();

    /*! @fn bool isDosLabel()
     *  @brief Returns true if the associated path step carries a dos axis.
     *  @details Returns true if the associated path step carries a dos axis.
     *  @retval bool True if the associated path step carries a dos axis,
     *    false otherwise.
     */
    bool isDosLabel();

    /*! @fn bool isDosNodeLabel()
     *  @brief Returns true if the associated path step is dos::node.
     *  @details Returns true if the associated path step is dos::node.
     *  @retval bool True if the associated path step is dos::node,
     *    false otherwise.
     */
    bool isDosNodeLabel();

    /*! @fn bool isDosOrDescendantLabel()
     *  @brief Returns true if the associated path step carries either a
     *    descendant or dos path step.
     *  @details Returns true if the associated path step carries either a
     *    descendant or dos path step.
     *  @retval bool True if the associated path step carries a descendant
     *    or dos axis, false otherwise.
     */
    bool isDosOrDescendantLabel();

    /*! @fn bool isNodeLabel()
     *  @brief Returns true if the associated path step is of the form axis::node().
     *  @details Returns true if the associated path step is of the form axis::node().
     *  @retval bool True if the associated path step is of the form
     *    axis::node() false otherwise.
     */
    bool isNodeLabel();

    /*! @fn bool isStarLabel()
     *  @brief Returns true if the associated path step is of the form axis::*.
     *  @details Returns true if the associated path step is of the form axis::*.
     *  @retval bool True if the associated path step is of the form
     *    axis::* false otherwise.
     */
    bool isStarLabel();

    /*! @fn bool hasSameLevelOrChildSuccessors()
     *  @brief Checks if the path step has a same-level or some child successors.
     *  @details This condition is true if the label has a successor label in
     *    the same path or some child path.
     *  @retval bool True if the path step has a same-level or some child
     *    child successors, false otherwise.
     */
    bool hasSameLevelOrChildSuccessors();

    /*! @fn bool atEndOfPath()
     *  @brief Returns true if the label describes the final step in a path.
     *  @details Always returns true for the empty path.
     *  @retval bool True if the label describes the final step in a path,
     *    false otherwise.
     */
    bool atEndOfPath();

    /*! @fn ProjectionTreeLabel* locateVarAncestorLabel(unsigned var)
     *  @brief Returns the ancestor label with the given variable.
     *  @details The caller must make sure that the passed variable var exists.
     *  @param[in] var The variable for which the ancestor label is searched.
     *  @retval ProjectionTreeLabel* The ancestor projection tree label.
     */
    ProjectionTreeLabel *locateVarAncestorLabel(unsigned var);

    /*! @fn bool matchesTag(TAG t)
     *  @brief Checks if the associated path step referenced by this
     *    label matches tag t.
     *  @details This method takes into consideration possible node() or * labels.
     *  @param[in] t The tag to be compared.
     *  @retval bool True if the tag matches, false otherwise.
     */
    bool matchesTag(TAG t);

    /*! @fn bool matchesText()
     *  @brief Checks if the associated path step matches text.
     *  @details This method returns true if the associated node test is of
     *     the form text() or node().
     *  @retval bool True if text is matched, false otherwise.
     */
    bool matchesText();

    /*! @fn bool descendantAxisBetw(unsigned basing_id, bool consider_final)
     *  @brief Checks for a descendant axis being used between the the current
     *    state and the state referred to by basing_id.
     *  @details Recursively climbs up to the label with basing_id.
     *  @param[in] basing_id The id of the label where to abort the check.
     *  @param[in] consider_final True if basing_id is to be considered.
     *  @retval bool Returns true if a descendant axis exists, false
     *   otherwise.
     */
    bool descendantAxisBetw(unsigned basing_id, bool consider_final = false);

  private:
    /*! @var ProjectionTreeLabel* parent
     *  @brief The parent label.
     *  @details May be NULL.
     */
    ProjectionTreeLabel * parent;

    /*! @var ProjectionTreeLabel* predecessor
     *  @brief The predecessor label.
     *  @details May be NULL.
     */
    ProjectionTreeLabel *predecessor;

    /*! @var ProjectionTreeNode* n
     *  @brief The associated node in the projection tree.
     *  @details The associated node in the projection tree.
     */
    ProjectionTreeNode *n;

    /*! @var unsigned position
     *  @brief Defines the position inside the path of the associated
     *    projection tree node (member variable n).
     *  @details If the path of the associated projection tree node is
     *    NULL, the variable is set to 0 and has no meaning.
     */
    unsigned position;

    /*! @var unsigned id
     *  @brief The id of the projection tree label
     *  @details When extracted regularly, this id is globally unique.
     */
    unsigned id;

    /*! @var ProjectionTreeLabel* same_level_successor.
     *  @brief The successor label in the path, if any.
     *  @details The successor label is defined as the ProjectionTreeLabel
     *    that describes the path step to the right (in the same node _n)
     *    of this label. May be NULL.
     */
    ProjectionTreeLabel *same_level_successor;

    /*! @var vector<ProjectionTreeLabel*> child_successors.
     *  @brief The set of successor child labels.
     *  @details More precisely, the set of ProjectionTreeLabels in the
     *    first position of paths contained in child nodes of _n,
     *    but only if this object's label is set to the last
     *    position of _n's path. Otherwise, the vector is empty.
     */
    vector < ProjectionTreeLabel * >child_successors;

    /*! @var vector<ProjectionTreeLabel*> self_successors
     *  @brief The self successor labels.
     *  @details More precisely, the set of labels associated to child projection
     *     tree nodes of _n that carry no path, i.e. the NULL path, and recursively
     *     their children if these satisfy the same condition.
     */
    vector < ProjectionTreeLabel * >self_successors;
};

#endif // PROJECTIONTREELABEL_H
