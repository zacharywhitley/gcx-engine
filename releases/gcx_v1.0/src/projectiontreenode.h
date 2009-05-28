/*
 | Author: Michael Schmidt
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
 |  following disclaimer.
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
#ifndef PROJECTIONTREENODE_H
#define PROJECTIONTREENODE_H

#include "rolelist.h"
#include "role.h"
#include "tagmap.h"
#include "expenums.h"
#include <iostream>
#include <vector>

using namespace std;

class ProjectionTreeNode {

public:
    ProjectionTreeNode();
	ProjectionTreeNode(ProjectionTreeNode* _parent,
						int _var,
						AXIS_TYPE _axis,
						TAG _tag,
						bool _is_ec_node,
						NODE_TYPE _type,
						Role* _role);
    virtual ~ProjectionTreeNode();

	inline AXIS_TYPE getAxisType() { return axis; }
	inline TAG getNodeTest() { return tag; }
	inline vector<ProjectionTreeNode*>* getChildren() { return &children; }
	inline bool isEcNode() { return is_ec_node; }
	inline NODE_TYPE getType() { return type; }
	
	bool createDOSStarChildNode(int _parent_var,Role* role,NODE_TYPE _type);
	bool createChildNode(int _parent_var,
							int _introduced_var,
							AXIS_TYPE _axis,
							TAG _tag,
							bool _create_dos,
							bool _set_ec_flag,
							NODE_TYPE _type,
							Role* _role);

	void appendRoles(vector<unsigned>& cumulative_indices,
						vector<unsigned>& noncumulative_indices);
	
	void print();

protected:
	void createDOSStarChild(Role* _role,NODE_TYPE _type);
	void createDescendantStarChild(NODE_TYPE _type);
	void print(unsigned tabs);
	
private:

	ProjectionTreeNode* parent;
	int var; // the variable the node represents (-1 if none)

	AXIS_TYPE axis; // the axis the node matches
	TAG tag; // the tag id
	bool is_ec_node; // [position()=1] indicator
	NODE_TYPE type;
	Role* role; // the role (may be NULL)
	
	vector<ProjectionTreeNode*> children;
};

#endif // PROJECTIONTREENODE_H
