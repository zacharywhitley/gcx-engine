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
#include "projectiondfastate.h"
#include <set>

ProjectionDFAState::ProjectionDFAState(
				ProjectionNFAState* p_nfa_state) :
	parent(NULL), depthctr(0), type(e_dom), is_ec_opt(false),
	ec_node_output(false) {

	origin_states.push_back(p_nfa_state);
	p_nfa_state->addEpsilonTransitionStates(origin_states);
	for (unsigned i=0; i<origin_states.size(); i++) {
		origin_states[i]->appendRoles(cumulative_indices,
										noncumulative_indices);
	}

	type=computeType(type,origin_states);
}

ProjectionDFAState::ProjectionDFAState(
				ProjectionDFAState* _parent,
				vector<ProjectionNFAState*> p_nfa_states) :
	parent(_parent), origin_states(p_nfa_states), depthctr(0),
	type((parent->getType()==e_out)?e_out:e_aux),
	is_ec_opt(true), ec_node_output(false) {

	for (unsigned i=0; i<origin_states.size(); i++) {
		origin_states[i]->appendRoles(cumulative_indices,
										noncumulative_indices);
		is_ec_opt&=origin_states[i]->isPositive() &&
					origin_states[i]->getPtn()->isEcNode();
	}

	// in general, existence-check optimizable nodes are always
	// e_dom, thus an existence-check e_aux node does not exist
	// -> this (might?) be necessary to avoid conflicts that
	// aris with nodes that are kept due to the child-desc special
	// case
	if (origin_states.size()==0) is_ec_opt=false;

	// in case this node is an existence-check node, we register the
	// node to the nearest ancestor node that matches at least one
	// ProjectionNFANode... I'm not absolutely sure whether it is
	// the best thing we can do, but it should do except for some
	// exceptional cases. In all cases, it is safe to do so, the worst
	// thing that can happen is that nodes are not projected away
	if (is_ec_opt) parent->registerEcState(this);
	
	type=computeType(type,origin_states);
}

ProjectionDFAState::~ProjectionDFAState() {
	// here we have a tree structure, this is why we can delete nodes
	// in a straightforward fashion
	for (unsigned i=0; i<transitions.size(); i++) {
		delete transitions[i]->pointsTo();
	}
}

ProjectionDFAState* ProjectionDFAState::takeTransition(TAG tag) {

	if (depthctr>0) {

		depthctr++;
		return this;

	} else {

		// first we check whether this transition has been computed yet
		bool found=false;
		ProjectionDFAState* next=NULL;
		for (unsigned i=0; i<transitions.size() && !found; i++) {
			if (transitions[i]->getTag()==tag) {
				found=true; // has already been computed
				next=transitions[i]->pointsTo(); // may be NULL
			}
		}

		// case 1: transition into nirvana (has already been computed)
		if (found && !next) {

			depthctr++;
			return this;
			
		// case 2: transition exists (has already been computed before)
		} else if (found && next) {

			return next;
			
		// case 3: not yet computed, so let's compute it now
		} else { // (!found)

			// check all origin states to contain matching transitions
			vector<ProjectionNFAState*> matches;
			for (unsigned i=0; i<origin_states.size(); i++) {
				ProjectionNFAState* cur_state=origin_states[i];
				cur_state->addStatesMatchingTag(tag,matches);
			}

			// matches now contains a list of all basing NFA nodes, so
			// all we have to do is to create a new transition under tag
			// to this node and return the node
			if (matches.size()>0) {
				// ... there exists at least one transition
				ProjectionDFAState* child=new ProjectionDFAState(this,matches);
				addTransition(tag,child);
				return child;
			} else {
				// ... there is no transition
				addTransition(tag,NULL);
				depthctr++;
				return this;
			}
		}
	}
}

void ProjectionDFAState::registerEcState(ProjectionDFAState* ec_state) {

	if (parent==NULL)
		return; // no registration necessary

	// scan for a /positive/ origin state
	for (unsigned i=0;i<origin_states.size();i++) {
		if (origin_states[i]->isPositive()) {
			registered_ec_states.push_back(ec_state);
			return;
		}
	}

	parent->registerEcState(ec_state);
}

void ProjectionDFAState::resetEcFlag() {
	ec_node_output=false;
}

