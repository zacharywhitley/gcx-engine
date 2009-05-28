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
 *     @brief Implementations of header file tokenconfiguration.h.
 *     @details Implementations of constructors, destructor and functions of the corresponding header file tokenconfiguration.h.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#include "tokenconfiguration.h"

TokenConfiguration::TokenConfiguration(ProjectionTree* pt) :
parent(NULL), labels(pt->getLabels()) {

    for (unsigned i=0;i<labels->nrOfLabels();i++) {
        active_tokens.push_back(0); // no other tokens 
        passive_tokens.push_back(0); // no other tokens 
    }

    // we place one token at the root node ...
    active_tokens[0]=1; // place token at root label (with id 0) 

    // ... and one on all its dos::node() successors
    vector<unsigned> dos_node_ids;
    labels->getAllRecursiveDosNodeSuccessors(0,dos_node_ids);
    for (unsigned i=0;i<dos_node_ids.size();i++) {
        active_tokens[dos_node_ids[i]]=1;
    }
}

TokenConfiguration::TokenConfiguration(TokenConfiguration* _parent)
    : parent(_parent), labels(parent->getLabels()),
        passive_tokens(parent->getPassiveTokens()) {
    for (unsigned i=0;i<labels->nrOfLabels();i++) {
        active_tokens.push_back(0);
    }
}

TokenConfiguration::~TokenConfiguration() {}

void TokenConfiguration::createRoleList(vector<Role*>& roles,
                                        vector<unsigned>& role_counts) {

    // the role list is derived from the set of those active tokens that
    // are placed right at the end of a path
    // There is one special case:
    // The amount of roles assigned to dos::node()-labels equals to
    // the amount of roles assigned to their predecessor label (cumulative
    // roles!)
    for (unsigned i=0;i<active_tokens.size();i++) {

        ProjectionTreeLabel* cur_label=labels->getLabelById(i);

        // ... so first let us check for tokens placed at final path step
        if (active_tokens[i] && cur_label->atEndOfPath()) {

            ProjectionTreeNode* n=cur_label->getProjectionTreeNode();

            // we add roles for this node ...
            if (n->getRole()) {
                roles.push_back(n->getRole());

                unsigned role_count=cur_label->isDosNodeLabel()?
                        active_tokens[cur_label->getPredecessor()->getId()]:
                        active_tokens[i];
                role_counts.push_back(role_count);
            }

            // ... and also for all self-successors
            // (the role_count computation above is not necessary,
            //  since self-successors are never children of dos::node() nodes)
            vector<ProjectionTreeLabel*>* self_labels=
                                        cur_label->getSelfSuccessors();
            for (unsigned j=0;j<self_labels->size();j++) {
                ProjectionTreeNode* self_n=
                    (*self_labels)[j]->getProjectionTreeNode();
                if (self_n->getRole()) {
                    roles.push_back(self_n->getRole());
                    role_counts.push_back(active_tokens[i]); // identical
                }
            }
        } 
    }
}

