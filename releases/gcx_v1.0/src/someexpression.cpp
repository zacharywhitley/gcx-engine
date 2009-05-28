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
#include "someexpression.h"
#include "pathenvironment.h"
#include "buffernode.h"
#include "sequenceexpression.h"
#include "streampreprocessor.h"
#include "bufferchilditerator.h"
#include "bufferdescendantiterator.h"
#include "error.h"

SomeExpression::SomeExpression(VarExpression* _var,
    VarStepExpression* _varstep,CondExpression* _cond_exp) 
    : CondExpression(e_typesome), var(_var),varstep(_varstep),cond_exp(_cond_exp) {}

SomeExpression::~SomeExpression() {
    delete var;
    delete varstep;
    delete cond_exp;
}

void SomeExpression::print(ostream& o) const {
	o << "some " << (*var)
	  << " in " << (*varstep)
	  << " satisfies (" << (*cond_exp) << ")";
}

bool SomeExpression::scopeCheck(vector<unsigned>& def_vars,
								vector<unsigned>& introduced_vars) {
	vector<unsigned> new_def_vars=def_vars;

	// check for variable name having been introduced yet ...
	bool varname_in_use=false;
	for (unsigned i=0; i<introduced_vars.size() && !varname_in_use; i++) {
		varname_in_use|=
			strcmp(VarName::getInstance()->getVar(introduced_vars[i]),
					VarName::getInstance()->getVar(var->getId()))==0;
	}

	// ... and replace the id if necessary
	if (varname_in_use) {
		unsigned old_id=var->getId();
		const char* var_name=VarName::getInstance()->getVar(old_id);
		unsigned new_id=VarName::getInstance()->addVar(strdup(var_name),true);
		var->setId(new_id);
		cond_exp->replaceVarId(old_id,new_id);
		new_def_vars.push_back(new_id);
	} else {
		// strictly speaking, the following should only be applied to
		// the "condexp" part of the final return statement, but the change
		// does not affect varstep expressions, so nothing will go wrong
		introduced_vars.push_back(var->getId());
		new_def_vars.push_back(var->getId());
	}
	
	return varstep->scopeCheck(def_vars,introduced_vars)
		&& cond_exp->scopeCheck(new_def_vars,introduced_vars);
}

void SomeExpression::replaceVarId(unsigned old_id,unsigned new_id) {

	// the varstep expression still belongs to the old scope
	varstep->replaceVarId(old_id,new_id);

    // a new scoping area may be created for exp, take special care
	if (strcmp(VarName::getInstance()->getVar(old_id),
				VarName::getInstance()->getVar(var->getId()))!=0) {
		cond_exp->replaceVarId(old_id,new_id);
	}

}

void SomeExpression::extractProjectionTree(ProjectionTree* pt) {
		
    PathStepExpression* ps=varstep->getPathStep();
	if (ps->getType()==e_typepathstep) {
	
	    // we only add a role if the introduced variable is not subsumed
	    // in all branches (builtin optimization)
	    Role* role=cond_exp->subsumesVarInAllBranches(var->getId())?
			NULL:RoleList::getInstance()->freshRole(var->getId(),
													ROLETYPE_VARIABLE,
													NULL);
		
	    pt->createChildNode(varstep->getVar()->getId(),
								var->getId(),
	                   	 		ps->getAxisType(),
	                   	 		ps->getNodeTest(),
	                    		false,
	                    		false,
								e_dom,
								role);
	} else {
		unsigned var_id=varstep->getVar()->getId();
		pt->createDOSStarChildNode(var_id,
							RoleList::getInstance()->
								freshRole(var_id,ROLETYPE_NONEXISTENCE,NULL),
							e_out);
	}

		
	cond_exp->extractProjectionTree(pt);
}

void SomeExpression::extractPathEnvironment(PathEnvironment* penv) {

	// this is the path to the current var binding 
	PathExpression* p=new PathExpression(
								penv->getPath(varstep->getVar()->getId()),
								varstep->getPathStep());
	penv->addPath(var->getId(),p);

	cond_exp->extractPathEnvironment(penv);
}
	
bool SomeExpression::evalCond(Environment* env) {
	Error::getInstance()->throwError(
		"Some expression not yet implemented");
	return false; // TO BE DONE
}

void SomeExpression::extractFSAMap(FSAMap* fsamap,unsigned parent_var) {
	unsigned var_id=var->getId();
	unsigned parent_var_id=varstep->getVar()->getId();

	if (parent_var_id==parent_var) {
		if (fsamap->getFSA(parent_var)==parent_var) {
			fsamap->addFSA(var_id,var_id); // the var is FSA of itself
		} else {
			fsamap->addFSA(var_id,fsamap->getFSA(parent_var));
		}
	} else {
		fsamap->addFSA(var_id,fsamap->getFSA(parent_var_id));
	}

	// step inside
	cond_exp->extractFSAMap(fsamap,var_id);
}

Expression* SomeExpression::placeUpdates(vector<UpdateExpression*>& updates) {
	// TODO (THIS IS CRUCIAL...)
	// How to implement?
	// Proposal: introduce an additional list of updates...
	return this;
}

bool SomeExpression::subsumesVarInAllBranches(unsigned _var) {
	return varstep->getVar()->getId()==_var ||
	cond_exp->subsumesVarInAllBranches(_var);
}
