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
#include "projectiontreenode.h"
#include "varname.h"
#include "tagmap.h"

ProjectionTreeNode::ProjectionTreeNode() :
	parent(NULL), var(0), axis(e_child), tag(TAGID_ROOT),
	is_ec_node(false), type(e_aux), role(NULL) {
}

ProjectionTreeNode::ProjectionTreeNode(ProjectionTreeNode* _parent,
										int _var,
										AXIS_TYPE _axis,
										TAG _tag,
										bool _is_ec_node,
										NODE_TYPE _type,
										Role* _role) :
	parent(_parent), var(_var), axis(_axis), tag(_tag),
	is_ec_node(_is_ec_node), type(_type), role(_role) {
}

ProjectionTreeNode::~ProjectionTreeNode() {
	for (unsigned i=0; i<children.size(); i++) {
		delete children[i];
	}
	// roles are deleted via the RoleList datastructure
}

/*
 * creation of dos::* child node (more exactly self::* with 
 * descendant::* child)
*/
bool ProjectionTreeNode::createDOSStarChildNode(int _parent_var,
												Role* _role,
												NODE_TYPE _type) {

	if (_parent_var==var) {
		createDOSStarChild(_role,_type);
		return true;
	} else {

		bool created=false;
		for (unsigned i=0; i<children.size() && !created; i++) {
			created=children[i]->createDOSStarChildNode
											(_parent_var,_role,_type);
		}
		return created;
	}
}
											

bool ProjectionTreeNode::createChildNode(int _parent_var,
											int _introduced_var,
											AXIS_TYPE _axis,
											TAG _tag,
											bool _create_dos,
											bool _is_ec_node,
											NODE_TYPE _type,
											Role* _role) {
	
	// check for position being the right one	
	if (_parent_var==var) {
		
		if (!_create_dos) {	
			ProjectionTreeNode* child=new ProjectionTreeNode(this,
															_introduced_var,
															_axis,
															_tag,
															_is_ec_node,
															_type,
															_role);
			children.push_back(child);
		} else {
			ProjectionTreeNode* child=new ProjectionTreeNode(this,
															_introduced_var,
															_axis,
															_tag,
															_is_ec_node,
															_type,
															NULL);
			child->createDOSStarChild(_role,_type);
			children.push_back(child);
		}
		return true;
		
	// if position false, search all children 
	} else {
			
		bool created=false;
		for (unsigned i=0; i<children.size() && !created; i++) {
			created=children[i]->createChildNode(_parent_var,
												_introduced_var,
												_axis,
												_tag,
												_create_dos,
												_is_ec_node,
												_type,
												_role);
		}
		return created;
	}
}

void ProjectionTreeNode::createDOSStarChild(Role* _role,NODE_TYPE _type) {

	// we create a hierarchy "self::* - descendant::*" where only "self::*" carries a role
	// (this replaces the theoretical "dos::*"-construct, which makes handling of
	//  cumulative roles more complicated)
	  
	// this is the self::* node...
	ProjectionTreeNode* child=new ProjectionTreeNode(this,
														-1,
														e_self,
														TAGID_STAR,
														false,
														_type,
														_role);

	// ... which has a single, role/-less descendant::* child
	child->createDescendantStarChild(_type);
	children.push_back(child);
}

void ProjectionTreeNode::createDescendantStarChild(NODE_TYPE _type) {
	ProjectionTreeNode* child=new ProjectionTreeNode(this,
														-1,
														e_descendant,
														TAGID_STAR,
														false,
														_type,
														NULL);
	children.push_back(child);
}

void ProjectionTreeNode::print() {
    print(0);
}

void ProjectionTreeNode::print(unsigned tabs) {
	for (unsigned i=0; i<tabs; i++) {
		cout << "  ";
	}
	
	switch (axis) {
		case e_child:
			cout << "child::";
			break;
		case e_descendant:
			cout << "descendant::";
			break;
		case e_self:
			cout << "self::";
			break;
		default:
			cout << "UNDEFINED::";
	}
	cout << TagMap::getInstance()->getTag(tag);
	if (is_ec_node) {
		cout << "[position()=1]";
	}
	if (var>-1) {
		cout << " ($" << VarName::getInstance()->getVar(var) << ")";
	}
	if (role!=NULL) {
		cout << " (r" << role->getId() << ",$";
	   	cout << VarName::getInstance()->getVar(role->getBasingVar())
				<< ",$" 
				<< VarName::getInstance()->getVar(role->getBasingFSA())
		 		<< ")";
	}

	switch (type) {
		case (e_aux):
			cout << " TYPE: aux" << endl;
			break; 
		case (e_dom):
			cout << " TYPE: dom" << endl;
			break;
		#ifdef USE_CLOBS
		case (e_clob):
			cout << " TYPE: clob" << endl;
			break;
		#endif // ifdef USE_CLOBS
		case (e_out):
			cout << " TYPE: out" << endl;
			break;
	}

	for (unsigned i=0; i<children.size(); i++) {
		children[i]->print(tabs+1);
	}
}


void ProjectionTreeNode::appendRoles(vector<unsigned>& cumulative_indices,
										vector<unsigned>& noncumulative_indices) {
	if (role) {
		role->getType()==ROLETYPE_NONEXISTENCE?
							cumulative_indices.push_back(role->getId()):
							noncumulative_indices.push_back(role->getId());
	}
}