bool TokenConfiguration::forceChildKeep() {

    // this method implements our typical descendant-child axis clash,
    // e.g. consider the query:
    //
    // for $a in /a return ($a/b,$a//b)
    //
    // and the document
    //
    // <a><c><b/></c></a>.
    //
    // Although <c> does not get assigned any roles, it obviously
    // must be kept (and cannot be interleaved), to avoid wrong results
    // caused by this axis constellation. The method checks if we have
    // to keep child tags for the current state in order to avoid this
    // clash.

    vector<TAG> child_tags;
    bool child_contains_star_or_node=false;
    vector<TAG> descendant_tags;
    bool descendant_contains_star_or_node=false;
    for (unsigned i=0;i<active_tokens.size();i++) {
        if (active_tokens[i]+passive_tokens[i]) {
            ProjectionTreeLabel* cur=labels->getLabelById(i);
            ProjectionTreeLabel* sls=cur->getSameLevelSuccessor();
            vector<ProjectionTreeLabel*>* css=cur->getChildSuccessors();
        
            if (sls) {
                if (active_tokens[i] && sls->isChildLabel()) {
                    if (sls->isNodeLabel() || sls->isStarLabel()) {
                        child_contains_star_or_node=true;
                    } else if (!sls->matchesText()) {
                        child_tags.push_back(sls->getTag());
                    }
                } else if (sls->isDescendantLabel()) {
                    if (sls->isNodeLabel() || sls->isStarLabel()) {
                        descendant_contains_star_or_node=true;
                    } else if (!sls->matchesText()) {
                        descendant_tags.push_back(sls->getTag());
                    }
                }
            }

            for (unsigned j=0;j<css->size();j++) {
                ProjectionTreeLabel* cs=(*css)[j];
                if (active_tokens[i] && cs->isChildLabel()) {
                    if (cs->isNodeLabel() || cs->isStarLabel()) {
                        child_contains_star_or_node=true;
                    } else if (!cs->matchesText()) {
                        child_tags.push_back(cs->getTag());
                    }
                } else if (cs->isDescendantLabel()) {
                    if (cs->isNodeLabel() || cs->isStarLabel()) {
                        descendant_contains_star_or_node=true;
                    } else if (!cs->matchesText()) {
                        descendant_tags.push_back(cs->getTag());
                    }
                }
            }
        }
    }

    // now check for violating situation
    if (child_contains_star_or_node && descendant_contains_star_or_node) {
        return true;
    }
    if (child_contains_star_or_node && descendant_tags.size()>0) {
        return true;
    }
    if (descendant_contains_star_or_node && child_tags.size()>0) {
        return true;
    }
    for (unsigned i=0;i<child_tags.size();i++) {
        for (unsigned j=0;j<descendant_tags.size();j++) {
            if (child_tags[i]==descendant_tags[j]) {
                return true;
            }
        }
    }
    
    return false; // children must not be kept
}

bool TokenConfiguration::keepSubtree() {

    // we keep the whole subtree if we are in excatly one or more
    // dos::node() states (and NO other states) 
    // (this means we have to keep everything, but there are no more roles)
    bool witness=false;
    for (unsigned i=0;i<active_tokens.size();i++) {
        if (active_tokens[i] || passive_tokens[i]) {
            if (labels->getLabelById(i)->isDosNodeLabel()) {
                witness=true;
            } else {
                return false;
            }
        }
    }

    return witness;
}

bool TokenConfiguration::isOutput() {

    // an output configuration is a configuration where dos::node()
    // states are matched
    for (unsigned i=0;i<active_tokens.size();i++) {
        if (active_tokens[i] && labels->getLabelById(i)->isDosNodeLabel())
            return true;
    }
    
    return false;
}

bool TokenConfiguration::skipSubtree() {
    return isEmpty(); // not really needed any more
}

