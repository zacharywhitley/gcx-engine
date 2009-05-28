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
 * 	@brief Header file for projectiontreelabel.cpp.
 * 	@details Header file specifying constructors, destructor and functions for projectiontreelabel.cpp.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef PROJECTIONTREELABEL_H
#define PROJECTIONTREELABEL_H

#include "projectiontreenode.h"

/*! @class ProjectionTreeLabel
 * 	@brief Represents labels for positions in the projection trees.
 * 	@details More precisely, each path step expression in the projection
 * 	         tree is assigned a unique label. In addition, projection tree
 * 	         nodes with NULL path get assigned a label, too. For each label,
 * 	         we store different types of successor labels, such as vertical
 * 	         and horizontal successors. 
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class ProjectionTreeLabel {

public:
	/*! @brief Constructor.
     *  @details Constructor - Creates a new projection tree label.
     *  @param[in] _n The projection tree node the label is assigned to.
     *  @param[in] _position The position of the path step stored in node _n.
     *  @param[in] _id The id of the ProjectionTreeLabel.
     *  @param[in] _same_level_successor The ProjectionTreeLabel that 
     *                                   describes the path step to the
     *                                   right (in the same node _n) of this
     *                                   label. May be NULL. 
     *  @param[in] _child_successors The set of ProjectionTreeLabels in the
     *                               first position of paths contained in
     *                               child nodes of _n, but only if this label
     *                               is set to the last position of _n's path.
     *                               Otherwise, the vector is empty.
     *  @param[in] _self_successors The set of labels that describes child
     *                              nodes of _n that carry no path, i.e. the
     *                              NULL path, and recursively their children
     *                              if these satisfy the same condition.
     */
	ProjectionTreeLabel(ProjectionTreeNode* _n,
						unsigned _position, unsigned _id,
						ProjectionTreeLabel* _same_level_successor,
						vector<ProjectionTreeLabel*> _child_successors,
						vector<ProjectionTreeLabel*> _self_successors);
	
	/*! @brief Destructor.
     *  @details Destructor.
     */
	virtual ~ProjectionTreeLabel();

	/*! @brief Recursively brings the parent and predecessor links up to date.
     *  @details To be applied exactly once after all labels have been
     *           collected.
     *  @param[in] _parent Pointer to the parent projection tree label.
     *  @param[in] _predecessor Pointer to the predecessor label.
     *  @retval void
     */
	void updateParentPredecessorInformation(ProjectionTreeLabel* _parent,
											ProjectionTreeLabel* _predecessor);

	/*! @brief Returns the parent label.
     *  @details A projection tree label is called parent label if
     *           it refers the last past step in the parent
     *           projection tree node.
     *  @retval ProjectionTreeLabel* The parent label.
     */
	inline ProjectionTreeLabel* getParent() { return parent; } 

	/*! @brief Returns the predecessor label.
     *  @details A projection tree label is called predecessor
     *           if it is reached by going one path step back.
     *           The predecessor (in contrast to the parent)
     *           might also be in the same level. If it is not,
     *           it equals to the parent.
     *  @retval ProjectionTreeLabel* The predecessor label.
     */
	inline ProjectionTreeLabel* getPredecessor() { return predecessor; } 

	/*! @brief Returns the first label associated with the same path step.
     *  @details
     *  @retval ProjectionTreeLabel* The first label associated with this
     *                               path step.
     */
	ProjectionTreeLabel* getLeftmostSLPredecessor();
	
	/*! @brief Returns the id of the variable the label ultimately depends on.
     *  @details This is the variable that is associated with the
     *           associated projection tree node.
     *  @retval unsigned The variable id.
     */
	unsigned getDependingVar();

	/*! @brief Returns the ancestor label with the given variable.
     *  @details The caller must make sure that the passed variable var exists.
     *  @param[in] var The variable for which the ancestor label is searched.
     *  @retval ProjectionTreeLabel The ancestor projection tree label.
     */
	ProjectionTreeLabel* locateVarAncestorLabel(unsigned var);
	
	/*! @brief Return the path that is referenced by this label.
     *  @details The path may be NULL.
     *  @retval PathExpression* Pointer to the path expression.
     */
	PathExpression* getPath();

	/*! @brief Get the pathstep that is references by this label.
     *  @details The path step may be NULL (if and only if the path is NULL).
     *  @retval PathStepExpression Pointer to the path step expression.
     */
	PathStepExpression* getPathStep();

	/*! @brief Returns true if the label describes the final step in a path.
     *  @details
     *  @retval bool
     */
	bool atEndOfPath();

	/*! @brief Returns true if the associated path step referenced
     *         by this label matches tag t.
     *  @details
     *  @param[in] t The tag to be compared.
     *  @retval bool
     */
	bool matchesTag(TAG t);

	/*! @brief Returns true if the associated path step carries
     *         a child or descendant axis.
     *  @details
     *  @retval bool
     */
	bool isChildOrDescendantLabel();

	/*! @brief Returns true if the associated path step carries
     *         a descendant axis.
     *  @details
     *  @retval bool
     */
	bool isDescendantLabel();

	/*! @brief Returns true if the associated path step carries
     *         a child axis.
     *  @details
     *  @retval bool
     */
	bool isChildLabel();

	/*! @brief Returns true if the associated path step carries
     *         a descendant-or-self axis. 
     *  @details
     *  @retval bool
     */
	bool isDosLabel();

	/*! @brief Returns true if the associated path step carries
     *         a descendant-or-self::node label. 
     *  @details
     *  @retval bool
     */
	bool isDosNodeLabel();

	/*! @brief Returns true if the associated path step carries either
     *         a descendant or descendant-or-self axis. 
     *  @details
     *  @retval bool
     */
	bool isDosOrDescendantLabel();

	/*! @brief Returns true if the associated path step is of
     *         the form axis::node().
     *  @details
     *  @retval bool
     */
	bool isNodeLabel();

	/*! @brief Returns true if the associated path step is
     *         of the form axis::*.
     *  @details
     *  @retval bool
     */
	bool isStarLabel();

	/*! @brief Returns true if the associated path step is 
     *         of the form axis::text().
     *  @details
     *  @retval bool
     */
	bool matchesText();

	/*! @brief Returns true if the path step is of the form axis::text().
     *  @details
     *  @retval bool
     */
	bool hasSameLevelOrChildSuccessors();

	/*! @brief Returns the tag carried by the associated path
     *         step expression.
     *  @details The caller must ensure that the associated path
     *           step expression differs from NULL.
     *  @retval TAG The tag id of the associated path step expression.
     */
	TAG getTag();

	/*! @brief Returns the axis type that, going up from this label,
     *         immediately follows its first straight ancestor (FSA) label.
     *  @retval AXIS_TYPE
     */
	AXIS_TYPE getAxisFollowingFSA();

	/*! @brief Returns the axis type that, going up from this label,
     *         immediately follows the given first straight ancestor (FSA) id.
     *  @param[in] unsigned fsa The first straight ancestor (FSA) is.
     *  @retval AXIS_TYPE
     */
	AXIS_TYPE getAxisFollowingFSA(unsigned fsa);

	/*! @brief Returns the first straight ancestor (FSA) label.
     *  @details The first straight ancestor (FSA) label is the label of the last path
     *           step of the path that is bound to the first straight ancestor (FSA) variable
     *  @retval ProjectionTreeLabel*
     */
	ProjectionTreeLabel* getFSALabel();

	/*! @brief Returns the id of the label.
     *  @details
     *  @retval unsigned The id of the label.
     */
	inline unsigned getId() { return id; }

	/*! @brief Returns the path step position inside the path. 
     *  @details
     *  @retval unsigned The path step position inside the path.
     */
	inline unsigned getPosition() { return position; }

	/*! @brief Returns the same level successor.
     *  @details May return NULL if no same level successor exists.
     *  @retval ProjectionTreeLabel* Pointer to the same level successor.
     */
	inline ProjectionTreeLabel* getSameLevelSuccessor() { return same_level_successor; }

	/*! @brief Returns the child successors. 
     *  @details 
     *  @retval vector<ProjectionTreeLabel*> Pointer to the vector
     *         containing the child successors.
     */
	inline vector<ProjectionTreeLabel*>* getChildSuccessors() { return &child_successors; }

	/*! @brief Returns the self successors. 
     *  @details 
     *  @retval vector<ProjectionTreeLabel*> Pointer to the vector
     *         containing the self successors.
     */
	inline vector<ProjectionTreeLabel*>* getSelfSuccessors() { return &self_successors; }

	/*! @brief Returns the associated projection tree node.
     *  @details Will always be different from NULL.
     *  @retval ProjectionTreeNode* The associated projection tree node.
     */
	inline ProjectionTreeNode* getProjectionTreeNode() { return n; }

	/*! @brief Debug prints the projection tree label to STDOUT.
     *  @details
     *  @retval void
     */
	void print();
	
	/*! @brief Checks for a descendant axis used between the
     *         the current state and the state referred to by basing_id.
     *  @details
     *  @retval bool Returns true if a descendant axis exists, false
     *          otherwise.
     */
	bool descendantAxisBetw(unsigned basing_id,bool consider_final=false);

