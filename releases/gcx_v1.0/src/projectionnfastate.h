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
#ifndef PROJECTIONNFASTATE_H
#define PROJECTIONNFASTATE_H

#include "projectionnfatransition.h"
#include "projectiontreenode.h"
#include <iostream>
#include <vector>

using namespace std;

class ProjectionNFAState {

public:
    ProjectionNFAState(ProjectionTreeNode* _ptn,
						bool _is_positive,
						NODE_TYPE _type);
    virtual ~ProjectionNFAState();

	void print();
	void printSingleState();
	void printDebugInfo();
	
	void addTransition(TAG tag,bool positive,ProjectionNFAState* to);
	void addStatesMatchingTag(TAG _tag,vector<ProjectionNFAState*>& s);
	void addEpsilonTransitionStates(vector<ProjectionNFAState*>& s);
	void appendRoles(vector<unsigned>& cumulative_indices,
						vector<unsigned>& noncumulative);

	inline bool isPositive() { return is_positive; }	
	inline NODE_TYPE getType() { return type; }
	//inline unsigned getStringFwState() { return string_fw_state; }
	inline bool isSelfPointing();
	inline ProjectionTreeNode* getPtn() { return ptn; }
	
	void transitiveClosure(vector<ProjectionNFAState*>& tc);
	
protected:
	void print(vector<ProjectionNFAState*>& printed_states);
	void setUpTransitions();

private:
	bool is_positive; // negated or non-negated state
	ProjectionTreeNode* ptn; // the projection tree node the state points to 

	vector<ProjectionNFAState*> epsilon_transitions; // epsilon transitions
	vector<ProjectionNFATransition> transitions; // non-epsilon transitions

	NODE_TYPE type;
};

#endif // PROJECTIONNFASTATE_H
