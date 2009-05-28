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
#include "projectionnfastate.h"
#include "tagmap.h"

ProjectionNFAState::ProjectionNFAState(ProjectionTreeNode* _ptn,
										bool _is_positive,
										NODE_TYPE _type) :
	is_positive(_is_positive), ptn(_ptn), type(_type) {
	setUpTransitions();
}

ProjectionNFAState::~ProjectionNFAState() {

	// REMARK:
	// !!! we have to delete a graph structure
	// !!! this is done externally in the ProjectionNFA contructor
	// !!! by collecting all nodes and deleting them one after the 
	// !!! other... internal deletion causes problems due to
	// !!! double matched nodes during deletion process 
}

void ProjectionNFAState::print() {
	vector<ProjectionNFAState*> e;
	print(e);
}

void ProjectionNFAState::print(vector<ProjectionNFAState*>& printed_states) {
		
	bool skip_state=false;
	for (unsigned i=0; i<printed_states.size() && !skip_state; i++) {
		skip_state|=printed_states[i]==this;
	}

	if (!skip_state) {

		cout << endl << "##### NEXT STATE:" << endl;
		is_positive?cout << "TYPE: 'POSITIVE'" << endl:
					cout << "TYPE: 'NEGATIVE'" << endl;
		cout << "NODE LABEL: '" 
				<< TagMap::getInstance()->getTag(ptn->getNodeTest()) 
				<< "'" << endl;
		
		// epsilon transitions
		cout << "'" << epsilon_transitions.size() 
				<< "' epsilon transitions" << endl;
		for (unsigned i=0; i<epsilon_transitions.size(); i++) {
			epsilon_transitions[i]->printSingleState();
			cout << endl;
		}
		
		// non-epsilon transitions
		cout << "'" << transitions.size() 
				<< "' non-epsilon transitions" << endl;
		for (unsigned i=0; i<transitions.size(); i++) {
			transitions[i].print();
		}
		
		switch (type) {
			case (e_aux):
				cout << "TYPE: aux" << endl;
				break;
			case (e_dom):
				cout << "TYPE: dom" << endl;
				break;
			#ifdef USE_CLOBS
			case (e_clob):
				cout << "TYPE: clob" << endl;
				break;
			#endif // USE_CLOBS
			case (e_out):
				cout << "TYPE: out" << endl;
				break;
		}
		
		printed_states.push_back(this);
		
		for (unsigned i=0; i<epsilon_transitions.size(); i++) {
			epsilon_transitions[i]->print(printed_states);
		}
		for (unsigned i=0; i<transitions.size(); i++) {
			transitions[i].pointsTo()->print(printed_states);
		}
	}
}

void ProjectionNFAState::printDebugInfo() {

		is_positive?cout << "TYPE: 'POSITIVE' ":
					cout << "TYPE: 'NEGATIVE' ";
		cout << "NODE LABEL: '" 
				<< TagMap::getInstance()->getTag(ptn->getNodeTest()) 
				<< "'";

}

void ProjectionNFAState::printSingleState() {
	is_positive?cout << "TYPE: 'POSITIVE' ":
				cout << "TYPE: 'NEGATIVE' ";
	cout << TagMap::getInstance()->getTag(ptn->getNodeTest()); 
}


