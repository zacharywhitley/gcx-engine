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
#include "forexpression.h"
#include "pathenvironment.h"
#include "buffernode.h"
#include "sequenceexpression.h"
#include "streampreprocessor.h"
#include "bufferiteration.h"

ForExpression::ForExpression(VarExpression* _var,
    VarStepExpression* _varstep,Expression* _exp) 
    : Expression(e_typefor), var(_var),varstep(_varstep),exp(_exp) {}

ForExpression::~ForExpression() {
    delete var;
    delete varstep;
    delete exp;
}

void ForExpression::print(ostream& o) const {
    o << "for " << (*var) 
      << " in " 
      << (*varstep) << " return ";

    EXP_TYPE subtype=exp->getType();
    if (subtype==e_typefor || subtype==e_typeif 
        || subtype==e_typenodeconstr
        || subtype==e_typesequence
        || subtype==e_typeupdate) {
        o << endl;
        pp->increaseTab();
        pp->printTabs();
        o << (*exp);
        pp->decreaseTab();
    } else {
        o << (*exp);
    }
}

bool ForExpression::scopeCheck(vector<unsigned>& def_vars,
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
		exp->replaceVarId(old_id,new_id);
    	new_def_vars.push_back(new_id);
	} else {
		// stricly speaking, the following should only be applied to
		// the "exp" part of the final return statement, but the change
		// does not affect varstep expressions, so nothing will go wrong
		introduced_vars.push_back(var->getId());
    	new_def_vars.push_back(var->getId());
	}

    return varstep->scopeCheck(def_vars,introduced_vars)
        && exp->scopeCheck(new_def_vars,introduced_vars);
}

void ForExpression::replaceVarId(unsigned old_id,unsigned new_id) {
		
	// the varstep expression still belongs to the old scope
	varstep->replaceVarId(old_id,new_id);

	// a new scoping area may be created for exp, take special care
	if (strcmp(VarName::getInstance()->getVar(old_id),
				VarName::getInstance()->getVar(var->getId()))!=0) {
		exp->replaceVarId(old_id,new_id);
	}
}

void ForExpression::mergeSequences() {
    exp->mergeSequences();
}

void ForExpression::rewriteVarsteps() {
	if (exp->getType()==e_typevarstep) {
		// take care: we do not rewrite node() and text() expressions
		if (((VarStepExpression*)exp)->getPathStep()->getType()
				==e_typepathstep) {
			VarStepExpression* sub=((VarStepExpression*)exp)->clone();
			delete exp;
			VarExpression* tmpvar=
				new VarExpression(VarName::getInstance()->getFreshVarname(),
									true);
			ForExpression* forexp=new ForExpression(tmpvar,sub,tmpvar->clone());
			exp=forexp;
		}
	} else {
		exp->rewriteVarsteps();
	}
}

