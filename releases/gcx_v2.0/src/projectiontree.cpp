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
 *     @brief Implementations of header file projectiontree.h.
 *     @details Implementations of constructors, destructor and functions of the corresponding header file projectiontree.h.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#include "projectiontree.h"

ProjectionTree::ProjectionTree(VariableTree* _vt, vector<DependencySet*>* _depset) :
root(new ProjectionTreeNode(NULL, TAGID_ROOT, NULL, false,
RoleList::getInstance()->getFreshRole(TAGID_ROOT, rt_root, NULL),false)),
roleinfo(RoleList::getInstance()), labels(NULL) {
    createProjectionTree(_vt->getRootNode(), _depset);
}

ProjectionTree::~ProjectionTree() {
    delete root;
    delete labels;
}

void ProjectionTree::createProjectionTree(VariableTreeNode* varnode, vector<DependencySet*>* depset) {
    vector<DependencyTuple*>* tuples;
    Role* r;
    ParVarMap * parvarmap = ParVarMap::getInstance();
    vector<VariableTreeNode*>* childnodes = varnode->getChildren();
    for (unsigned i=0; i<childnodes->size(); i++) {
        r = RoleList::getInstance()->getFreshRole(
                (*childnodes)[i]->getVar(),
                rt_variable,
                NULL);
        insertNode(
                parvarmap->getParVar((*childnodes)[i]->getVar()),
                (*childnodes)[i]->getVar(),
                parvarmap->getPath((*childnodes)[i]->getVar()),
                false,
                r,
                (*childnodes)[i]->isDirectOutput());
        
        createProjectionTree((*childnodes)[i], depset);
        
        for (unsigned j=0; j<depset->size(); j++) {
            if ((*depset)[j]->getVar()==(*childnodes)[i]->getVar() && (*depset)[j]->hasTuples()) {
                tuples = (*depset)[j]->getTuples();
                for (unsigned k=0; k<tuples->size(); k++) {
                    ROLE_TYPE dep_type=(*tuples)[k]->isCondTuple()?rt_condition:rt_output;
                    r = RoleList::getInstance()->getFreshRole(
                            (*depset)[j]->getVar(),  // associated variable
                            dep_type,                // dep type
                            (*tuples)[k]->getPath());
                    insertNode(
                            (*depset)[j]->getVar(),  // associated variable
                            -1,                      // always -1
                            (*tuples)[k]->getPath(), // associated path
                            true,                    // create a dependency node
                            r,                       // asociated role 
                            false);                  // dependency nodes are never direct output 
                    (*tuples)[k]->setRole(r);
                }
            }
        }
    }
    if (varnode->getVar()==TAGID_ROOT && (*depset)[TAGID_ROOT]->hasTuples()) {
        vector<DependencyTuple*>* tuples = (*depset)[TAGID_ROOT]->getTuples();
        for (unsigned k=0; k<tuples->size(); k++) {
            ROLE_TYPE dep_type=(*tuples)[k]->isCondTuple()?rt_condition:rt_output;
            Role* r = RoleList::getInstance()->getFreshRole(
                    (*depset)[TAGID_ROOT]->getVar(), // associated variable
                    dep_type,                        // dep type
                    (*tuples)[k]->getPath());
            insertNode(
                    (*depset)[TAGID_ROOT]->getVar(),  // associated variable
                    -1,                       // always -1
                    (*tuples)[k]->getPath(),  // associated path
                    true,                     // create a dependency node
                    r,                        // associated role
                    false);                   // dependency nodes are never direct output
            (*tuples)[k]->setRole(r);
        }
    }
}

void ProjectionTree::insertNode(int parent_var, int introduced_var,
                                PathExpression * path, bool dep, Role* role,
                                bool direct_output) {
    root->insertNode(parent_var, introduced_var, path, dep, role, direct_output);
}

void ProjectionTree::removeUnneededNodes(PassiveProjectionTree* ppt) {
    root->removeUnneededNodes(ppt);
}

void ProjectionTree::removeRedundantRoles(FSAMap* fsamap,PassiveProjectionTree* ppt) {
    root->removeRedundantRoles(fsamap,ppt);
}

void ProjectionTree::labelTree() {
    labels=new ProjectionTreeLabels(root);
    labels->updateParentPredecessorInformation();
}

void ProjectionTree::print() {
    root->print();
}