void ProjectionNFAState::setUpTransitions() {

	if (is_positive) {
			
		vector<ProjectionTreeNode*>* children=ptn->getChildren();

		for (unsigned i=0; i<children->size(); i++) {

			ProjectionTreeNode* child=(*children)[i];
			switch (child->getAxisType()) {

				case e_child:
				{
					ProjectionNFAState* dest_state=
							new ProjectionNFAState(child,true,
													child->getType());
					transitions.push_back(
								ProjectionNFATransition(child->getNodeTest(),
														true,dest_state));
					break;
				}

				case e_self:
				{
					// only output nodes can be CLOB nodes
					ProjectionNFAState* epsilon_state=
							new ProjectionNFAState(child,true,
													child->getType());
					epsilon_transitions.push_back(epsilon_state);
					break;
				}

				case e_descendant:
				{
					TAG nt=child->getNodeTest();
					
					ProjectionNFAState* pos_dest_state=
							new ProjectionNFAState(child,true,child->getType());

					// transition to the positive destination state
					transitions.push_back(
								ProjectionNFATransition(nt,true,pos_dest_state));
					// positive destination state self loop
					pos_dest_state->addTransition(nt,true,pos_dest_state);

					// "NOT *" is unmatchable, thus we avoid unreachable states...
					if (nt!=TAGID_STAR) {
						ProjectionNFAState* neg_dest_state=
								new ProjectionNFAState(child,false,e_aux);
						// transition to the negative destination state
						transitions.push_back(
									ProjectionNFATransition(nt,
															false,
															neg_dest_state));
						// from negative destination state to positive one
						neg_dest_state->addTransition(nt,true,pos_dest_state);
						// from positive destination state to negative one
						pos_dest_state->addTransition(nt,false,neg_dest_state);
						// negative destination state self loop
						neg_dest_state->addTransition(nt,false,neg_dest_state);
					}
					break;
				}
				
				// should never happen
				default:
					break;
			}
		}
	} // neg_ptn never have children
}

void ProjectionNFAState::addTransition(TAG tag,bool positive,ProjectionNFAState* to) {
	transitions.push_back(ProjectionNFATransition(tag,positive,to));
}

void ProjectionNFAState::addStatesMatchingTag(TAG _tag,
											vector<ProjectionNFAState*>& s) {

	// epsilon transitions are ignored here (they don't play a role)
	for (unsigned i=0; i<transitions.size(); i++) {
		if (transitions[i].matchesTag(_tag)) {
			ProjectionNFAState* matching_state=transitions[i].pointsTo();
			s.push_back(matching_state); // this is the resulting state
			
			// ... and we add all states reachable via epsilon transitions
			matching_state->addEpsilonTransitionStates(s);
		}
	}
}

void ProjectionNFAState::addEpsilonTransitionStates(
				                            vector<ProjectionNFAState*>& s) {
    for (unsigned i=0; i<epsilon_transitions.size(); i++) {
        s.push_back(epsilon_transitions[i]);
    }
}

void ProjectionNFAState::appendRoles(vector<unsigned>& cumulative_indices,
										vector<unsigned>& noncumulative_indices) {
	// negative states also point to the corresponding path tree nodes
	// but they do not carry the corresponding role
	if (is_positive)
		ptn->appendRoles(cumulative_indices,noncumulative_indices);
}

bool ProjectionNFAState::isSelfPointing() {
	// self-pointing is always via non-epsilon transitions
	for (unsigned i=0;i<transitions.size(); i++) {
		if (transitions[i].pointsTo()==this)
			return true;
	}
	return false;
}

void ProjectionNFAState::transitiveClosure(vector<ProjectionNFAState*>& tc) {
	for (unsigned i=0; i<epsilon_transitions.size(); i++) {
		bool contained=false;
		for (unsigned j=0;j<tc.size() && !contained; j++) {
			contained=epsilon_transitions[i]==tc[j];
		}
		if (!contained) {
			// recursion after adding the node
			tc.push_back(epsilon_transitions[i]);
			epsilon_transitions[i]->transitiveClosure(tc);
		}
	}
	for (unsigned i=0; i<transitions.size(); i++) {
		bool contained=false;
		for (unsigned j=0;j<tc.size() && !contained; j++) {
			contained=transitions[i].pointsTo()!=NULL &&
						transitions[i].pointsTo()==tc[j];
		}
		if (!contained) {
			// recursion after adding the node
			tc.push_back(transitions[i].pointsTo());
			transitions[i].pointsTo()->transitiveClosure(tc);
		}
	}
}
