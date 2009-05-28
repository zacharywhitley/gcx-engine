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
 *     @brief Implementations of header file projectiontreenode.h.
 *     @details Implementations of constructors, destructor and functions of the corresponding header file projectiontreenode.h.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
#include "projectiontreenode.h"

ProjectionTreeNode::ProjectionTreeNode(ProjectionTreeNode* _parent, int _var,
                                       PathExpression* _path, bool _dep, Role* _role, bool _direct_output) :
parent(_parent), var(_var), path(_path), dep(_dep), role(_role), direct_output(_direct_output) {}

ProjectionTreeNode::~ProjectionTreeNode() {
    for (unsigned i=0; i<children.size(); i++) {
        delete children[i];
    }
}

bool ProjectionTreeNode::insertNode(int parent_var, int introduced_var,
    PathExpression * path, bool dep, Role* role, bool direct_output) {

    if (parent_var==var) {
        ProjectionTreeNode* child=
        new ProjectionTreeNode(this, introduced_var, path, dep, role, direct_output);
        children.push_back(child);
        
        return true;
    } else {
        bool created=false;
        for (unsigned i=0; i<children.size() && !created; i++) {
            created=children[i]->insertNode(parent_var, introduced_var, path, dep,
                                             role, direct_output);
        }

        return created;
    }
}

void ProjectionTreeNode::removeUnneededNodes(PassiveProjectionTree* ppt) {
    for (unsigned i=0; i<children.size(); i++) {
        if (children[i]->getPath()) {
            if (children[i]->getPath()->hasInnerTextNodeTest() &&
               !children[i]->getPath()->getPathStepAfterTextNodeTest()->isDosNodeStep()) {
                children[i]->removeSubtreeInclSelf(ppt);
                i--;
            } else if (children[i]->getPath()->hasTerminatingTextNodeTest()) {
                children[i]->removeSubtreeExclSelf(ppt);
            } else {
                children[i]->removeUnneededNodes(ppt);
            }
        } else {
            children[i]->removeUnneededNodes(ppt);
        }
    }
}

void ProjectionTreeNode::removeRedundantRoles(FSAMap* fsamap,PassiveProjectionTree* ppt) {
    if (children.size()>0 && !direct_output) { // !direct_output added by msc
        setRedundantRoleSelf();
    } else if (dep && isDosDepNode() && parent) { // process only /dos::node()-dep. nodes
        vector<ProjectionTreeNode*>* siblings = parent->getChildren();
        
        // check if this /dos::node()-dependency node has at least one sibling,
        // which derives from the variable tree...
        bool perform_check = false;
        for (unsigned i=0; i<siblings->size(); i++) {
            if (!(*siblings)[i]->isDepNode()) {
                perform_check = true;
                break;
            }
        }
        
        // ...if such a node exists, we perform a (expensive) deeper inspection
        // of all siblings of this /dos::node()-dependency node, checking, if we
        // could delete some siblings of this /dos::node() dependency node including
        // their subtrees...
        if (perform_check && parent->getVar()>=TAGID_ROOT) {
            
            // ...before search is executed, we need to know the first straight ancestor (FSA)
            // of this /dos::node()-dependency node...
            unsigned par_fsa = fsamap->getFSA(parent->getVar());
            
            // ...after obtaining the FSA, we proceed bottom-up from this /dos::node()-dependency
            // node in the projection tree, collecting all nodes before this FSA...
            vector<ProjectionTreeNode*> par_fsa_ancestors;            
            bool par_fsa_sat = false;
            ProjectionTreeNode* par = parent;
            while (par) {
                if (par_fsa_sat) {
                    par_fsa_ancestors.push_back(par);
                }
                if (!par_fsa_sat && par->getVar()>=TAGID_ROOT && (unsigned)par->getVar()==par_fsa) {
                    par_fsa_sat = true;
                }
                par = par->getParent();
            }
            
            // ...after collecting all nodes before the FSA, we start inspecting all siblings of the
            // /dos::node()-dependency node, by checking if:
            // a.) they are a node, which derives from the variable tree and which is not the
            //     /dos::node()-dependency node (this object), which forms the basis of the inspection.
            // b.) their FSA or a FSA of a node in their subtree does not point to the collected
            //     FSAs, which are the nodes before the FSA of this /dos::node()-dependency node.
            // c.) their path is contained by this /dos::node()-dependency node path. If the path
            //     of the sibling is empty (NULL) this sibling and his subtree is only deleted if the
            //     path of the /dos::node()-dependency node is of the form /dos::node() and does not
            //     contain any further pathstep(s) before this (last) /dos::node() pathstep, i.e the
            //     sibling including his subtree is only deleted if the path size of the
            //     /dos::node()-dependency node is one.
            for (unsigned i=0; i<siblings->size(); i++) {
                if ((*siblings)[i]!=this && !(*siblings)[i]->isDepNode() && 
                        !(*siblings)[i]->hasSubtreeMatchingAncestorFSA(fsamap, par_fsa_ancestors) &&
                        ((*siblings)[i]->getPath() && (*siblings)[i]->getPath()->isSemanticallyContainedIn(path)) ||
                                !(*siblings)[i]->getPath()) {
                    if (!(*siblings)[i]->getPath()) {
                        if (path->getPathSize()==1 && path->getTailPathStep()->isDosNodeStep()) {
                            (*siblings)[i]->removeSubtreeInclSelf(ppt);
                            siblings = parent->getChildren();
                            i--;
                        }        
                    } else {
                        (*siblings)[i]->removeSubtreeInclSelf(ppt);
                        siblings = parent->getChildren();
                        i--;
                    }
                }
            }
        }
    }
    
    for (unsigned i=0; i<children.size(); i++) {
        children[i]->removeRedundantRoles(fsamap,ppt);
    }    
}