void ForExpression::extractProjectionTree(ProjectionTree* pt) {


	PathStepExpression* ps=varstep->getPathStep();
	if (ps->getType()==e_typepathstep) {

		// we only add a role if the introduced variable is not subsumed
		// in all branches (builtin optimization)
		Role* role=exp->subsumesVarInAllBranches(var->getId())?
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
	exp->extractProjectionTree(pt);
}

void ForExpression::extractPathEnvironment(PathEnvironment* penv) {

	// this is the path to the current var binding 
	PathExpression* p=new PathExpression(
								penv->getPath(varstep->getVar()->getId()),
								varstep->getPathStep());
	penv->addPath(var->getId(),p);

	exp->extractPathEnvironment(penv);
}

bool ForExpression::subsumesVarInAllBranches(unsigned _var) {
	/*
	return varstep->getVar()->getId()==_var ||
			exp->subsumesVarInAllBranches(_var);
	*/
	return false;
}

void ForExpression::eval(Environment* env, unsigned modus) {

    // we don't care about the update expression
    if (modus==Expression::EVAL_UPD_IN_SEQ)
        return;

    // create matching iterator
    BufferIterator* bit=BufferIteration::createBufferIterator(
									env->getVar(varstep->getVar()->getId()),
									varstep->getPathStep());
	BufferNode* cur=bit->next();
    
    // query evaluation
    if (modus==Expression::EVAL_QUERY) {
        
        while (cur) {
				
            cur->lock(); // we're not allowed to delete this one
			
			/*
			cout << endl << endl;
			cout << "MATCH IS:" << endl;
			cur->debugPrint();
			cout << endl << endl;
			*/

            // read one match in advance
            // this avoids crashing when the current token is deleted
            Environment* newenv=env->clone();
            newenv->addVar(var->getId(),cur);
            exp->eval(newenv,Expression::EVAL_QUERY);

            // now we have to proceed the pointer, as we need to avoid
            // a crash when sending the update
            BufferNode* _cur=bit->next();
			/*
			cout << "COMPUTED '" << endl;
			if (_cur!=NULL) {
				_cur->debugPrint();
			} else {
				cout << "NNUULLLL" << endl;
			}
			cout << endl;
			*/
			/*
			if (var->getId()==1) {
            	cout << "FOLLOWING BINDING (" 
                	    << VarName::getInstance()->getVar(var->getId()) 
                    	<< ")" << endl;
	            if (_cur) {
	                cout << "BOUND TO '";
	                BufferAccess::debugPrint(_cur);
	                cout << "'" << endl;
	            } else {
	                cout << "FAILED" << endl;
	                cout << endl << "????????????????????????????????" << endl;
	                StreamPreProcessor::getInstance()->bufferDebugPrint();
	                cout << endl << "????????????????????????????????" << endl;
	            }
			}
			*/
           
			//cout << "EVALUATING UPDATE ONLY..." << endl;
			//cout << *exp << endl;
			/*
			cout << endl << "-------------------------------------------" << endl;
			cout << "BUFFER BEFORE UPDATE:" << endl;
			env->getVar(varstep->getVar()->getId())->debugPrint();
			*/
			
            exp->eval(newenv,Expression::EVAL_UPD_IN_SEQ);
			
			/*
			cout << endl << "BUFFER AFTER UPDATE:" << endl;
			env->getVar(varstep->getVar()->getId())->debugPrint();
			cout << endl << "-------------------------------------------" << endl;
			*/
			//cout << "EVALUATING UPDATE ONLY DONE!" << endl;
			/*
			cout << endl << endl;
			cout << "Unlocking buffer node:" << endl; 
			cur->debugPrint();
			cout << endl << endl;
			*/

            cur->unlock();
			//cout << "YYY" << endl; 

            delete newenv;
            cur=_cur;
        }

    // update evaluation
    } else { // modus==Expression::EVAL_UPD

        while (cur) {

            cur->lock(); // we're not allowed to delete this one
            
            // read one match in advance
            // this avoids crashing when the current token is deleted
            BufferNode* _cur=bit->next(); 
            Environment* newenv=env->clone();
            newenv->addVar(var->getId(),cur);
            exp->eval(newenv,Expression::EVAL_UPD);

            cur->unlock();

            delete newenv;
            cur=_cur;
        }
    }
    delete bit;
   
    /* 
    cout << endl << "EVALUATION FINISHED FOR '$" 
        << VarName::getInstance()->getVar(var->getId()) 
        << "'" << endl;
    */
}

void ForExpression::extractFSAMap(FSAMap* fsamap,unsigned parent_var) {

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

    // step inside for-loop 
    exp->extractFSAMap(fsamap,var_id);
}

Expression* ForExpression::placeUpdates(vector<UpdateExpression*>& updates) {

    exp=exp->placeUpdates(updates);
    for (unsigned i=0; i<updates.size(); i++) {
        if (updates[i]->getVar()==var->getId()) {
            SequenceExpression* sexp=new SequenceExpression(
                    exp,updates[i]);
            exp=sexp;
        }
    }
    return this;
}