private:
	/*! @var ProjectionTreeLabel* parent
     * @brief The parent label.
     * @details May be NULL.
     */
	ProjectionTreeLabel* parent;

	/*! @var ProjectionTreeLabel* predecessor
     * @brief The predecessor label.
     * @details May be NULL.
     */
	ProjectionTreeLabel* predecessor;

	/*! @var ProjectionTreeNode* n
     * @brief The associated node in the projection tree.
     * @details
     */
	ProjectionTreeNode* n;

	/*! @var unsigned position
     * @brief Defines the position inside the path of the associated
     *        projection tree node (member variable n).
     * @details If the path of the associated projection tree node is
     *          NULL, the variable is set to 0 and has no meaning.
     */
	unsigned position;

	/*! @var unsigned id
     * @brief The id of the projection tree label
     * @details When extracted regularly, this id is globally unique.
     */
	unsigned id;

	/*! @var ProjectionTreeLabel* same_level_successor.
     *  @brief The successor label in the path, if any.
     *  @detail The successor label is defined as the ProjectionTreeLabel
     *          that describes the path step to the right (in the same node _n)
     *          of this label. May be NULL.
     */
	ProjectionTreeLabel* same_level_successor;

	/*! @var vector<ProjectionTreeLabel*> child_successors.
	 *  @brief The set of successor child labels.
     *  @details More precisely, the set of ProjectionTreeLabels in the
     *           first position of paths contained in child nodes of _n,
     *           but only if this object's label is set to the last
     *           position of _n's path. Otherwise, the vector is empty.
     */
	vector<ProjectionTreeLabel*> child_successors;

	/*! @var vector<ProjectionTreeLabel*> self_successors
     *  @brief The self successor labels.
     *  @details More precisely, the set of labels associated to
     *           child projection tree nodes of _n that carry no path,
     *           i.e. the NULL path, and recursively their children
     *           if these satisfy the same condition.
     */
	vector<ProjectionTreeLabel*> self_successors;
};

#endif // PROJECTIONTREELABEL_H
