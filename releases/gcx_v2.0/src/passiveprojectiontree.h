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
 *  @brief Header file for passiveprojectiontree.cpp.
 *  @details Header file specifying constructors, destructor and functions for passiveprojectiontree.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
#ifndef PASSIVEPROJECTIONTREE_H
#define PASSIVEPROJECTIONTREE_H

#include "passiveprojectiontreenode.h"
#include "pathenvironment.h"

/*! @class PassiveProjectionTree
 *  @brief Class for the passive projection tree.
 *  @details The passive projection tree is kept up to avoid the interleaving of path
 *            steps that have been optimized away. Basically, each node represents a
 *            single path step, and each path from the root describes a complete path.
 *            Nodes matched on this path during preprojection must not be interleaved.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class PassiveProjectionTree {

public:
    /*! @fn PassiveProjectionTreeNode() 
     *  @brief Constructor.
     *  @details Constructs an empty passive projection tree (virtual root node only).
     */
    PassiveProjectionTree(PathEnvironment* _penv);

    /*! @fn ~PassiveProjectionTreeNode()
     *  @brief Destructor.
     *  @brief Deletes the passive projection tree.
     */ 
    virtual ~PassiveProjectionTree();    

    /*! @fn PassiveProjectionTreeNode* getRootNode()
     *  @brief Returns the root node of the passive projection tree.
     */
    inline PassiveProjectionTreeNode* getRootNode() { return root; }

    /*! @fn PathEnvironment* getPathEnv()
     *  @brief Returns the path environment.
     */
    inline PathEnvironment* getPathEnv() { return penv; }

    /*! @fn void registerPath(PathExpressions* path)
     *  @brief Adds a path to the passive projection tree.
     *  @param[in] PathExpression* path The path to be registered.
     *  @details The nodes of the path are inserted at the correct positions.
     */
    void registerPath(PathExpression* path);
    
    /*! @fn void print()
     *  @brief Prints ths passive projection tree node to STDOUT.
     *  @details This methid is for debug purposes only.
     */
    void print();

private:
    
    /*! @var PassiveProjectionTreeNode* root
     *  @brief The root node of the tree.
     */
    PassiveProjectionTreeNode* root;

    /*! @var PathEnvironment* penv
     *  @brief The path environment mapping variables to paths.
     */
    PathEnvironment* penv;
};

#endif // PASSIVEPROJECTIONTREE_H
