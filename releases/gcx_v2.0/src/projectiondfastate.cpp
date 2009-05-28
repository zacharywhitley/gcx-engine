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
 *     @brief Implementations of header file projectiondfastate.h.
 *     @details Implementations of constructors, destructor and functions of the corresponding header file projectiondfastate.h.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#include "projectiondfastate.h"

//! Constructor for the root node
ProjectionDFAState::ProjectionDFAState(ProjectionTree* _pt,PassiveProjectionTree* _ppt) :
    parent(NULL), // the root node has no parent
    config(new TokenConfiguration(_pt)), // place start button 
    pconfig(new PassiveProjectionTreeConfiguration(_ppt)), // place start button 
    transitions(this) {
    update();
}

ProjectionDFAState::ProjectionDFAState(ProjectionDFAState* _parent,
                                       PassiveProjectionTreeConfiguration* _pconfig) :
parent(_parent), config(NULL), pconfig(_pconfig), transitions(NULL) {}

//! constructor for non-root nodes
ProjectionDFAState::ProjectionDFAState(ProjectionDFAState* _parent,
                                        TokenConfiguration* _config,
                                        PassiveProjectionTreeConfiguration* _pconfig) :
parent(_parent), config(_config), pconfig(_pconfig), transitions(this) {
    update();
}

ProjectionDFAState::~ProjectionDFAState() {
    delete config;
    delete pconfig;
}

void ProjectionDFAState::update() {

    // attention: order matters here!
    config->createRoleList(roles,role_counts);

    // keep_node=false applies only for non-text nodes
    keep_node=((parent && parent->isOutput()) ||               // inherited
                (config && config->isOutput()) ||              // by configuration
                (config && config->hasActiveToken()) ||        // DFAstate matched
                (pconfig && pconfig->hasActiveNodes()) ||      // passive pt matches
                (parent && parent->getTokenConfiguration()->forceChildKeep()));
    //keep_node=true; // turn off interleaving of nodes 
    keep_subtree=config->keepSubtree();
    is_output=(parent && parent->isOutput()) || config->isOutput();
    skip_subtree=config->skipSubtree();
    
    // compute cumulative and non-cumulative indices
    for (unsigned i=0;i<roles.size();i++) {
        if (roles[i]->isDosRole()) {
            for (unsigned j=0;j<role_counts[i];j++) {
                cumulative_roles.push_back(roles[i]->getId());
            }
        } else {
            for (unsigned j=0;j<role_counts[i];j++) {
                non_cumulative_roles.push_back(roles[i]->getId());
            }
        }
    }
}

ProjectionDFAState* ProjectionDFAState::takeTransition(TAG t,
                                                        short& t_type) {
    if (keep_subtree) {
        t_type=TRANSITION_KEEP_SUBTREE;
        return this;
    } else if (skip_subtree) {
        t_type=TRANSITION_SKIP_SUBTREE;
        return this;
    } else {
        ProjectionDFAState* transition=transitions.takeTransition(t,t_type);
        if (t_type==TRANSITION_UNKNOWN) {
            computeTransition(t);
            transition=transitions.takeTransition(t,t_type);
        }
        if (transition==NULL) {
            t_type=TRANSITION_SKIP_SUBTREE; // overwrite
            return this;
        } else {
            return transition;
        }
    }
}

ProjectionDFAState* ProjectionDFAState::takeTextTransition() {
    return transitions.takeTextTransition(isRoot());
}

void ProjectionDFAState::computeTransition(TAG t) {
    // we delegate to the transitions, providing all information
    // that are necessary (i.e. caller/parent, tag, and configuration)
    // NOTE: PCDATA transitions must not explicitly be computed
    transitions.computeTransition(this,t);
}


void ProjectionDFAState::print() {
    print(0);    
}

void ProjectionDFAState::print(unsigned tabs,bool is_text) {
    cout << "config=";
    if (!config) {
        cout << "[]";
    } else {
        config->print(is_text);
    }
    if (!is_text && !keep_node) {
        cout << ", [interleave]";
    }

    if (keep_node) cout << ", [keep_node]";

    cout << ", roles=[";
    for (unsigned i=0;i<roles.size();i++) {
        if (i>0) cout << ",";
        cout << role_counts[i] << "*r" << roles[i]->getId();
    }
    cout << "]";
    if (keep_subtree) cout << ", (keep_sub)";
    if (skip_subtree) cout << ", (skip_sub)";
    cout << endl;

    transitions.print(tabs);
}
