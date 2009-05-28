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
#ifndef PROJECTIONDFASTATE_H
#define PROJECTIONDFASTATE_H

#include "projectiondfatransition.h"
#include "projectionnfastate.h"
#include <iostream>
#include <vector>

using namespace std;

class ProjectionDFAState {

public:
    ProjectionDFAState(ProjectionNFAState* p_nfa_state);
    ProjectionDFAState(ProjectionDFAState* _parent,
						vector<ProjectionNFAState*> p_nfa_states);
    virtual ~ProjectionDFAState();

	inline void increaseDepth() { depthctr++; }
	inline unsigned getDepth() { return depthctr; }
	inline bool isInitialState() { return parent==NULL; }
	inline vector<unsigned>* getCumulativeIndices() 
							{ return &cumulative_indices; }
	inline vector<unsigned>* getNoncumulativeIndices() 
							{ return &noncumulative_indices; }
	inline vector<ProjectionNFAState*> getOriginStates()
							{ return origin_states; }
	
	void addTransition(TAG tag,ProjectionDFAState* state);
	ProjectionDFAState* takeTransition(TAG _tag);
	ProjectionDFAState* traceBack();

	inline bool isEcOptimizable() { return is_ec_opt; }
	inline bool ecNodeOutput() { return ec_node_output; }
	void registerEcState(ProjectionDFAState* ec_state);
	void setEcFlag();
	void resetEcFlag();
	
	inline bool outOfBounds() { return depthctr>0; }
	inline NODE_TYPE getType() { return type; }

	NODE_TYPE computeType(NODE_TYPE base,
							vector<ProjectionNFAState*>& origin);
	
	void print();
	void print(unsigned tabs);
	void printSingleState();

private:
	ProjectionDFAState* parent;

	// the multiset of DFA states the NFA represents
	vector<ProjectionNFAState*> origin_states;
	
	// the registered ec states we have to notify
	vector<ProjectionDFAState*> registered_ec_states;
	
	// the transitions that have been computed so far
	vector<ProjectionDFATransition*> transitions;
	
	// indices applicable to matching nodes
	vector<unsigned> cumulative_indices;
	vector<unsigned> noncumulative_indices;

	unsigned depthctr;	
	NODE_TYPE type;

	bool is_ec_opt;			// matches ONLY ec-nodes
	bool ec_node_output;	// a witness has been output
};

#endif // PROJECTIONDFASTATE_H