void ProjectionDFAState::setEcFlag() {
	ec_node_output=true;
}

ProjectionDFAState* ProjectionDFAState::traceBack() {

	if (depthctr>0) {
		depthctr--;
		return this;
	} else {
		for (unsigned i=0;i<registered_ec_states.size();i++)
			registered_ec_states[i]->resetEcFlag(); // notify
		return parent;
	}
}

void ProjectionDFAState::print() {
	print(0);
}

NODE_TYPE ProjectionDFAState::computeType(
				NODE_TYPE base,
				vector<ProjectionNFAState*>& origin) {

	// type e_out inheritance
	NODE_TYPE ret=base;

	// early breaking
	if (ret==e_out) {
		return e_out;
	}

	// compute maximum type
	for (unsigned i=0;i<origin.size();i++) {
		if (origin[i]->getType()>ret) {
			ret=origin[i]->getType();
		}
	}

	// check for exceptio case
	if (ret==e_aux) {
		// state set represented by the parent
		vector<ProjectionNFAState*> p_origin=
						parent->getOriginStates();

		// projection tree nodes represented by parent
		vector<ProjectionTreeNode*> p_pnodes;
		for (unsigned i=0;i<p_origin.size();i++) {
			p_pnodes.push_back(p_origin[i]->getPtn());
		}

		// we now check the projection tree node set to
		// contain a same-labeled child (or matching one)
		// once occuring with e_child and once with
		// e_descendant
		set<TAG> child_tags;
		set<TAG> desc_tags;
		for (unsigned i=0;i<p_pnodes.size();i++) {
			ProjectionTreeNode* cur=p_pnodes[i];
			vector<ProjectionTreeNode*>* cur_children=
										cur->getChildren();
			for (unsigned j=0;j<cur_children->size();j++) {
				if ((*cur_children)[j]->getAxisType()==e_child) {
					child_tags.insert(
							(*cur_children)[j]->getNodeTest());
				} else {
					desc_tags.insert(
							(*cur_children)[j]->getNodeTest());
				}
			}
		}

		set<TAG>::iterator c_it;
		for (c_it=child_tags.begin();c_it!=child_tags.end();c_it++) {
			set<TAG>::iterator d_it;
			for (d_it=desc_tags.begin();d_it!=desc_tags.end();d_it++) {
				// NOTES:
				// - in case there are TAGID_STARs, we would never
				//   enter this case (the node would be typed > e_aux)
				if (*c_it==*d_it) {
					return e_dom;
				}
			}
		}
	}
	
	return ret;
}

void ProjectionDFAState::print(unsigned tabs) {

	cout << "CUM={";
	for (unsigned i=0; i<cumulative_indices.size(); i++) {
		cout << cumulative_indices[i];
		if (i<cumulative_indices.size()-1)
			cout << ",";
	}
	cout << "}";

	cout << " NONCUM={";
	for (unsigned i=0; i<noncumulative_indices.size(); i++) {
		cout << noncumulative_indices[i];
		if (i<noncumulative_indices.size()-1)
			cout << ",";
	}
	cout << "}";
	cout << " [";
	for (unsigned i=0; i<origin_states.size(); i++) {
		origin_states[i]->printDebugInfo();
		if (i!=origin_states.size()-1) {
			cout << ",";
		}
	}	
	cout << "]";
	if (depthctr>0) {
		cout << " ... at depth '" << depthctr << "'";
	}

	cout << ", type=";
	switch (type) {
		case (e_aux):
			cout << "aux";
			break;
		case (e_dom):
			cout << "dom";
			break;
		#ifdef USE_CLOBS
		case (e_clob):
			cout << "clob";
			break;
		#endif // USE_CLOBS
		case (e_out):
			cout << "out";
			break;
	}
		

	cout << endl;
	
	for (unsigned i=0; i<transitions.size(); i++) {
		transitions[i]->print(tabs+1);
	}
}

void ProjectionDFAState::printSingleState() {
	cout << "PROJECTION DFA (SINGLE STATE) PRINTING NOT YET IMPLEMENTED" << endl;
}

void ProjectionDFAState::addTransition(TAG tag,ProjectionDFAState* state) {
	transitions.push_back(new ProjectionDFATransition(tag,state));
}
