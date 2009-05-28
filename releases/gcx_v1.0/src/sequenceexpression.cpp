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
#include "sequenceexpression.h"
#include "varstepexpression.h"
#include "forexpression.h"

SequenceExpression::SequenceExpression(Expression* _left, Expression* _right)
    : Expression(e_typesequence) {
    exps.push_back(_left);
    exps.push_back(_right);
}

SequenceExpression::~SequenceExpression() {

    for (unsigned i=0; i<exps.size(); i++) {
		//cout << *(exps[i]) << endl;
        delete exps[i];
    }
}

void SequenceExpression::print(ostream& o) const {

    o << "(" << endl;
    pp->increaseTab();
    for (unsigned i=0; i<exps.size(); i++) {
        pp->printTabs();    
        o << (*exps[i]);
        if (i<exps.size()-1)
            o << ",";
        o << endl;
    }
    pp->decreaseTab();
    pp->printTabs();
    o << ")";

}

bool SequenceExpression::scopeCheck(vector<unsigned>& def_vars,
									vector<unsigned>& introduced_vars) {
    bool ret=true;
    for (unsigned i=0; i<exps.size(); i++) {
		vector<unsigned> def_vars_cpy=def_vars;
        ret&=exps[i]->scopeCheck(def_vars_cpy,introduced_vars);
    }
    return ret;
}

void SequenceExpression::replaceVarId(unsigned old_id,unsigned new_id) {
	for (unsigned i=0; i<exps.size(); i++) {
		exps[i]->replaceVarId(old_id,new_id);
	}
}

void SequenceExpression::mergeSequences() {

    // flatten subexpressions
    for (unsigned i=0; i<exps.size(); i++) {
        exps[i]->mergeSequences();
    }
    
    vector<Expression*> new_exps;
    for (unsigned i=0; i<exps.size(); i++) {
        if (exps[i]->getType()==e_typesequence) {
            SequenceExpression* se_sub=
                (SequenceExpression*) exps[i];
            vector<Expression*>* sub=se_sub->getExps();
            for (unsigned i=0; i<sub->size(); i++) {
                new_exps.push_back((*sub)[i]);
            }
        } else {
            new_exps.push_back(exps[i]);
        }
    }

    exps=new_exps;
}

void SequenceExpression::rewriteVarsteps() {

	for (unsigned i=0; i<exps.size(); i++) {
		if (exps[i]->getType()==e_typevarstep) {
			// take care: we do not rewrite node() and text() expressions
			if (((VarStepExpression*)exps[i])->getPathStep()->getType()
				==e_typepathstep) {
		        VarStepExpression* sub=((VarStepExpression*)exps[i])->clone();
				delete exps[i];
	        	VarExpression* tmpvar=
	            	new VarExpression(VarName::getInstance()->getFreshVarname(),
										true);

	        	ForExpression* forexp=new ForExpression(tmpvar,
														sub,
														tmpvar->clone());
				exps[i]=forexp;
			}
		} else {
			exps[i]->rewriteVarsteps();
		}
	}

}

void SequenceExpression::extractProjectionTree(ProjectionTree* pt) {
    for (unsigned i=0; i<exps.size(); i++) {
        exps[i]->extractProjectionTree(pt);
    }
}

void SequenceExpression::extractPathEnvironment(PathEnvironment* penv) {
    for (unsigned i=0; i<exps.size(); i++) {
        exps[i]->extractPathEnvironment(penv);
    }
}

void SequenceExpression::eval(Environment* env, unsigned modus) {

    // forward evaluation request in given modus
    for (unsigned i=0; i<exps.size(); i++) {
        exps[i]->eval(env,modus);
    }
}

void SequenceExpression::extractFSAMap(FSAMap* fsamap,unsigned parent_var) {
    for (unsigned i=0; i<exps.size(); i++) {
        exps[i]->extractFSAMap(fsamap,parent_var);
    }
}

Expression* SequenceExpression::placeUpdates(
        vector<UpdateExpression*>& updates) {  
    for (unsigned i=0; i<exps.size(); i++) {
        exps[i]=exps[i]->placeUpdates(updates);
    }
    return this;
}

bool SequenceExpression::subsumesVarInAllBranches(unsigned var) {
	bool subsumes_all=true;
	for (unsigned i=0; i<exps.size() && subsumes_all; i++) {
		subsumes_all&=exps[i]->subsumesVarInAllBranches(var);
	}
	return subsumes_all;
}

