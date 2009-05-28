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
#include "pathexpression.h"

PathExpression::PathExpression() 
    : Expression(e_typepath) {
}

PathExpression::PathExpression(PathExpression* p, PathStepExpression* ps) 
    : Expression(e_typepath) {

    // instead of creating new path step expression,
    // we use references to existing path steps
    vector<PathStepExpression*> _pathsteps=p->getPathSteps();
    for (unsigned i=0; i<_pathsteps.size(); i++) {
        pathsteps.push_back(_pathsteps[i]);
    }

    if (ps!=NULL)
        pathsteps.push_back(ps);
}

PathExpression::~PathExpression() {
    // as the PathStepExpression* elements of the vector point
    // to existing path steps in the query, there is no need
    // to delete them here (they will be deleted when deleting
    // the VarStepExpression* objects in the query)
}

bool PathExpression::isEmptyPath() {
    return pathsteps.size()==0;
}

bool PathExpression::isSinglePath() {
    return pathsteps.size()==1;
}

PathStepExpression* PathExpression::getPrefixPathStep() {
    return pathsteps[0];
}

bool PathExpression::matchesPrefixPathStep(PathStepExpression* ps) {
    if (pathsteps.size()==0) {
        return ps->isEmpty();
    } else {
        return pathsteps[0]->getAxisType()==ps->getAxisType() &&
            pathsteps[0]->getNodeTest()==ps->getNodeTest();
    }
}

/* returns true if path size >= 1 */
bool PathExpression::removePrefixPathStep() {
    vector<PathStepExpression*>::iterator it=pathsteps.begin();
    pathsteps.erase(it,it+1);
    return pathsteps.size()>0;
}

void PathExpression::print(ostream& o) const {
    for (unsigned i=0; i<pathsteps.size(); i++) {
        o << *(pathsteps[i]);
    }
}