TokenConfiguration* TokenConfiguration::applyTag(TAG t) {

    // create empty token configuration (with labels and prev_tokens info)
    TokenConfiguration* new_conf=new TokenConfiguration(this);

    // We first scan for matching successors...
    // Note that we will add each matching state exactly once.
    // The count computation will then be done below.
    // In this part, we ignore dos::node() successors;
    // they will be collected in a separate step afterwards.
    vector<ProjectionTreeLabel*> matches;
    for (unsigned i=0;i<active_tokens.size();i++) {

        if (active_tokens[i] || passive_tokens[i]) {

            ProjectionTreeLabel* l=labels->getLabelById(i);
            ProjectionTreeLabel* sls=l->getSameLevelSuccessor();
            vector<ProjectionTreeLabel*>* css=l->getChildSuccessors();

            // first we append the same-level successor if matching ...
            if (sls) {
                if ((active_tokens[i] && sls->isChildLabel() && sls->matchesTag(t)) ||
                    (sls->isDescendantLabel() && sls->matchesTag(t))) {
                    matches.push_back(sls);
                }
            }

            // ... and next all child level successors
            for (unsigned j=0;j<css->size();j++) {
                ProjectionTreeLabel* cs=(*css)[j];
                
                if ((active_tokens[i] && cs->isChildLabel() && cs->matchesTag(t)) ||
                    (cs->isDescendantLabel() && cs->matchesTag(t))) { 
                    matches.push_back(cs);
                }
            }
        }
    }


    // we next append the set of all matched tokens recursively by dos::node()
    // successor states (either same-level or child-level)
    for (unsigned i=0;i<matches.size();i++) {
        ProjectionTreeLabel* cur=matches[i];

        ProjectionTreeLabel* sls=cur->getSameLevelSuccessor();
        vector<ProjectionTreeLabel*>* css=cur->getChildSuccessors();

        if (sls && sls->isDosNodeLabel())
             matches.push_back(sls);
        for (unsigned j=0;j<css->size();j++) {
            ProjectionTreeLabel* cs=(*css)[j];
            if (cs->isDosNodeLabel()) {
                matches.push_back(cs);
            }
        }
    }
    
    // Count computation:
    // We now set the number of active tokens according to what we've
    // computed so far; the rules for this step  are as follows:
    // (1) Each token in the matches-vector will be assigned active tokens
    // (2) The critical point here is how many active tokens to assign:
    //     the code below is commented accordingly
    for (unsigned i=0;i<matches.size();i++) {
        ProjectionTreeLabel* cur=matches[i];
        unsigned id=cur->getId();

        // The predecessor is the root node
        if (cur->getPredecessor()==NULL) {
            new_conf->setActiveTokens(id,1);

        } else {
            unsigned basing_id=0; // will be set next
            // will be set to true if this is the first descendant axis
            // relative to the label with basing_id
            bool is_not_first_descendant_axis=false; 
            unsigned predecessor_id=cur->getPredecessor()->getId();

            // if we consider labels inside a path, the basing id is the
            // id of the leftmost label in this path
            if (cur->getPredecessor()->getPath()==cur->getPath()) {
                basing_id=cur->getLeftmostSLPredecessor()->getId();

                // ... except for the case that the FSA of this label is something else;
                // in this case we take the fsa label as the basing id
                if (labels->getLabelById(basing_id)->getFSALabel()->getId()==basing_id) {
                    is_not_first_descendant_axis=cur->getPredecessor()->
                                                  descendantAxisBetw(basing_id,true);
                } else {
                    basing_id=labels->getLabelById(basing_id)->getFSALabel()->getId();
                    is_not_first_descendant_axis=cur->getPredecessor()->
                                                  descendantAxisBetw(basing_id);
                }

            // otherwise, we have just experienced a path switch 
            } else {
                ProjectionTreeLabel* fsa=cur->getFSALabel();

                // if this label variable is its own fsa, we consider
                // the predecessor as a basis, otherwise we choose the FSA
                // label as the basing one 
                basing_id=cur->getFSALabel()==cur?
                    cur->getPredecessor()->getId():fsa->getId();
                is_not_first_descendant_axis=cur->getPredecessor()->
                            descendantAxisBetw(basing_id);
            }

            switch (cur->getPathStep()->getAxisType()) {                                    
                case at_dos:
                    is_not_first_descendant_axis?
                        new_conf->setActiveTokens(
                            id,new_conf->getLastActiveTokenCountFor(predecessor_id)):
                        new_conf->setActiveTokens(
                            id,new_conf->sumUpActiveTokenCountFor(predecessor_id));
                    break;

                case at_descendant:
                    is_not_first_descendant_axis?
                        new_conf->setActiveTokens(
                            id,getLastActiveTokenCountFor(predecessor_id)):
                        new_conf->setActiveTokens(
                            id,sumUpActiveTokenCountFor(predecessor_id));
                    break;

                case at_child:
                    new_conf->setActiveTokens(
                        id,getLastActiveTokenCountFor(predecessor_id));
                    break;
            }
        }
    }


    // finally, for all active tokens with descendant()- or dos()-label
    // successors, we add the set of active tokens to the passive tokens
    for (unsigned i=0;i<active_tokens.size();i++) {
        if (active_tokens[i]) {
            ProjectionTreeLabel* cur=labels->getLabelById(i);
            ProjectionTreeLabel* sls=cur->getSameLevelSuccessor();
            vector<ProjectionTreeLabel*>* css=cur->getChildSuccessors();
            
            bool has_desc_successors=false;
            has_desc_successors|=sls && sls->isDosOrDescendantLabel();
            for (unsigned j=0;j<css->size() && !has_desc_successors;j++) {
                ProjectionTreeLabel* cs=(*css)[j];
                has_desc_successors|=cs->isDosOrDescendantLabel();
            }

            if (has_desc_successors) {
                new_conf->addPassiveTokens(i,active_tokens[i]);
            }
        }
    }

    // if the configuration is empty, no new state needs to be created 
    // and we return NULL instead of an empty configuration
    if (new_conf->isEmpty()) { 
        delete new_conf;
        new_conf=NULL;
    }

    return new_conf;
}

