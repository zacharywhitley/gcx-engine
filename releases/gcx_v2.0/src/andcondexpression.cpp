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
 * 	@brief Implementations of header file andcondexpression.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file andcondexpression.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "andcondexpression.h"

AndCondExpression::AndCondExpression(CondExpression* _left, CondExpression* _right) :
CondExpression(ect_and), left(_left), right(_right) {}

AndCondExpression::~AndCondExpression() {
    delete left;
    delete right;
}

bool AndCondExpression::scopeCheck(vector<unsigned>& def_vars,
                                   vector<unsigned>& introduced_vars) {
    return left->scopeCheck(def_vars, introduced_vars) &&
    		right->scopeCheck(def_vars, introduced_vars);
}

void AndCondExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    left->replaceVarId(old_id, new_id);
    right->replaceVarId(old_id, new_id);
}

void AndCondExpression::extractFSAMap(FSAMap* fsamap, unsigned parent_var) {
	left->extractFSAMap(fsamap, parent_var);
	right->extractFSAMap(fsamap, parent_var);
}
        
void AndCondExpression::extractParVarMap(ParVarMap* parvarmap) {
	left->extractParVarMap(parvarmap);
	right->extractParVarMap(parvarmap);
}

void AndCondExpression::extractDependencies(vector<DependencySet*>* depset) {
    left->extractDependencies(depset);
    right->extractDependencies(depset);
}

Expression* AndCondExpression::placeSignOffs(vector<SignOffExpression*>& signoffs) {
	left=(CondExpression*) left->placeSignOffs(signoffs);
	right=(CondExpression*) right->placeSignOffs(signoffs);
	return this;
}

void AndCondExpression::rewriteAggregateFuncts() {
	left->rewriteAggregateFuncts();
	right->rewriteAggregateFuncts();
}

void AndCondExpression::print(ostream& o) const {
    EXP_TYPE l=left->getType();
	if (l==ect_and || l==ect_or || l==ect_not ||
			l==ect_exists || l==ect_operand || l==ect_true || l==ect_false) {
		o << "(" << (*left) << ")";
    } else {
    	o << (*left);
    }
	o << " and ";
    EXP_TYPE r=right->getType();
    if (r==ect_and || r==ect_or || r==ect_not ||
    		r==ect_exists || r==ect_operand || r==ect_true || r==ect_false) {
    	o << "(" << (*right) << ")";
    } else {
    	o << (*right);
    }
}

void* AndCondExpression::eval(Environment* env, unsigned modus) {
	// the method is used to forward signOff execution requests
	if (modus==EVAL_SIGNOFF) {
		left->eval(env,EVAL_SIGNOFF);
		right->eval(env,EVAL_SIGNOFF);
	}

	return NULL;
}

bool AndCondExpression::evalCond(Environment* env, unsigned modus) {
	bool left_val=left->evalCond(env,modus);
	if (left_val) {
		return right->evalCond(env,modus);
	} else {
		// make sure we evaluate the update in this part
		right->evalCond(env,EVAL_SIGNOFF);
		return false;
	}
}
