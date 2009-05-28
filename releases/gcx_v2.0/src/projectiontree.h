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
 * 	@brief Header file for projectiontree.cpp.
 * 	@details Header file specifying constructors, destructor and functions for projectiontree.cpp.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef PROJECTIONTREE_H
#define PROJECTIONTREE_H

#include "projectiontreenode.h"
#include "variabletree.h"
#include "dependencyset.h"
#include "projectiontreelabels.h"

/*! @class ProjectionTree
 * 	@brief
 * 	@details
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class ProjectionTree {

public:
    ProjectionTree(VariableTree* _vt, vector<DependencySet*>* _depset);
    virtual ~ProjectionTree();    

    inline ProjectionTreeNode* getRootNode() { return root; }

    void insertNode(int parent_var, int introduced_var,
                    PathExpression * path, bool dep, Role* role,
                    bool direct_output);
    
    // Here is the description of the additional parameter:
    // @param[in] PassiveProjectionTree* ppt The projection tree to register dropped nodes.
    void removeUnneededNodes(PassiveProjectionTree* ppt);
    
    // Here is the description of the additional parameter:
    // @param[in] PassiveProjectionTree* ppt The projection tree to register dropped nodes.
    void removeRedundantRoles(FSAMap* fsamap,PassiveProjectionTree* ppt);

	inline ProjectionTreeLabels* getLabels() { return labels; }

	//! extract the labels (apply after optimization)
	void labelTree();

    void print();

private:
	void createProjectionTree(VariableTreeNode* varnode, vector<DependencySet*>* depset);
	
	//! the root node of the tree
    ProjectionTreeNode* root;

	//! contains all necessary role information
    RoleList* roleinfo;

	//! all labels of the tree
	ProjectionTreeLabels* labels;
};

#endif // PROJECTIONTREE_H
