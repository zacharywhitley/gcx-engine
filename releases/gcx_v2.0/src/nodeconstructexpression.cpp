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
 * 	@brief Implementations of header file nodeconstructexpression.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file nodeconstructexpression.h.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "nodeconstructexpression.h"

NodeConstructExpression::NodeConstructExpression(const char* _tag, Expression* _exp) :
Expression(et_nodeconstr), tag(tagmap->insertTag(_tag)), exp(_exp) {
}

NodeConstructExpression::~NodeConstructExpression() {
    delete exp;
}

bool NodeConstructExpression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars) {
    return exp->scopeCheck(def_vars, introduced_vars);
}

void NodeConstructExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    exp->replaceVarId(old_id, new_id);
}

void NodeConstructExpression::mergeSequences() {
    exp->mergeSequences();
}

void NodeConstructExpression::rewriteVarsteps() {
    if (exp->getType()==et_varstep) {
        VarStepExpression* sub=((VarStepExpression*)exp)->clone();
        delete exp;
		const char* vn=VarName::getInstance()->getFreshVarname();
        VarExpression* tmpvar=new VarExpression(vn,true,true);
        ForExpression* forexp=new ForExpression(tmpvar,sub,tmpvar->clone());
        exp=forexp;
    } else {
        exp->rewriteVarsteps();
    }
}

void NodeConstructExpression::extractFSAMap(FSAMap* fsamap, unsigned parent_var) {
    exp->extractFSAMap(fsamap, parent_var);
}

void NodeConstructExpression::extractParVarMap(ParVarMap* parvarmap) {
    exp->extractParVarMap(parvarmap);
}

void NodeConstructExpression::extractDependencies(vector<DependencySet*>* depset) {
    exp->extractDependencies(depset);
}

Expression* NodeConstructExpression::placeSignOffs(vector<SignOffExpression*>& signoffs) {
    exp=exp->placeSignOffs(signoffs);
    return this;
}

void NodeConstructExpression::rewriteAggregateFuncts() {
	exp->rewriteAggregateFuncts();
}

void NodeConstructExpression::print(ostream& o) const {
    EXP_TYPE subtype=exp->getType();
    if (subtype==et_empty) {
        o << "<" << tagmap->getTag(tag) << "/>";
        return;
    }
    o << "<" << tagmap->getTag(tag) << ">{";
    if (subtype==et_for || subtype==et_if || subtype==et_nodeconstr || subtype==et_sequence || subtype==et_signoff) {
        o << endl;
        pp->increaseTab();
        pp->printTabs();
        o << (*exp) << endl;
        pp->decreaseTab();
        pp->printTabs();
    } else {
        o << (*exp);
    }
    o << "}</" << tagmap->getTag(tag) << ">";
}

void* NodeConstructExpression::eval(Environment* env, unsigned modus) {

	switch (modus) {

		case EVAL_QUERY:
			if (exp->getType()==et_empty) {
				cout << "<" << tagmap->getTag(tag) << "/>";
			} else {
				cout << "<" << tagmap->getTag(tag) << ">";
				exp->eval(env,EVAL_QUERY);
				cout << "</" << tagmap->getTag(tag) << ">";
			}
			break;

		case EVAL_QUERY_SILENT:
			Error::getInstance()->throwError("NodeConstructExpression: Illegal Evaluation Mode.");
			break;

		case EVAL_SIGNOFF:
        	exp->eval(env,EVAL_SIGNOFF);
			break;
    }

	return NULL;
}
