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
#include "nodeconstructexpression.h"
#include "varstepexpression.h"
#include "forexpression.h"

NodeConstructExpression::NodeConstructExpression(const char* _tag, 
                                                    Expression* _exp)
    : Expression(e_typenodeconstr), 
        tag(tagmap->insertTag(_tag)), 
        exp(_exp) {
}

NodeConstructExpression::~NodeConstructExpression() {
    delete exp;
}

void NodeConstructExpression::print(ostream& o) const {

    // special case: empty subexpression
    EXP_TYPE subtype=exp->getType();
    if (subtype==e_typeempty) {
        o << "<" << tagmap->getTag(tag) << "/>";
        return;
    }
    
    // nonempty subexpressions
    o << "<" << tagmap->getTag(tag) << ">{";
    if (subtype==e_typefor || subtype==e_typeif 
        || subtype==e_typenodeconstr
        || subtype==e_typesequence
        || subtype==e_typeupdate) {
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

bool NodeConstructExpression::scopeCheck(vector<unsigned>& def_vars,
											vector<unsigned>& introduced_vars) {
    return exp->scopeCheck(def_vars,introduced_vars);
}

void NodeConstructExpression::replaceVarId(unsigned old_id,unsigned new_id) {
	exp->replaceVarId(old_id,new_id);
}

void NodeConstructExpression::mergeSequences() {
    exp->mergeSequences();
}

void NodeConstructExpression::rewriteVarsteps() {
	if (exp->getType()==e_typevarstep) {
		// take care: we do not rewrite node() and text() expressions
		if (((VarStepExpression*)exp)->getPathStep()->getType()
			==e_typepathstep) {
	        VarStepExpression* sub=((VarStepExpression*)exp)->clone();
			delete exp;
	        VarExpression* tmpvar=
	            new VarExpression(VarName::getInstance()->getFreshVarname(),
									true);
	        ForExpression* forexp=new ForExpression(tmpvar,
													sub,
													tmpvar->clone());
	        exp=forexp;
		}
	} else {
		exp->rewriteVarsteps();
	}
}

void NodeConstructExpression::extractProjectionTree(ProjectionTree* pt) {
    exp->extractProjectionTree(pt);
}

void NodeConstructExpression::extractPathEnvironment(PathEnvironment* penv) {
	exp->extractPathEnvironment(penv);
}

bool NodeConstructExpression::subsumesVarInAllBranches(unsigned var) {
	return exp->getType()!=e_typeempty && exp->subsumesVarInAllBranches(var);
}

void NodeConstructExpression::eval(Environment* env, unsigned modus) {

    // we don't care about the update
    if (modus==Expression::EVAL_UPD_IN_SEQ)
        return;

    if (modus==Expression::EVAL_QUERY) {
      cout << "<" << tagmap->getTag(tag) << ">";
        exp->eval(env,Expression::EVAL_QUERY);
        cout << "</" << tagmap->getTag(tag) << ">";
    } else { // modus==Expression::EVAL_UPD
        exp->eval(env,Expression::EVAL_UPD);
    }
}


void NodeConstructExpression::extractFSAMap(
        FSAMap* fsamap,unsigned parent_var) {
    exp->extractFSAMap(fsamap,parent_var);
}

Expression* NodeConstructExpression::placeUpdates(
        vector<UpdateExpression*>& updates) {
    exp=exp->placeUpdates(updates);
    return this;
}
