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
 *     @brief Implementations of header file projectiondfatransitions.h.
 *     @details Implementations of constructors, destructor and functions of the corresponding header file projectiondfatransitions.h.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#include "projectiondfatransitions.h"
#include "projectiondfastate.h"

ProjectionDFATransitions::ProjectionDFATransitions(
                                            ProjectionDFAState* _parent) :
parent(_parent), text_transition_computed(false), text_transition(NULL) {}

ProjectionDFATransitions::~ProjectionDFATransitions() {
    delete text_transition;
    hash_map<TAG,ProjectionDFAState*>::iterator it=transitions.begin();
    while (it!=transitions.end()) {
        delete (ProjectionDFAState*)it->second;
        it++;
    }
}

ProjectionDFAState* ProjectionDFATransitions::takeTransition(TAG t,
                                                            short& t_type) {
    hash_map<TAG,ProjectionDFAState*>::iterator it=transitions.find(t);
    if (it==transitions.end()) {
        t_type=TRANSITION_UNKNOWN;
        return NULL;
    } else {
        t_type=TRANSITION_REGULAR;
        return it->second;
    }
}

void ProjectionDFATransitions::computeTransition(ProjectionDFAState* parent,TAG t) {
    TokenConfiguration* new_conf=parent->getTokenConfiguration()->applyTag(t);
    PassiveProjectionTreeConfiguration* new_pconf=
        parent->getPassiveProjectionTreeConfiguration()==NULL?
            NULL:parent->getPassiveProjectionTreeConfiguration()->applyTag(t);
    if (new_conf) {
        transitions[t]=new ProjectionDFAState(parent,new_conf,new_pconf);
    } else {
        delete new_pconf;
        transitions[t]=NULL;
    }
}

void ProjectionDFATransitions::computeTextTransition(bool is_root) {

    text_transition_computed=true;
    if (is_root) {
        text_transition=NULL;
    } else {
        TokenConfiguration* new_conf=parent->getTokenConfiguration()->applyText();
        PassiveProjectionTreeConfiguration* new_pconf=
            parent->getPassiveProjectionTreeConfiguration()==NULL?
                NULL:parent->getPassiveProjectionTreeConfiguration()->applyText();
        if (new_conf) {
            text_transition=new ProjectionDFAState(parent,new_conf,new_pconf);
        } else {
            if (parent->isOutput()) {
                text_transition=new ProjectionDFAState(parent,new_pconf);
            } else {
                delete new_pconf;
                text_transition=NULL;
            }
        }
    }
}

void ProjectionDFATransitions::print(unsigned tabs) {

    // if there is a text() transition, then print it
    if (text_transition) {
        for (unsigned i=0; i<=tabs; i++) {
            cout << "  ";
        }
        cout << "#PCDATA --> ";
        text_transition->print(tabs+1,true);
    }
    
    // print the regular transitions
    hash_map<TAG,ProjectionDFAState*>::iterator it=transitions.begin();
    while (it!=transitions.end()) {
        for (unsigned i=0; i<=tabs; i++) {
            cout << "  ";
        }
         cout << "<" << TagMap::getInstance()->getTag(it->first)
                << "> --> ";
        ProjectionDFAState* follow_up=it->second;
        if (follow_up!=NULL) {
            follow_up->print(tabs+1);
        } else {
            cout << "NULL" << endl;
        }
        it++;
    };
}

ProjectionDFAState* ProjectionDFATransitions::takeTextTransition(bool is_root) {
    if (!text_transition_computed) {
        computeTextTransition(is_root);
    }
     return text_transition;
}
