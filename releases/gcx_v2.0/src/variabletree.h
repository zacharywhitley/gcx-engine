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
 *     @brief Header file for variabletree.cpp.
 *     @details Header file specifying constructors, destructor and functions for variabletree.cpp.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#ifndef VARIABLETREE_H
#define VARIABLETREE_H

#include "variabletreenode.h"
#include "parvarmap.h"

/*! @class VariableTree
 *     @brief Represents the constructed variable tree.
 *     @details Represents the constructed variable tree, which is constructed through ParVarMap and is needed for
 *             constructing the ProjectionTree.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
class VariableTree {

public:
    /*! @brief Constructor.
     *     @details Constructor - creating object for variable <tt>\$root</tt>.
     */
    VariableTree();
    
    /*! @brief Destructor.
     *     @details Destructor.
     */
    virtual ~VariableTree();

    /*! @fn void insertNode(unsigned parent_var, unsigned introduced_var)
     *  @brief Inserts a new node into the variable tree.
     *  @details Inserts a new node into the variable tree, which is appended as child of the first argument of this method.
     *  @param[in] parent_var Parent node of the new node.
     *  @param[in] introduced_var New node or variable respectively to be inserted.
     *  @retval void
     */
    void insertNode(unsigned parent_var, unsigned introduced_var, bool direct_output);
    
    /*! @fn void print()
     *  @brief Prints the complete variable tree.
     *  @details Prints the complete variable tree. 
     *  @retval void
     */
    void print();
    
    /*! @fn inline VariableTreeNode* getRootNode()
     *  @brief Returns the root node of the variable tree.
     *  @details Returns the root node of the variable tree, i.e. returns the member variable,
     *             which is always variable <tt>\$root</tt>.
     *  @retval VariableTreeNode* Pointer to a VariableTreeNode object.
     */
    inline VariableTreeNode* getRootNode() { return root; }

private:
    /*! @var VariableTreeNode* root
     *     @brief The root node of the variable tree.
     *     @details The root node of the variable tree, which is always variable <tt>\$root</tt>.
     */
    VariableTreeNode* root;
};

#endif // VARIABLETREE_H
