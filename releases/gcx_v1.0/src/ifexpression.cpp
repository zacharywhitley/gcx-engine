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
#include "ifexpression.h"
#include "updateexpression.h"
#include "sequenceexpression.h"
#include "varstepexpression.h"
#include "forexpression.h"

IfExpression::IfExpression(CondExpression* _cond,
                    Expression* _thenexp, 
                Expression* _elseexp) 
    : Expression(e_typeif), cond(_cond), thenexp(_thenexp), 
    elseexp(_elseexp) {}

IfExpression::~IfExpression() {
    delete cond;
    delete thenexp;
    delete elseexp;
}

void IfExpression::print(ostream& o) const {
    o << "if (" << (*cond) << ")" << endl;
    pp->increaseTab();
    pp->printTabs();
    o << "then " << endl;
    pp->increaseTab();
    pp->printTabs();
    o << (*thenexp) << endl;
    pp->decreaseTab();
    pp->printTabs();
    o << "else " << endl;
    pp->increaseTab();
    pp->printTabs();
    o << (*elseexp);
    pp->decreaseTab();
    pp->decreaseTab();
}

bool IfExpression::scopeCheck(vector<unsigned>& def_vars,
								vector<unsigned>& introduced_vars) {
    return cond->scopeCheck(def_vars,introduced_vars)
        && thenexp->scopeCheck(def_vars,introduced_vars)
        && elseexp->scopeCheck(def_vars,introduced_vars);
}

void IfExpression::replaceVarId(unsigned old_id,unsigned new_id) {
	cond->replaceVarId(old_id,new_id);
	thenexp->replaceVarId(old_id,new_id);
	elseexp->replaceVarId(old_id,new_id);
}

void IfExpression::mergeSequences() {
    thenexp->mergeSequences();
    elseexp->mergeSequences();
}

void IfExpression::rewriteVarsteps() {

	if (thenexp->getType()==e_typevarstep) {
		// take care: we do not rewrite node() and text() expressions
		if (((VarStepExpression*)thenexp)->getPathStep()->getType()
				==e_typepathstep) {
			VarStepExpression* sub=((VarStepExpression*)thenexp)->clone();
			delete thenexp;
   	    	VarExpression* tmpvar=
            	new VarExpression(VarName::getInstance()->getFreshVarname(),
									true);
        	ForExpression* forexp=new ForExpression(tmpvar,sub,tmpvar->clone());
			thenexp=forexp;
		}
	} else {
		thenexp->rewriteVarsteps();
	}

	if (elseexp->getType()==e_typevarstep) {
		// take care: we do not rewrite node() and text() expressions
		if (((VarStepExpression*)elseexp)->getPathStep()->getType()
				==e_typepathstep) {
			VarStepExpression* sub=((VarStepExpression*)elseexp)->clone();
			delete elseexp;
	        VarExpression* tmpvar=
	            new VarExpression(VarName::getInstance()->getFreshVarname(),
									true);
	        ForExpression* forexp=new ForExpression(tmpvar,sub,tmpvar->clone());
			elseexp=forexp;
		}
	} else {
		elseexp->rewriteVarsteps();
	}
}

void IfExpression::extractProjectionTree(ProjectionTree* pt) {
    cond->extractProjectionTree(pt);
    thenexp->extractProjectionTree(pt);
    elseexp->extractProjectionTree(pt);
}

void IfExpression::extractPathEnvironment(PathEnvironment* penv) {
	cond->extractPathEnvironment(penv);
	thenexp->extractPathEnvironment(penv);
	elseexp->extractPathEnvironment(penv);
}

bool IfExpression::subsumesVarInAllBranches(unsigned var) {
	return (cond->subsumesVarInAllBranches(var) &&
			thenexp->subsumesVarInAllBranches(var) &&
			elseexp->subsumesVarInAllBranches(var));
}

void IfExpression::eval(Environment* env, unsigned modus) {

    // we don't care about the update expression
    if (modus==Expression::EVAL_UPD_IN_SEQ)
        return;
	
    if (modus==Expression::EVAL_QUERY) {

        if (cond->evalCond(env)) {
            thenexp->eval(env,Expression::EVAL_QUERY);
            elseexp->eval(env,Expression::EVAL_UPD);
        } else {
            elseexp->eval(env,Expression::EVAL_QUERY);
            thenexp->eval(env,Expression::EVAL_UPD);
        }
    } else { // modus==Expression::EVAL_UPD
        thenexp->eval(env,Expression::EVAL_UPD);
        elseexp->eval(env,Expression::EVAL_UPD);
    }
}

void IfExpression::extractFSAMap(FSAMap* fsamap,unsigned parent_var) {
    thenexp->extractFSAMap(fsamap,parent_var);
    elseexp->extractFSAMap(fsamap,parent_var);
}

Expression* IfExpression::placeUpdates(vector<UpdateExpression*>& updates) {
    vector<unsigned> then_ivs;
    vector<unsigned> else_ivs;
    
    thenexp=thenexp->placeUpdates(updates);
    elseexp=elseexp->placeUpdates(updates);
    return this;
}
