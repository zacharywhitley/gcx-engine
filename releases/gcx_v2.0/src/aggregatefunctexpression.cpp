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
 * 	@brief Implementations of header file aggregatefunctexpression.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file aggregatefunctexpression.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "aggregatefunctexpression.h"

AggregateFunctExpression::AggregateFunctExpression(EXP_TYPE _type, VarExpression* _var) :
Expression(_type), var(_var), varstep(NULL), forexp(NULL) {}

AggregateFunctExpression::AggregateFunctExpression(EXP_TYPE _type, VarStepExpression* _varstep) :
Expression(_type), var(NULL), varstep(_varstep), forexp(NULL) {}

AggregateFunctExpression::~AggregateFunctExpression() {
	delete var;
	delete varstep;
	delete forexp;
}

bool AggregateFunctExpression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars) {
	if (var) {
		return var->scopeCheck(def_vars, introduced_vars);
	} else if (varstep) {
		return varstep->scopeCheck(def_vars, introduced_vars);
	} else if (forexp) {
		return forexp->scopeCheck(def_vars, introduced_vars);
	}
	return true;
}

void AggregateFunctExpression::replaceVarId(unsigned old_id, unsigned new_id) {
	if (var) {
		var->replaceVarId(old_id, new_id);
	} else if (varstep) {
		varstep->replaceVarId(old_id, new_id);
	} else if (forexp) {
		forexp->replaceVarId(old_id, new_id);
	}	
}

void AggregateFunctExpression::extractFSAMap(FSAMap* fsamap, unsigned parent_var) {
	if (forexp) {
		forexp->extractFSAMap(fsamap, parent_var);
	}
}

void AggregateFunctExpression::extractParVarMap(ParVarMap* parvarmap) {
	if (forexp) {
		forexp->extractParVarMap(parvarmap);
	}
}

void AggregateFunctExpression::extractDependencies(vector<DependencySet*>* depset) {
	// there is no need to add a /dos::node()-dependency tuple for aggregate function fn:count()
	// because all for counting required nodes are covered by the variable node!
	if (forexp && type!=et_aggregatefunctcount) {  
		forexp->extractDependencies(depset);
	}
}

Expression* AggregateFunctExpression::placeSignOffs(vector<SignOffExpression*>& signoffs) {
	if (forexp) {
		forexp=(ForExpression*) forexp->placeSignOffs(signoffs);
	}
	return this;
}

void AggregateFunctExpression::rewriteAggregateFuncts() {
	if (var) {
		VarExpression* tmpvar=new VarExpression(VarName::getInstance()->getFreshVarname(),true,true);
		ForExpression* fexp=new ForExpression(tmpvar,var->clone(),tmpvar->clone());
		forexp=fexp;
	} else if (varstep) {
		VarExpression* tmpvar=new VarExpression(VarName::getInstance()->getFreshVarname(),true,true);
		ForExpression* fexp=new ForExpression(tmpvar,varstep->clone(),tmpvar->clone());
		forexp=fexp;
	}
}

void AggregateFunctExpression::print(ostream& o) const{
	switch (type) {
		case et_aggregatefunctsum:
			if (forexp) {
				o << "fn:sum(" << (*forexp) << ")";
			} else if (varstep) {
				o << "fn:sum(" << (*varstep) << ")";
			} else {
				o << "fn:sum(" << (*var) << ")";
			}
			break;
		case et_aggregatefunctavg:
			if (forexp) {
				o << "fn:avg(" << (*forexp) << ")";
			} else if (varstep) {
				o << "fn:avg(" << (*varstep) << ")";
			} else {
				o << "fn:avg(" << (*var) << ")";
			}
			break;
		case et_aggregatefunctmin:
			if (forexp) {
				o << "fn:min(" << (*forexp) << ")";
			} else if (varstep) {
				o << "fn:min(" << (*varstep) << ")";
			} else {
				o << "fn:min(" << (*var) << ")";
			}
			break;
		case et_aggregatefunctmax:
			if (forexp) {
				o << "fn:max(" << (*forexp) << ")";
			} else if (varstep) {
				o << "fn:max(" << (*varstep) << ")";
			} else {
				o << "fn:max(" << (*var) << ")";
			}
			break;
		case et_aggregatefunctcount:
			if (forexp) {
				o << "fn:count(" << (*forexp) << ")";
			} else if (varstep) {
				o << "fn:count(" << (*varstep) << ")";
			} else {
				o << "fn:count(" << (*var) << ")";
			}
			break;
		case et_aggregatefunctstddevsamp:
			if (forexp) {
				o << "fn:stddev_samp(" << (*forexp) << ")";
			} else if (varstep) {
				o << "fn:stddev_samp(" << (*varstep) << ")";
			} else {
				o << "fn:stddev_samp(" << (*var) << ")";
			}
			break;
		case et_aggregatefunctstddevpop:
			if (forexp) {
				o << "fn:stddev_pop(" << (*forexp) << ")";
			} else if (varstep) {
				o << "fn:stddev_pop(" << (*varstep) << ")";
			} else {
				o << "fn:stddev_pop(" << (*var) << ")";
			}
			break;
		case et_aggregatefunctvarsamp:
			if (forexp) {
				o << "fn:var_samp(" << (*forexp) << ")";
			} else if (varstep) {
				o << "fn:var_samp(" << (*varstep) << ")";
			} else {
				o << "fn:var_samp(" << (*var) << ")";
			}
			break;
		case et_aggregatefunctvarpop:
			if (forexp) {
				o << "fn:var_pop(" << (*forexp) << ")";
			} else if (varstep) {
				o << "fn:var_pop(" << (*varstep) << ")";
			} else {
				o << "fn:var_pop(" << (*var) << ")";
			}
			break;
		case et_aggregatefunctlist:
			if (forexp) {
				o << "fn:list(" << (*forexp) << ")";
			} else if (varstep) {
				o << "fn:list(" << (*varstep) << ")";
			} else {
				o << "fn:list(" << (*var) << ")";
			}
			break;
		default: // should never happen
			break;
	}
}
