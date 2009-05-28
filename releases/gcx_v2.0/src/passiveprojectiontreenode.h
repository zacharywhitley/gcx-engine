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
 *  @brief Header file for passiveprojectiontreenode.cpp.
 *  @details Header file specifying constructors, destructor and functions
 *           for passiveprojectiontreenode.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
#ifndef PASSIVEPROJECTIONTREENODE_H
#define PASSIVEPROJECTIONTREENODE_H

#include "pathexpression.h"

//! @namespace std
using namespace std;

/*! @class PassiveProjectionTreeNode
 *  @brief A node of the passive projection tree.
 *  @details Each node in the passive projection tree describes exactly one
 *           path step (i.e., is labeled with a path step or NULL, for the root).
 *  @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */

class PassiveProjectionTreeNode {

public:
    /*! @fn PassiveProjectionTreeNode()
     *  @brief Constructor.
     *  @details Constructs the the passive projection tree root node.
     */
    PassiveProjectionTreeNode();

    /*! @fn PassiveProjectionTreeNode(PassiveProjectionTreeNode* _parent,PathStepExpression *_label) {
     *  @brief Constructor.
     *  @details Constructs a labeled passive projection tree node.
     *  @param[in] PassiveProjectionTreeNode* _parent The designated parent of this node. 
     *  @param[in] PathStepExpression* _label The designated label of this node. 
     */
    PassiveProjectionTreeNode(PassiveProjectionTreeNode* _parent, PathStepExpression* _label);

    /*! @fn ~PassiveProjectionTreeNode()
     *  @brief Destructor.
     *  @details Deletes the passive projection tree node and all its children.
     */
    virtual ~PassiveProjectionTreeNode();

    /*! @fn PassiveProjectionTreeNode* getParent()
     *  @brief Returns the parent of this passive projection tree node.
     *  @details Returns NULL for the root node.
     */
    inline PassiveProjectionTreeNode* getParent() { return parent; } 

    /*! @fn vector<PassiveProjectionTreeNode*>* getChildren()
     *  @brief Returns the child vector of this passive projection tree node.
     *  @retval vector<PassiveProjectionTreeNode*>*
     */
    inline vector<PassiveProjectionTreeNode*>* getChildren() { return &children; } 

    /*! @fn PathStepExpression* getLabel()
     *  @brief Returns the label of the passive projection tree node.
     *  @details Note that the label of the root node is NULL.
     */
    inline PathStepExpression* getLabel() { return label; }

    /*! @fn void registerPath(PathExpression* path, unsigned pos)
     *  @brief registers a path to the passive projection tree node.
     *  @param[in] PathExpression* path The path to be registered.
     *  @param[in] unsigned pos The current position within the path
     */
    void registerPath(PathExpression* path, unsigned pos);
    
    /*! @fn void print()
     *  @brief Prints the path expression node to STDOUT.
     */
    void print();


protected:
    /*! @fn void print()
     *  @brief Prints the path expression node to STDOUT with fixed indent.
     *  @details Help function of public print() method..
     */
    void print(unsigned tabs);
    
private:
    /*! @var PassiveProjectionTreeNode* parent
     *  @brief Pointer to the parent of this passive projection tree node.
     *  @details Is NULL for the root node.
     */
    PassiveProjectionTreeNode* parent;

    /*! @var PathStepExpression* ps
     *  @brief The path step expression associated with this node.
     *  @details Is NULL for the root node.
     */
    PathStepExpression* label;

    /*! @var vector<PassiveProjectionTreeNode*> children
     *  @brief The children of this passive projection tree node.
     */
    vector<PassiveProjectionTreeNode*> children;
};

#endif // PASSIVEPROJECTIONTREENODE_H
