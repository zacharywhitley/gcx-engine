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
 *  @brief Implementations of header file projectiontreelabel.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file projectiontreelabel.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
#include "projectiontreelabel.h"
#include "varname.h"
#include "fsamap.h"
#include "pathsteptagexpression.h"
#include "error.h"

ProjectionTreeLabel::ProjectionTreeLabel(
    ProjectionTreeNode* _n,unsigned _position,unsigned _id,
    ProjectionTreeLabel* _same_level_successor,
    vector<ProjectionTreeLabel*> _child_successors,
    vector<ProjectionTreeLabel*> _self_successors) :
parent(NULL), predecessor(NULL), n(_n), position(_position),
id(_id), same_level_successor(_same_level_successor),
child_successors(_child_successors),
self_successors(_self_successors) {
}

ProjectionTreeLabel::~ProjectionTreeLabel() {}

void ProjectionTreeLabel::updateParentPredecessorInformation(
                                        ProjectionTreeLabel* _parent,
                                        ProjectionTreeLabel* _predecessor) {
    parent=_parent;
    predecessor=_predecessor;

    if (same_level_successor) {
        same_level_successor->
            updateParentPredecessorInformation(_parent,this);
    } else {
        for (unsigned i=0;i<child_successors.size();i++) {
            child_successors[i]->
                updateParentPredecessorInformation(this,this);
        }
    }
}

PathExpression* ProjectionTreeLabel::getPath() {
    return n->getPath();
}

PathStepExpression* ProjectionTreeLabel::getPathStep() {
    return n->getPath()?n->getPath()->getPathStepAt(position):NULL;
}

bool ProjectionTreeLabel::atEndOfPath() {
    return n->getPath()?n->getPath()->getPathSize()==position+1:true;
}

bool ProjectionTreeLabel::isDosLabel() {
    return n->getPath()?
        n->getPath() && getPathStep()->getAxisType()==at_dos:false;
}

bool ProjectionTreeLabel::isChildOrDescendantLabel() {
    if (!n->getPath()) return false;
    return getPathStep()->getAxisType()==at_child ||
            getPathStep()->getAxisType()==at_descendant;
}

bool ProjectionTreeLabel::isDescendantLabel() {
    return n->getPath()?getPathStep()->getAxisType()==at_descendant:false;
}

bool ProjectionTreeLabel::isChildLabel() {
    return n->getPath()?getPathStep()->getAxisType()==at_child:false;
}

bool ProjectionTreeLabel::isDosNodeLabel() {
    return n->getPath()?
        getPathStep()->isNodeNodeTest() && 
        getPathStep()->getAxisType()==at_dos:false;
}

bool ProjectionTreeLabel::isNodeLabel() {
    return n->getPath()?getPathStep()->isNodeNodeTest():false;
}

bool ProjectionTreeLabel::isStarLabel() {
    return n->getPath()?getPathStep()->isStarNodeTest():false;
}

bool ProjectionTreeLabel::matchesText() {
    return n->getPath()?
        getPathStep()->isTextNodeTest()||getPathStep()->isNodeNodeTest():false;
}

bool ProjectionTreeLabel::isDosOrDescendantLabel() {
    return n->getPath()?
        getPathStep()->getAxisType()==at_dos
        ||getPathStep()->getAxisType()==at_descendant:false;
}

bool ProjectionTreeLabel::hasSameLevelOrChildSuccessors() {
    return same_level_successor || child_successors.size();
}

TAG ProjectionTreeLabel::getTag() {
    if (n->isRoot()) return TAGID_ROOT;
    
    // we abort if the path step is not a tag node test 
    // (the method should never have been called in this case)
    if (getPathStep()->isTagNodeTest()) {
        PathStepTagExpression* ptt=(PathStepTagExpression*) getPathStep();
        return ptt->getNodeTest();
    } else {
        Error::getInstance()->throwError("ProjectionTreeLabel: Internal Cast Error.");
        return TAGID_ROOT; // to avoid compiler warning
    }
    
}

bool ProjectionTreeLabel::matchesTag(TAG t) {
    if (!n->getPath()) return false;

    return getPathStep()->isMatchingTag(t);
}

void ProjectionTreeLabel::print() {

    cout << "[" << id << "] "; 

    // print path and internal position info
    if (n->isRoot()) {
        cout << "path=<ROOT>";
    } else if (n->getPath()) {
        cout << "path=" << *n->getPath();
        cout << ", step=[" << position << "]";
    } else {
        cout << "path=%";
    }

    // print same_level successors
    cout << "; sl_succ=";
    if (same_level_successor) {
        cout << same_level_successor->getId();
    } else {
        cout << "%";
    }

    // print child successors
    cout << ", c_succ=[";
    for (unsigned i=0;i<child_successors.size();i++) {
        if (i) cout << ",";
        cout << child_successors[i]->getId();
    }
    cout << "]";

    // print self successors
    cout << ", self_succ=[";
    for (unsigned i=0;i<self_successors.size();i++) {
        if (i) cout << ",";
        cout << self_successors[i]->getId();
    }
    cout << "]";

    // print basing variable
    cout << " (";
    int var_id=getProjectionTreeNode()->getVar();
    if (var_id>=0) {
        cout << "$" << VarName::getInstance()->getVarname(var_id);
    } else {
        cout << "%";
    }
    cout << ")";

    if (parent!=NULL) {
        cout << " p=[" << parent->getId() << "]" ;
    }

    cout << endl;
}

unsigned ProjectionTreeLabel::getDependingVar() {
    int var=getProjectionTreeNode()->getVar();
    return var==-1?parent->getDependingVar():(unsigned)var;
}

ProjectionTreeLabel* ProjectionTreeLabel::locateVarAncestorLabel(unsigned var) {
    if (getProjectionTreeNode()->getVar()==(int)var) {
        return this;
    } else {
        for (unsigned i=0;i<self_successors.size();i++) {
            if (self_successors[i]->getProjectionTreeNode()->getVar()==(int)var) {
                return this;
            }
        }

        // else (i.e. no success)
        return parent->locateVarAncestorLabel(var);
    }
}

ProjectionTreeLabel* ProjectionTreeLabel::getFSALabel() {
    int var_fsa=FSAMap::getInstance()->getFSA(getDependingVar());
    return locateVarAncestorLabel(var_fsa);
}

AXIS_TYPE ProjectionTreeLabel::getAxisFollowingFSA() {
    return getAxisFollowingFSA(
            FSAMap::getInstance()->getFSA(getDependingVar()));
}

AXIS_TYPE ProjectionTreeLabel::getAxisFollowingFSA(unsigned fsa) {
    return predecessor->getProjectionTreeNode()->getVar()==(int)fsa?
        getPathStep()->getAxisType():predecessor->getAxisFollowingFSA(fsa);
}


bool ProjectionTreeLabel::descendantAxisBetw(unsigned basing_id,
                                                bool consider_final) {

    // if this is the basing_id label we are done
    if (id==basing_id) {
        if (consider_final) {
            return getPathStep()->getAxisType()==at_descendant ||
                    getPathStep()->getAxisType()==at_dos;
        } else {
            return false; 
        }
    } else if (getPathStep()->getAxisType()==at_descendant ||
                getPathStep()->getAxisType()==at_dos) {
        return true; // we found such an axis
    } else {
        return getPredecessor()->descendantAxisBetw(basing_id,consider_final);
    }
}

ProjectionTreeLabel* ProjectionTreeLabel::getLeftmostSLPredecessor() {
    if (predecessor==parent) {
        return this;
    } else {
        return predecessor->getLeftmostSLPredecessor();
    }
}
