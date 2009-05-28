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
#include "andcondexpression.h"

AndCondExpression::AndCondExpression(CondExpression* _left,CondExpression* _right)
    : CondExpression(e_ctypeand), left(_left), right(_right) {}

AndCondExpression::~AndCondExpression() {
    delete left;
    delete right;
}

void AndCondExpression::print(ostream& o) const {
    EXP_TYPE l=left->getType();
    if (l==e_ctypeor || l==e_ctypeand || l==e_ctypenot) {
        o << "(" << (*left) << ")";
    } else {
        o << (*left);
    }
    o << " and ";
    EXP_TYPE r=right->getType();
    if (r==e_ctypeor || r==e_ctypeand || r==e_ctypenot) {
        o << "(" << (*right) << ")";
    } else {
        o << (*right);
    }
}

bool AndCondExpression::scopeCheck(vector<unsigned>& def_vars,
									vector<unsigned>& introduced_vars) {
    return left->scopeCheck(def_vars,introduced_vars)  
        && right->scopeCheck(def_vars,introduced_vars);
}

void AndCondExpression::replaceVarId(unsigned old_id,unsigned new_id) {
	left->replaceVarId(old_id,new_id);
	right->replaceVarId(old_id,new_id);
}

void AndCondExpression::extractProjectionTree(ProjectionTree* pt) {
	left->extractProjectionTree(pt);
	right->extractProjectionTree(pt);
}

void AndCondExpression::extractPathEnvironment(PathEnvironment* penv) {
	left->extractPathEnvironment(penv);
	right->extractPathEnvironment(penv);
}

bool AndCondExpression::subsumesVarInAllBranches(unsigned var) {
	return left->subsumesVarInAllBranches(var) &&
			right->subsumesVarInAllBranches(var);
}
 

bool AndCondExpression::evalCond(Environment* env) {
    return left->evalCond(env) && right->evalCond(env);
}
