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
 * 	@brief Header file for bufferiterator.cpp.
 * 	@details Header file specifying constructors, destructor and functions for bufferiterator.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef BUFFERITERATOR_H
#define BUFFERITERATOR_H

#include <vector>
#include <stack>
#include "buffernode.h"
#include "streampreprocessor.h"
#include "tagnode.h"
#include "pathstepattributeposition.h"

/*! @def READ_UP_TO_CLOSE_BASE
 *  @brief Predefined variable for use as <tt>first</tt> argument of method getNext().
 *  @details Predefined variable for use as <tt>first</tt> argument of method getNext(), which
 * 			forces the StreamPreProcessor to read the input stream up to the closing tag of the
 * 			BufferNode to which the VarExpression is currently bound.
 *  @note Ensure that method getNext() is invoked in <tt>correct</tt> argument order!
 */
#define READ_UP_TO_CLOSE_BASE		0

/*! @def READ_UP_TO_CLOSE_CONTEXT
 *  @brief Predefined variable for use as <tt>first</tt> argument of method getNext().
 *  @details Predefined variable for use as <tt>first</tt> argument of method getNext(), which
 * 			forces the StreamPreProcessor to read the input stream up to the closing tag of the
 * 			context	node, which is the BufferNode that will be returned after each invocation of
 * 			method getNext().
 *  @note Ensure that method getNext() is invoked in <tt>correct</tt> argument order!
 */
#define READ_UP_TO_CLOSE_CONTEXT	1

/*! @def READ_UP_TO_CLOSE_NONE
 *  @brief Predefined variable for use as <tt>first</tt> argument of method getNext().
 *  @details Predefined variable for use as <tt>first</tt> argument of method getNext(), which
 * 			forces the StreamPreProcessor to read the input stream only up to the node which is
 * 			needed/returned	and not more. This is the default behaviour and assumed if method
 * 			getNext() is invoked without any arguments.
 *  @note Ensure that method getNext() is invoked in <tt>correct</tt> argument order!
 */
#define READ_UP_TO_CLOSE_NONE		2

/*! @def LOCK_CONTEXT_NO_CLEAR
 *  @brief Predefined variable for use as <tt>second</tt> argument of method getNext().
 *  @details Predefined variable for use as <tt>second</tt> argument of method getNext(), which
 * 			locks the current returned node of method getNext() and does not invoke method clear()
 * 			on returned BufferNode after invocation of method unlock().
 *  @note Ensure that method getNext() is invoked in <tt>correct</tt> argument order!
 */
#define LOCK_CONTEXT_NO_CLEAR		10

/*! @def LOCK_CONTEXT_LAST_CLEAR
 *  @brief Predefined variable for use as <tt>second</tt> argument of method getNext().
 *  @details Predefined variable for use as <tt>second</tt> argument of method getNext(), which
 * 			locks the current returned node of method getNext() and invokes method clear() only on
 * 			the last returned BufferNode after invocation of method unlock(), i.e. invokes method
 * 			clear() after method getNext() returns <tt>NULL<tt>.
 *  @note Ensure that method getNext() is invoked in <tt>correct</tt> argument order!
 */
#define LOCK_CONTEXT_LAST_CLEAR		11

/*! @def LOCK_CONTEXT_ALWAYS_CLEAR
 *  @brief Predefined variable for use as <tt>second</tt> argument of method getNext().
 *  @details Predefined variable for use as <tt>second</tt> argument of method getNext(), which
 * 			locks the current returned node of method getNext() and invokes method clear() on each
 * 			returned BufferNode after invocation of method unlock().
 *  @note Ensure that method getNext() is invoked in <tt>correct</tt> argument order!
 */
#define LOCK_CONTEXT_ALWAYS_CLEAR	12

/*! @def LOCK_NONE_NO_CLEAR
 *  @brief Predefined variable for use as <tt>second</tt> argument of method getNext().
 *  @details Predefined variable for use as <tt>second</tt> argument of method getNext(), which
 * 			does not lock the current returned node of method getNext() and does not invoke method
 * 			clear() on each returned BufferNode after invocation of method unlock(). This is the
 * 			default behaviour and assumed if method getNext() is invoked without any arguments.
 *  @note Ensure that method getNext() is invoked in <tt>correct</tt> argument order!
 */
