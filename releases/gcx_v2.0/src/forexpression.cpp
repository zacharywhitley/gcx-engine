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
 *     @brief Implementations of header file forexpression.h.
 *     @details Implementations of constructors, destructor and functions of the corresponding header file forexpression.h.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#include "forexpression.h"

ForExpression::ForExpression(VarExpression* _var, VarExpression* _varexp, Expression* _exp) :
Expression(et_for), var(_var), varexp(_varexp), varstep(NULL), exp(_exp) {}

ForExpression::ForExpression(VarExpression* _var, VarStepExpression* _varstep, Expression* _exp) :
Expression(et_for), var(_var), varexp(NULL), varstep(_varstep), exp(_exp) {}

ForExpression::~ForExpression() {
    delete var;
    delete varexp;    
    delete varstep;  
    delete exp;
}

bool ForExpression::scopeCheck(vector<unsigned>& def_vars,
                               vector<unsigned>& introduced_vars) {
    vector<unsigned> new_def_vars=def_vars;

    // check for variable name having been introduced yet ...
    bool varname_in_use=false;
    for (unsigned i=0; i<introduced_vars.size() && !varname_in_use; i++) {
        varname_in_use|= strcmp(VarName::getInstance()->getVarname(introduced_vars[i]), VarName::getInstance()->getVarname(var->getId()))==TAGID_ROOT;
    }

    // ... and replace the id if necessary
    if (varname_in_use) {
        unsigned old_id=var->getId();
        const char* var_name=VarName::getInstance()->getVarname(old_id);
        //unsigned new_id=VarName::getInstance()->insertVarname(strdup(var_name), true);
        unsigned new_id=VarName::getInstance()->insertVarname(var_name, true);
        var->setId(new_id);
        exp->replaceVarId(old_id, new_id);
        new_def_vars.push_back(new_id);
    } else {
        // stricly speaking, the following should only be applied to
        // the "exp" part of the final return statement, but the change
        // does not affect varstep expressions, so nothing will go wrong
        introduced_vars.push_back(var->getId());
        new_def_vars.push_back(var->getId());
    }

    return varexp?
        varexp->scopeCheck(def_vars, introduced_vars) &&
            exp->scopeCheck(new_def_vars, introduced_vars):
        varstep->scopeCheck(def_vars, introduced_vars) &&
            exp->scopeCheck(new_def_vars, introduced_vars);
}

void ForExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    if (varexp) {
        varexp->replaceVarId(old_id, new_id);
    } else {
        varstep->replaceVarId(old_id, new_id);    
    }

    // a new scoping area may be created for exp, take special care
    if (strcmp(VarName::getInstance()->getVarname(old_id),
        VarName::getInstance()->getVarname(var->getId()))!=TAGID_ROOT) {
        exp->replaceVarId(old_id, new_id);
    }
}

void ForExpression::mergeSequences() {
    exp->mergeSequences();
}

void ForExpression::rewriteVarsteps() {
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

void ForExpression::extractFSAMap(FSAMap* fsamap, unsigned parent_var) {
    unsigned var_id=var->getId();
    unsigned parent_var_id=varstep?varstep->getVar()->getId():varexp->getId();

    if (parent_var_id==parent_var) {
        if (fsamap->getFSA(parent_var)==parent_var) {
            fsamap->insertFSA(var_id, var_id);
        } else {
            fsamap->insertFSA(var_id, fsamap->getFSA(parent_var));
        }
    } else {
        fsamap->insertFSA(var_id, fsamap->getFSA(parent_var_id));
    }

    exp->extractFSAMap(fsamap, var_id);
}

void ForExpression::extractParVarMap(ParVarMap* parvarmap) {
    if (varexp) {
        parvarmap->insertParVar(var->getId(), varexp->getId(), NULL, containsDirectOutput());
    } else {
        parvarmap->insertParVar(var->getId(),varstep->getVar()->getId(),
                                varstep->getPath(), containsDirectOutput());
    }

    exp->extractParVarMap(parvarmap);
}

void ForExpression::extractDependencies(vector<DependencySet*>* depset) {
    exp->extractDependencies(depset);
}

Expression* ForExpression::placeSignOffs(vector<SignOffExpression*>& signoffs) {
    exp=exp->placeSignOffs(signoffs);
    for (unsigned i=0; i<signoffs.size(); i++) {
        if (signoffs[i]->getVar()==var->getId()) {
            SequenceExpression* sexp=new SequenceExpression(exp,signoffs[i]);
            exp=sexp;
        }
    }
    return this;
}

void ForExpression::rewriteAggregateFuncts() {
    exp->rewriteAggregateFuncts();
}

void ForExpression::print(ostream& o) const {
    if (varexp) {
        o << "for " << (*var) << " in " << (*varexp) << " return ";    
    } else {
        o << "for " << (*var) << " in " << (*varstep) << " return ";
    }
    
    EXP_TYPE subtype=exp->getType();
    if (subtype==et_for || subtype==et_if || subtype==et_nodeconstr
            || subtype==et_sequence || subtype==et_signoff) {
        o << endl;
        pp->increaseTab();
        pp->printTabs();
        o << (*exp);
        pp->decreaseTab();
    } else {
        o << (*exp);
    }
}

void* ForExpression::eval(Environment* env, unsigned modus) {

    switch (modus) {
        
        case EVAL_QUERY:
        {
            // we create a buffer iterator and iterate over the buffer,
            // thereby creating new environments for each binding
            // and propagating evaluation
            BufferIterator* bit=varexp?
                new BufferIterator(env->getNodeBinding(varexp->getId()),NULL):
                new BufferIterator(env->getNodeBinding(varstep->getVar()->getId()),varstep->getPath());

            while (BufferNode* cur=bit->getNext(READ_UP_TO_CLOSE_NONE, LOCK_CONTEXT_ALWAYS_CLEAR)) {
                Environment* newenv=env->clone();
                newenv->insertNodeBinding(var->getId(), cur);
                exp->eval(newenv,EVAL_QUERY);

                delete newenv;
            }
            delete bit;
            
            break;
        }

        case EVAL_QUERY_SILENT:
            Error::getInstance()->throwError("ForExpression: Illegal Evaluation Mode.");
            break;

        case EVAL_SIGNOFF:
        {
            BufferIterator* bit=varexp?
                new BufferIterator(env->getNodeBinding(varexp->getId()),NULL):
                new BufferIterator(env->getNodeBinding(varstep->getVar()->getId()),varstep->getPath());

            while (BufferNode* cur=bit->getNext(READ_UP_TO_CLOSE_NONE, LOCK_CONTEXT_ALWAYS_CLEAR)) {
                // evaluate the signOff statements
                Environment* newenv=env->clone();
                newenv->insertNodeBinding(var->getId(), cur);
                exp->eval(newenv,EVAL_SIGNOFF);
                
                delete newenv;
            }
            delete bit;
            break;
        }
    }
    
    return NULL;
}

void ForExpression::evalSignOffForBinding(Environment* env, BufferNode* cur) {
    // we create the new environment and send the signOff statements
    Environment* newenv=env->clone();
    newenv->insertNodeBinding(var->getId(), cur);
    exp->eval(newenv,EVAL_SIGNOFF);
    delete newenv;
}

bool ForExpression::containsDirectOutput() {
    switch (exp->getType()) {
        case et_nodeconstr:
        case et_const:
            return true;
        case et_sequence:
        case et_if:
            return exp->containsDirectOutput();
        default:
            return false;
    }
}