TokenConfiguration* TokenConfiguration::applyText() {

    // create empty token configuration (with labels and prev_tokens info)
    TokenConfiguration* new_conf=new TokenConfiguration(this);

    // We first scan for matching successors...
    // Note that we will add each matching state exactly once.
    // The count computation will then be done below.
    // In this part, we ignore dos::node() successors;
    // they will be collected in a separate step afterwards.
    vector<ProjectionTreeLabel*> matches;
    for (unsigned i=0;i<active_tokens.size();i++) {

        if (active_tokens[i] || passive_tokens[i]) {

            ProjectionTreeLabel* l=labels->getLabelById(i);
            ProjectionTreeLabel* sls=l->getSameLevelSuccessor();
            vector<ProjectionTreeLabel*>* css=l->getChildSuccessors();

            // first we append the same-level successor if matching ...
            if (sls) {
                if ((active_tokens[i] && sls->isChildLabel() && sls->matchesText()) ||
                    (sls->isDescendantLabel() && sls->matchesText())) {
                    matches.push_back(sls);
                }
            }

            // ... and next all child level successors
            for (unsigned j=0;j<css->size();j++) {
                ProjectionTreeLabel* cs=(*css)[j];
                
                if ((active_tokens[i] && cs->isChildLabel() && cs->matchesText()) ||
                    (cs->isDescendantLabel() && cs->matchesText())) {
                    matches.push_back(cs);
                }
            }
        }
    }

    // we next append the set of all matched tokens recursively by dos::node()
    // successor states (either same-level or child-level)
    for (unsigned i=0;i<matches.size();i++) {
        ProjectionTreeLabel* cur=matches[i];

        ProjectionTreeLabel* sls=cur->getSameLevelSuccessor();
        vector<ProjectionTreeLabel*>* css=cur->getChildSuccessors();

        if (sls && sls->isDosNodeLabel()) matches.push_back(sls);
        for (unsigned j=0;j<css->size();j++) {
            ProjectionTreeLabel* cs=(*css)[j];
            if (cs->isDosNodeLabel()) matches.push_back(cs);
        }
    }
    
    // Count computation:
    // We now set the number of active tokens according to what we've
    // computed so far; the rules for this step  are as follows:
    // (1) Each token in the matches-vector will be assigned active tokens
    // (2) The critical point here is how many active tokens to assign:
    //     Each path step (and hence ProjectionTreeLabel) has exactly one
    //     parent label (except for the ROOT, of course). This parent label
    //     might either be a same-level or a parent-level label. We thus
    //     distinguish these three scenarios; the code is commented below.
    for (unsigned i=0;i<matches.size();i++) {
        ProjectionTreeLabel* cur=matches[i];
        unsigned id=cur->getId();

        // (a) The predecessor is the root node
        if (cur->getPredecessor()==NULL) {
            new_conf->setActiveTokens(id,1);

        } else {

            unsigned basing_id=0; // will be set next
            bool is_not_first_descendant_axis=false; // will be set next
            unsigned predecessor_id=cur->getPredecessor()->getId();

            // if we consider labels inside a path, the basing id is the
            // if of the leftmost label in this path
            if (cur->getPredecessor()->getPath()==cur->getPath()) {
                basing_id=cur->getLeftmostSLPredecessor()->getId();

                // ... except for the case that the FSA of this label is something else;
                // in this case we take the fsa label as the basing id
                if (labels->getLabelById(basing_id)->getFSALabel()->getId()==basing_id) {
                    is_not_first_descendant_axis=cur->getPredecessor()->
                                                  descendantAxisBetw(basing_id,true);
                } else {
                    basing_id=labels->getLabelById(basing_id)->getFSALabel()->getId();
                    is_not_first_descendant_axis=cur->getPredecessor()->
                                                  descendantAxisBetw(basing_id);
                }

            // else we either consider the FSA label or the predecessor label
            // as the basing one
            } else {
                ProjectionTreeLabel* fsa=cur->getFSALabel();

                // if this label variable is its own fsa, we consider
                // the predecessor as a basis, otherwise we choose the FSA
                // label as the basing one 
                basing_id=cur->getFSALabel()==cur?
                    cur->getPredecessor()->getId():fsa->getId();
                is_not_first_descendant_axis=cur->getPredecessor()->
                                descendantAxisBetw(basing_id);
            }

            switch (cur->getPathStep()->getAxisType()) {                                    
                case at_dos:
                    is_not_first_descendant_axis?
                        new_conf->setActiveTokens(
                            id,new_conf->getLastActiveTokenCountFor(predecessor_id)):
                        new_conf->setActiveTokens(
                            id,new_conf->sumUpActiveTokenCountFor(predecessor_id));
                    break;

                case at_descendant:
                    is_not_first_descendant_axis?
                        new_conf->setActiveTokens(
                            id,getLastActiveTokenCountFor(predecessor_id)):
                        new_conf->setActiveTokens(
                            id,sumUpActiveTokenCountFor(predecessor_id));
                    break;

                case at_child:
                    new_conf->setActiveTokens(
                        id,getLastActiveTokenCountFor(predecessor_id));
                    break;
            }
        }
    }

    // there is no need to set passive tokens, since there will be no
    // successor states for PCDATA-carrying nodes

    // if the configuration is empty, no new state needs to be created 
    // and we return NULL instead of an empty configuration
    if (!new_conf->hasActiveToken()) { 
        delete new_conf;
        new_conf=NULL;
    } 

    return new_conf;
}