#define LOCK_NONE_NO_CLEAR			13

/*! @def BIT_DEBUG_MODE_FULL
 *  @brief Predefined variable for use as argument of method debugPrint().
 *  @details Predefined variable for use as argument of method debugPrint(), which indicates to
 * 			output all extracted informations of the entered PathExpression and stored nodes,
 * 			whereas BufferNode will be outputted in debug mode themselves.
 */
#define BIT_DEBUG_MODE_FULL 	0

/*! @def BIT_DEBUG_MODE_PARTIAL
 *  @brief Predefined variable for use as argument of method debugPrint().
 *  @details Predefined variable for use as argument of method debugPrint(), which indicates to
 * 			output all extracted informations of the entered PathExpression and stored nodes,
 * 			whereas BufferNode will be outputted in normal mode themselves.
 */
#define BIT_DEBUG_MODE_PARTIAL 	1

/*! @def BIT_DEBUG_MODE_SIMPLE
 *  @brief Predefined variable for use as argument of method debugPrint().
 *  @details Predefined variable for use as argument of method debugPrint(), which indicates to
 * 			output only the entered PathExpression and stored nodes, whereas BufferNode will be
 * 			outputted in normal mode themselves. This is the default behaviour and assumed if
 * 			method debugPrint() is invoked without any arguments.
 */
#define BIT_DEBUG_MODE_SIMPLE 	2

/*! @class BufferIterator
 * 	@brief Represents iterator over nodes stored in buffer for a given path.
 * 	@details Represents iterator over nodes stored in buffer for a given path, which takes therefore a
 * 			BufferNode to which a VarExpression (variable), e.g. <tt>\$x</tt>, <tt>\$z</tt> or <tt>\$root</tt>,
 * 			is currently bound and a PathExpression, e.g. <tt>/bib//book</tt> or <tt>/bib/book/title</tt>,
 * 			as arguments.
 *  @note Support for axis dos and attribute <tt>[position</tt><tt>()=n]</tt> for any pathstep and
 * 			in any combination has been implemented and should work (maybe) BUT these two cases have
 * 			never been tested and therefore there is no warranty that they are really working.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class BufferIterator {

public:
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a BufferNode and a PathExpression.
	 * 	@param[in] _base Pointer to a BufferNode object.
	 *  @param[in] _path Pointer to a PathExpression object.
	 */
    BufferIterator(BufferNode* _base, PathExpression* _path);
    
	/*! @brief Destructor.
	 * 	@details Destructor.
	 */
    virtual ~BufferIterator();

    /*! @fn BufferNode* getNext(unsigned read_up_to_closing=READ_UP_TO_CLOSE_NONE, unsigned lock_node=LOCK_NONE_NO_CLEAR)
     * 	@brief Returns the next context node for the entered path.
     * 	@details Returns the next context node for the entered path, which is located
     * 			from the entered base BufferNode after execution of the PathExpression.
     * 			Needed or not up to now available BufferNode in the Buffer are requested from
     * 			the	StreamPreProcessor.
     *  @param[in] read_up_to_closing Forces the StreamPreProcessor to read the input stream up to
     * 			the	closing tag of a specific BufferNode (see also predefined variables
     * 			READ_UP_TO_CLOSE_BASE, READ_UP_TO_CLOSE_CONTEXT, READ_UP_TO_CLOSE_NONE).
     *  @param[in] lock_node Locks, unlocks and invokes garbage collection on a BufferNode by
     * 			invoking method lock(), unlock() and clear() on a BufferNode (see also predefined
     * 			variables LOCK_CONTEXT_NO_CLEAR, LOCK_CONTEXT_LAST_CLEAR, LOCK_CONTEXT_ALWAYS_CLEAR,
     * 			LOCK_NONE_NO_CLEAR). 
     * 	@retval BufferNode Pointer to a BufferNode object.
     */
    BufferNode* getNext(unsigned read_up_to_closing=READ_UP_TO_CLOSE_NONE,
    						unsigned lock_node=LOCK_NONE_NO_CLEAR);
    
    /*! @fn void reset()
     * 	@brief Resets this BufferIterator object.
     * 	@details Resets this BufferIterator object, i.e. resets most of the member variables to avoid
     * 			time-consuming setting of member variables by creating a new object. This method is
     * 			needed because of the nested loop joins in method eval() of CondOperandExpression.
     * 	@retval void
     */
    void reset();
    
    /*! @fn void debugPrint(unsigned debug_mode=BIT_DEBUG_MODE_SIMPLE)
     * 	@brief Prints all stored information.
     * 	@details Prints all stored information, which are the extracted informations of the entered
     * 			PathExpression and stored nodes.
     *  @param[in] debug_mode Indicates the grade of information output (see also predefined variables
     * 			BIT_DEBUG_MODE_FULL, BIT_DEBUG_MODE_PARTIAL, BIT_DEBUG_MODE_SIMPLE).
     * 	@retval void
     */
	void debugPrint(unsigned debug_mode=BIT_DEBUG_MODE_SIMPLE);

