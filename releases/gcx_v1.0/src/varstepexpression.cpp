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
#include "varstepexpression.h"
#include "pathenvironment.h"
#include "streampreprocessor.h"
#include "bufferiteration.h"
#include "error.h"

VarStepExpression::VarStepExpression()
	: Expression(e_typevarstep), var(0), pathstep(NULL) {}

VarStepExpression::VarStepExpression(VarExpression* _var,
                                        PathStepExpression* _pathstep)
    : Expression(e_typevarstep), var(_var), pathstep(_pathstep) {}

VarStepExpression::~VarStepExpression() {
    delete var;
    delete pathstep;
}

void VarStepExpression::print(ostream& o) const {
    o << (*var) << (*pathstep);
}

bool VarStepExpression::scopeCheck(vector<unsigned>& def_vars,
									vector<unsigned>& introduced_vars) {
    return var->scopeCheck(def_vars,introduced_vars);
}

void VarStepExpression::replaceVarId(unsigned old_id,unsigned new_id) {
	var->replaceVarId(old_id,new_id);
}

void VarStepExpression::extractProjectionTree(ProjectionTree* pt) {

	// all non text() or non node() varstep expression should have been
	// eliminated in the rewriting phase
	if (pathstep->getType()==e_typepathstep) {
		// although we could handle this case, we just ignore it: 
		// it will never happen when applying optimizations
	
		Error::getInstance()->throwError(	
			"Pure varstep expression in rewritten query. Something is wrong.");
	} else {
		pt->createDOSStarChildNode(
						var->getId(),
						RoleList::getInstance()->
					    	freshRole(var->getId(),ROLETYPE_NONEXISTENCE,NULL),
					    e_out);
	}
}

bool VarStepExpression::subsumesVarInAllBranches(unsigned _var) {
	return var->getId()==_var;
}

void VarStepExpression::eval(Environment* env, unsigned modus) {
	if (pathstep->getType()==e_typepathstep) {
		Error::getInstance()->throwError(	
			"Pure varstep expression in rewritten query. Something is wrong.");
	} else {

	    // don't care about non-evaluation modi
    	if (modus!=Expression::EVAL_QUERY)
        	return;

	    // create matching iterator
	    BufferIterator* bit=BufferIteration::createBufferIterator(
										env->getVar(var->getId()),pathstep);
	    BufferNode* cur=bit->next();

	    while (cur) {

			if (cur->type==TYPE_TAG) {
				while (!cur->isClosed()) {
					StreamPreProcessor::getInstance()->readNext();
				}
			}
			
	        cur->print();
	        cur=bit->next();
	    }
	    delete bit;
	}
}

VarStepExpression* VarStepExpression::clone() {
	VarStepExpression* vs=new VarStepExpression();
	vs->setVar(var->clone());
	vs->setPathStep(pathstep->clone());
	return vs;
}
