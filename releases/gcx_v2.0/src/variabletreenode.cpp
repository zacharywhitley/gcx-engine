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
 *     @brief Implementations of header file variabletreenode.h.
 *     @details Implementations of constructors, destructor and functions of the corresponding header file variabletreenode.h.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#include "variabletreenode.h"

VariableTreeNode::VariableTreeNode() :
parent(NULL), var(TAGID_ROOT), direct_output(false) {}

VariableTreeNode::VariableTreeNode(VariableTreeNode* _parent, unsigned _var,
                                   bool _direct_output) :
parent(_parent), var(_var), direct_output(_direct_output) {}

VariableTreeNode::~VariableTreeNode() {
    for (unsigned i=0; i<children.size(); i++) {
        delete children[i];
    }
}

bool VariableTreeNode::insertNode(unsigned parent_var,
                                   unsigned introduced_var,
                                   bool direct_output) {
    if (parent_var==var) {
        VariableTreeNode* child=new VariableTreeNode(this, introduced_var, direct_output);
        children.push_back(child);

        return true;
    } else {
        bool created=false;
        for (unsigned i=0; i<children.size() && !created; i++) {
            created=children[i]->insertNode(parent_var, introduced_var, direct_output);
        }

        return created;
    }
}

void VariableTreeNode::print() {
    print(0);
}

void VariableTreeNode::print(unsigned tabs) {
    for (unsigned i=0; i<tabs; i++) {
        cout << "  ";
    }
    cout << "($" << VarName::getInstance()->getVarname(var) << ")";
    if (direct_output) {
        cout << " *";
    }
    cout << endl;
    for (unsigned i=0; i<children.size(); i++) {
        children[i]->print(tabs+1);
    }
}