bool ProjectionTreeNode::hasSubtreeMatchingAncestorFSA(FSAMap* fsamap, vector<ProjectionTreeNode*> ancestors) {
    bool matching_fsa_ancestor = false;
    if (ancestors.size()<=0) {
        return false;
    }
    
    for (unsigned i=0; i<ancestors.size(); i++) {
        if (!dep && var>=TAGID_ROOT && fsamap->getFSA(var)==(unsigned)ancestors[i]->getVar()) {
            matching_fsa_ancestor = true;
            break;
        }
    }
    
    if (!matching_fsa_ancestor) {
        for (unsigned i=0; i<children.size(); i++) {
            matching_fsa_ancestor |= children[i]->hasSubtreeMatchingAncestorFSA(fsamap, ancestors);
        }
    }
    
    return matching_fsa_ancestor;
}

void ProjectionTreeNode::removeSubtreeInclSelf(PassiveProjectionTree* ppt) {
    removeSubtreeExclSelf(ppt);

    if (parent) {
        vector<ProjectionTreeNode*>* siblings = parent->getChildren();
        for (unsigned i=0; i<siblings->size(); i++) {
            if ((*siblings)[i]==this) {
                if (!(*siblings)[i]->getRole() ||
                        RoleList::getInstance()->removeRole((*siblings)[i]->getRole())) {
                    (*siblings)[i]->registerToPassiveProjectionTree(ppt);
                    delete (*siblings)[i];
                    siblings->erase(siblings->begin()+i);
                }
            }
        }
    } else { // ROOT
        if (!role || RoleList::getInstance()->removeRole(role)) {
            this->registerToPassiveProjectionTree(ppt);
            delete this;
        }
    }
}

void ProjectionTreeNode::removeSubtreeExclSelf(PassiveProjectionTree* ppt) {
    for (unsigned i=0; i<children.size(); i++) {
        if (!children[i]->getRole() || RoleList::getInstance()->removeRole(children[i]->getRole())) {
            children[i]->removeSubtreeExclSelf(ppt);
            if (children.size()>0) {
                children[i]->registerToPassiveProjectionTree(ppt);
                delete children[i];
                children.erase(children.begin()+i);
                i--;    
            }            
        }
    }
}

void ProjectionTreeNode::setRedundantRoleSelf() {
    if (!role || RoleList::getInstance()->removeRole(role)) {
        role=NULL;
    }
}

void ProjectionTreeNode::print() {
    print(0);
}

void ProjectionTreeNode::print(unsigned tabs) {
    for (unsigned i=0; i<tabs; i++) {
        cout << "  ";
    }

    if (var>-1) {
        cout << "($" << VarName::getInstance()->getVarname(var) << ") => ";
    }

    if (path) {
        cout << *(path);
    } else {
        cout << "%/%";
    }

    if (role) {
        cout << " (r" << role->getId() << ",";
        switch (role->getType()) {
            case rt_root:
                cout << "root";
                break;
            case rt_variable:
                cout << "var";
                break;
            case rt_condition:
                cout << "cond";
                break;
            case rt_output:
                cout << "out";
                break;
        }
        cout << ",$" << VarName::getInstance()->getVarname(role->getBasingVar())
                << ",$"
                << VarName::getInstance()->getVarname(role->getBasingFSA()) << ")";
    } else {
        cout << "    <= redundant role/node";
    }
    
    if (direct_output) {
        cout << " *";
    }
    cout << endl;

    for (unsigned i=0; i<children.size(); i++) {
        children[i]->print(tabs+1);
    }
}

void ProjectionTreeNode::registerToPassiveProjectionTree(PassiveProjectionTree* ppt) {
    PathExpression* root_to_cur=getRootPath();
    ppt->registerPath(root_to_cur);
    delete root_to_cur;
}

PathExpression* ProjectionTreeNode::getRootPath() {
    vector<PathExpression*> paths;
    getPathsFromRoot(paths);

    PathExpression* r=new PathExpression();
    for (unsigned i=0;i<paths.size();i++) {
        PathExpression* cur=paths[i];
        if (cur && !cur->isEmptyPath()) {
            for (unsigned i=0;i<cur->getPathSize();i++) {
                r->addPathStep(cur->getPathStepAt(i)->clone());
            }
        }
    }

    return r;
}

void ProjectionTreeNode::getPathsFromRoot(vector<PathExpression*>& paths) {
    if (parent) {
        parent->getPathsFromRoot(paths);
    }
    paths.push_back(path);
}