private:
    /*! @fn BufferNode* isSatisfyingPath(BufferNode* nnode)
     * 	@brief Checks if a node, which is the argument of this method, is satisyfying the entered path.
     * 	@details Checks if a node, which is the argument of this method, is satisyfying the entered path.
     *  @param[in] nnode Pointer to a BufferNode object, which is checked if it satisfy the entered
     * 			PathExpression.
     * 	@retval BufferNode Pointer to a BufferNode object, which is the next node (if a subtree could
     * 			be skipped) beeing processed by the left-to-right depth-first traversal or NULL if
     * 			the	entered	PathExpression is fully satisfied.
     */
	BufferNode* isSatisfyingPath(BufferNode* nnode);
	
    /*! @fn bool isMatchingNodeTestAndPredicates(BufferNode* nnode, unsigned ps_idx)
     * 	@brief Checks if a node, which is the first argument of this method, matches the given
     * 			pathstep index, which is the second argument of this method.
     * 	@details Checks if a node, which is the first argument of this method, matches the given
     * 			pathstep index, which is the second argument of this method.
     *  @param[in] nnode The BufferNode, which should to be checked for matching the pathstep index.
     *  @param[in] ps_idx The pathstep index, which should be checked for matching the BufferNode.
     * 	@retval bool <tt>true</tt> if the node matches the given pathstep (index), <tt>false</tt> otherwise.
     */
	bool isMatchingNodeTestAndPredicates(BufferNode* nnode, unsigned ps_idx);
	
    /*! @fn BufferNode* getPrevPSNode()
     * 	@brief Returns the assigned node of the previous processed pathstep.
     * 	@details Returns the assigned node of the previous processed pathstep.
     * 	@retval BufferNode Pointer to a BufferNode object.
     */
	BufferNode* getPrevPSNode();
	
    /*! @fn void unsignPSNode(BufferNode* nnode)
     * 	@brief Invoked method on upward traversal of the XML-tree.
     * 	@details Invoked method on upward traversal of the XML-tree, which unsigns/removes stored
     * 			nodes and decrement the current processed pathstep index (member variable ps_index).
     *  @param[in] nnode The BufferNode, which is processed while upward traversal of the XML-tree.
     * 	@retval void
     */
	void unsignPSNode(BufferNode* nnode);
	
	/*! @var BufferNode* base
     *  @brief The entered BufferNode.
     *  @details The entered BufferNode, which is the first argument of the constructor and is the
     * 			BufferNode of the current binding of the VarExpression of the entered PathExpression. 
     */
    BufferNode* base;
    
	/*! @var BufferNode* cur
     *  @brief The stored returned node after each invocation of method getNext().
     *  @details The stored returned node after each invocation of method getNext(), which is
     * 			the (current) context node of the entered PathExpression. 
     */
    BufferNode* cur;
    
	/*! @var BufferNode* cur_old
     *  @brief The stored previous returned node after each invocation of method getNext().
     *  @details The stored previous returned node after each invocation of method getNext(), which
     * 			was the (previous returned) context node of the entered PathExpression. 
     */
    BufferNode* cur_old;
    
	/*! @var bool cur_locked
     *  @brief The indicator if a returned BufferNode was locked.
     *  @details The indicator if a returned BufferNode was locked, which is the case, i.e. this
     * 			member variable is <tt>true</tt>, if it was locked from this object and this member
     * 			variable is <tt>false</tt>, if it was locked by another (BufferIterator) object. 
     */
    bool cur_locked;
    
	/*! @var PathExpression* path
     *  @brief The entered PathExpression.
     *  @details The entered PathExpression, which is the second argument of the constructor and
     * 			determines iteration over the Buffer, beginning from the BufferNode of the
     * 			VarExpression.
     */
    PathExpression* path;
    
	/*! @var StreamPreProcessor* spp
     *  @brief The StreamPreProcessor object to read the input stream if new nodes are required.
     *  @details The StreamPreProcessor object to read the input stream if new nodes are required.
     */
    StreamPreProcessor* spp;
    
	/*! @var vector<BufferNode*> ps_context
     *  @brief The stored BufferNode for each pathstep of the entered PathExpression.
     *  @details The stored BufferNode for each pathstep of the entered PathExpression.
     */
    vector<BufferNode*> ps_context;
    
	/*! @var vector<unsigned> ps_context_position
     *  @brief The stored number of assignments, which has been made up to now, of BufferNode for
     * 			each pathstep of the entered PathExpression.
     *  @details The stored number of assignments, which has been made up to now, to BufferNode for
     * 			each pathstep of the entered PathExpression.
     */
    vector<unsigned> ps_context_position;
    
	/*! @var vector<bool> ps_skip_subtree
     *  @brief The indicator for each pathstep of the entered PathExpression if subtrees could be
     * 			skipped.
     *  @details The indicator for each pathstep of the entered PathExpression if subtrees could be
     * 			skipped. If the entry for a pathstep is <tt>true</tt> it is possible to skip/jump
     * 			immediately to the next right sibling and avoid processing whole subtrees,
     * 			<tt>false</tt> otherwise, will process subtrees searching for a matching node for
     * 			the	current	processed pathstep.
     */
    vector<bool> ps_skip_subtree;
    
	/*! @var vector<bool> ps_attribute
     *  @brief The indicator for each pathstep of the entered PathExpression if a pathstep has
     * 			a PathStepAttribute.
     *  @details The indicator for each pathstep of the entered PathExpression if a pathstep has
     * 			a PathStepAttribute.
     */
    vector<bool> ps_attribute;
    
	/*! @var vector<unsigned> ps_attribute_position
     *  @brief The stored number of the PathStepAttributePosition for each pathstep of the entered
     * 			PathExpression, which needs to be satisfied for matching a pathstep.
     *  @details The stored number of the PathStepAttributePosition for each pathstep of the entered
     * 			PathExpression, which needs to be satisfied for matching a pathstep.
     */
    vector<unsigned> ps_attribute_position;
    
	/*! @var vector<bool> ps_back_track
     *  @brief The indicator for each pathstep of the entered PathExpression if it is needed to
     * 			backtrack one or more pathsteps to find all matching nodes.
     *  @details The indicator for each pathstep of the entered PathExpression if it is needed to
     * 			backtrack one or more pathsteps to find all matching nodes.
     */
    vector<bool> ps_back_track;
    
	/*! @var unsigned ps_index
     *  @brief The current processed pathstep (index).
     *  @details The current processed pathstep (index), which is on a find of a matching node for a
     * 			pathstep incremented and on upward traversal of the XML-tree decremented by method
     * 			unsignPSNode().
     */
    unsigned ps_index;
};

#endif // BUFFERITERATOR_H
