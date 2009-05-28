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
 * 	@brief Implementations of header file existscondexpression.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file existscondexpression.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "existscondexpression.h"

ExistsCondExpression::ExistsCondExpression(VarExpression* _var) :
CondExpression(ect_exists), var(_var), varstep(NULL) {}

ExistsCondExpression::ExistsCondExpression(VarStepExpression* _varstep) :
CondExpression(ect_exists), var(NULL), varstep(_varstep) {}

ExistsCondExpression::~ExistsCondExpression() {
	delete var;	
	delete varstep;	
}

bool ExistsCondExpression::scopeCheck(vector<unsigned>& def_vars,
                                      vector<unsigned>& introduced_vars) {
	if (var) {
		return var->scopeCheck(def_vars, introduced_vars);
	} else {
		return varstep->scopeCheck(def_vars, introduced_vars);	
	}    
}

void ExistsCondExpression::replaceVarId(unsigned old_id, unsigned new_id) {
	if (var) {
		var->replaceVarId(old_id, new_id);
	} else {
		varstep->replaceVarId(old_id, new_id);	
	}    
}

void ExistsCondExpression::extractDependencies(vector<DependencySet*>* depset) {
	if (varstep) {
		PathExpression* p = varstep->getPath();
		unsigned var = varstep->getVar()->getId();

		for (unsigned i=0; i<depset->size(); i++) {
			if ((*depset)[i]->getVar()==var) {
				(*depset)[i]->insertTuple(p, true, false, true);
				break;
			}
		}
	}
}

void ExistsCondExpression::print(ostream& o) const {
	if (var) {
		o << "fn:exists(" << (*var) << ")";	
	} else {
		o << "fn:exists(" << (*varstep) << ")";	
	}    
}

bool ExistsCondExpression::evalCond(Environment* env, unsigned modus) {

	switch (modus) {
		case EVAL_QUERY:
		{
			BufferIterator* bit=var?
				new BufferIterator(env->getNodeBinding(var->getId()),NULL):
				new BufferIterator(env->getNodeBinding(varstep->getVar()->getId()),varstep->getPath());
			
			BufferNode* cur=bit->getNext();
			delete bit;
		
			return cur?true:false;
		}

		case EVAL_QUERY_SILENT:
			Error::getInstance()->throwError("ExistsCondExpression: Illegal Evaluation Mode.");
			break;

		case EVAL_SIGNOFF: // nothing to do here
			break;
	}

	return false; // never reached
}