bool TokenConfiguration::hasActiveToken() {
    for (unsigned i=0;i<active_tokens.size();i++) {
        if (active_tokens[i]) return true;
    }
    return false;
}

bool TokenConfiguration::isEmpty() {
    for (unsigned i=0;i<active_tokens.size();i++) {
        if (active_tokens[i]+passive_tokens[i]) return false;
    }
    return true;
}

void TokenConfiguration::print(bool is_text) {
    cout << "[";
    for (unsigned i=0;i<active_tokens.size();i++) {
        if (i>0) cout << ",";
        cout << active_tokens[i];
    }
    cout << "]";
    if (!is_text){
        cout << " - prev=[";
        for (unsigned i=0;i<passive_tokens.size();i++) {
            if (i>0) cout << ",";
            cout << passive_tokens[i];
        }
        cout << "]";
    }
}

unsigned TokenConfiguration::sumUpActiveTokenCountFor(unsigned token_id) {
    if (parent) {
        return parent->sumUpActiveTokenCountFor(token_id)+
                active_tokens[token_id];
    } else {
        return active_tokens[token_id];
    }
}

unsigned TokenConfiguration::getLastActiveTokenCountFor(unsigned token_id) {

    if (active_tokens[token_id]) {
        return active_tokens[token_id];
    } else if (parent!=NULL) {
        return parent->getLastActiveTokenCountFor(token_id);
    } else {
        Error::getInstance()->throwError("TokenConfiguration: Illegal Configuration.");
    }

    return 0; // never reached
}
