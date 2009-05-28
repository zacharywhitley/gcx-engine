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
 * 	@brief Implementations of header file role.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file role.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "role.h"
#include "pathexpression.h"
#include "signoffexpression.h"

Role::Role(unsigned _id, ROLE_TYPE _type, unsigned _basing_var,
           unsigned _basing_fsa, PathExpression* _rel_path, PathExpression* _var_path) :
id(_id), type(_type), basing_var(_basing_var), basing_fsa(_basing_fsa),
rel_path(_rel_path), var_path(_var_path) {
	
	var_path = var_path?var_path->clone():NULL;
	
	concat_path = rel_path?rel_path->clone():new PathExpression();
	if (var_path) {
		vector<PathStepExpression*>* ps = var_path->getPathSteps();
		for (unsigned i=0; i<ps->size(); i++) {
			concat_path->addPathStep((*ps)[i]->clone());
		}
	}
}

Role::~Role() {
	delete rel_path;
	delete var_path;
	delete concat_path;
}

bool Role::isPosRole() {
	return var_path && var_path->getTailPathStep()->hasAttribute() && var_path->getTailPathStep()->getAttribute()->getType()==at_position;
}

bool Role::isDosRole() {
	return var_path && var_path->getTailPathStep()->isDosNodeStep();
}

SignOffExpression* Role::getSignOffExpression() {
    VarExpression* var = new VarExpression(true);
    var->setId(basing_fsa);
    
    if (concat_path && !concat_path->isEmptyPath()) {
    	return new SignOffExpression(new VarStepExpression(var, concat_path->clone()), this);
    } else {
    	return new SignOffExpression((VarExpression*) var, this);
    }
}

void Role::print(ostream& o) const {
	o << "r" << id;
}

void Role::printExtended() {
    cout << "role=r" << id << ", base_var=$"
    		<< VarName::getInstance()->getVarname(basing_var) << ", base_fsa=$"
			<< VarName::getInstance()->getVarname(basing_fsa);
    (rel_path==NULL || rel_path->isEmptyPath())?
		cout << ", rel_path=%":cout << ", rel_path=" << *(rel_path);
    (var_path==NULL || var_path->isEmptyPath())?
		cout << ", var_path=%" : cout << ", var_path=" << *(var_path);
}
