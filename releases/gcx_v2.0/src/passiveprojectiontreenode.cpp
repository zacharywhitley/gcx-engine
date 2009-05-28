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
 *  @brief Implementations of header file passiveprojectiontreenode.h.
 *  @details Implementations of constructors, destructor and functions of the
 *           corresponding header file passiveprojectiontreenode.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
#include "passiveprojectiontreenode.h"

PassiveProjectionTreeNode::PassiveProjectionTreeNode() :
parent(NULL), label(NULL) {}

PassiveProjectionTreeNode::PassiveProjectionTreeNode(
		PassiveProjectionTreeNode* _parent, PathStepExpression* _label) :
parent(_parent), label(_label) {}

PassiveProjectionTreeNode::~PassiveProjectionTreeNode() {
	for (unsigned i=0;i<children.size();i++) {
		delete children[i];
	}
    delete label;
}

void PassiveProjectionTreeNode::registerPath(PathExpression* path, unsigned pos) {

    // seek for a child with the current path step expression as its label
    for (unsigned i=0;i<children.size(); i++) {
        PathStepExpression* lc=children[i]->getLabel();
        PathStepExpression* lp=path->getPathStepAt(pos);
        if (lc->isSyntacticallyEqualTo(lp)) {
            if (pos<path->getPathSize()-1) {
               return children[i]->registerPath(path,++pos);
            } else {
               return; // reached end of path step 
            }
        }
    }

    // if no child with path step label exists, insert the node
    PassiveProjectionTreeNode* new_child=
        new PassiveProjectionTreeNode(this,path->getPathStepAt(pos)->clone());
    children.push_back(new_child);
    if (pos<path->getPathSize()-1) {
        new_child->registerPath(path,++pos);
    }
}

void PassiveProjectionTreeNode::print() {
    print(0);
}

void PassiveProjectionTreeNode::print(unsigned tabs) {
    for (unsigned i=0;i<tabs;i++) {
        cout << "  ";
    }
    if (!label) {
        cout << "/";
    } else {
        cout << *label;
    }
    cout << endl;
    for (unsigned i=0; i<children.size(); i++) {
        children[i]->print(tabs+1);
    }
}
