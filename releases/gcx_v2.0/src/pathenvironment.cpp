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
 * 	@brief Implementations of header file pathenvironment.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file pathenvironment.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "pathenvironment.h"

PathEnvironment::PathEnvironment() {
    env[TAGID_ROOT]=new PathExpression();
    ParVarMap* parvarmap = ParVarMap::getInstance();
    vector<unsigned>* varsq = parvarmap->getVarsq();
    PathExpression* p;
    unsigned parvar;
    vector<PathExpression*> insert_paths;
    vector<PathStepExpression*>* insert_steps;
    for (unsigned i=1; i<varsq->size(); i++) { // omit root variable!
        insert_paths.clear();
        p = new PathExpression();
        if (parvarmap->getPath((*varsq)[i])) {
        	insert_paths.push_back(parvarmap->getPath((*varsq)[i]));
        }
        
        parvar = parvarmap->getParVar((*varsq)[i]);
        while (parvar!=TAGID_ROOT) {
        	if (parvarmap->getPath(parvar)) {
        		insert_paths.push_back(parvarmap->getPath(parvar));
        	}
            parvar = parvarmap->getParVar(parvar);
        }
        
        for (int j=insert_paths.size()-1; j>=0; j--) {
            insert_steps = insert_paths[j]->getPathSteps();
            for (unsigned k=0; k<insert_steps->size(); k++) {
                p->addPathStep((*insert_steps)[k]->clone());
            }
        }
        
        insertPath((*varsq)[i], p);
    }
}

PathEnvironment::~PathEnvironment() {
    PEnv::iterator it=env.begin();
    while (it!=env.end()) {
        delete it->second;
        env.erase(it);
        it=env.begin();
    }
}

void PathEnvironment::insertPath(unsigned var, PathExpression* path) {
    env[var]=path;
}

PathExpression* PathEnvironment::getPath(unsigned var) {
    PEnv::iterator it=env.find(var);
    return it->second;
}

void PathEnvironment::print() {
    PEnv::iterator it;
    for (it=env.begin(); it!=env.end(); it++) {
        cout << "$" << VarName::getInstance()->getVarname(it->first) << " -> ";
        if (!it->second->isEmptyPath()) {
            cout << *(it->second);
        } else {
            cout << "%";
        }
        cout << endl;
    }
}

PathExpression* PathEnvironment::getPathBetween(unsigned end, unsigned start) {
    PathExpression* p=new PathExpression();

    PathExpression* end_path=getPath(end);
    for (unsigned i=getPath(start)->getPathSteps()->size();
					i<end_path->getPathSteps()->size(); i++) {
        p->addPathStep(end_path->getPathStepAt(i)->clone());
    }

    return p;
}
