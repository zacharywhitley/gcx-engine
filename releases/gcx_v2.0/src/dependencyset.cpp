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
 *     @brief Implementations of header file dependencyset.h.
 *     @details Implementations of constructors, destructor and functions of the corresponding header file dependencyset.h.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#include "dependencyset.h"

DependencySet::DependencySet(unsigned _var) :
var(_var) {}

DependencySet::~DependencySet() {
    for (unsigned i=0; i<tuples.size(); i++) {
        delete tuples[i];
    }
}

void DependencySet::insertTuple(PathExpression* path, bool pos, bool dos, bool cond) {
    PathExpression* path_copy=path?path->clone():new PathExpression();
    if (pos) {
        path_copy->getTailPathStep()->setAttribute(new PathStepAttributePosition(1));
    }
    if (dos) {
        path_copy->addPathStep(new PathStepNodeExpression(at_dos));
    }
    DependencyTuple* tuple = new DependencyTuple(path_copy, cond);
    tuples.push_back(tuple);
}

void DependencySet::removeSyntacticallyEqualTuple(PassiveProjectionTree* ppt) {
    for (unsigned i=0; i<tuples.size()-1; i++) {
        for (unsigned j=i+1; j<tuples.size(); j++) {
            if (tuples[i]->isSyntacticallyEqualTo(tuples[j])) {
                tuples[j]->registerToPassiveProjectionTree(var,ppt);
                delete tuples[j];
                tuples.erase(tuples.begin()+j);
                j--;
            }
        }
    }
}

void DependencySet::removeSemanticallyContainedTuple(PassiveProjectionTree* ppt) {
    for (unsigned i=0; i<tuples.size(); i++) {
        for (unsigned j=0; j<tuples.size(); j++) {
            if (j!=i) {
                if (tuples[j]->isSemanticallyContainedIn(tuples[i])) {
                    tuples[j]->registerToPassiveProjectionTree(var,ppt);
                    delete tuples[j];
                    tuples.erase(tuples.begin()+j);
                    if (j>=i) {
                        j--;
                    } else {
                        i--;
                        if (i<tuples.size()) {
                            j--;                            
                        } else {                            
                            j = tuples.size();    
                        }                        
                    }
                }
            }
        }
    }
}

void DependencySet::print() {
    for (unsigned i=0; i<tuples.size(); i++) {
        tuples[i]->print();
        if (i!=tuples.size()-1) {
            cout << ", " << endl << "           ";
        }
    }
}
