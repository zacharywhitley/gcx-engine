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
 *     @brief Implementations of header file ifexpression.h.
 *     @details Implementations of constructors, destructor and functions of the corresponding header file ifexpression.h.
 *     @author Michael Schmidt
 *     @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#include "ifexpression.h"

IfExpression::IfExpression(CondExpression* _cond, Expression* _thenexp, Expression* _elseexp) :
Expression(et_if), cond(_cond), thenexp(_thenexp), elseexp(_elseexp) {}

IfExpression::~IfExpression() {
    delete cond;
    delete thenexp;
    delete elseexp;
}

bool IfExpression::scopeCheck(vector<unsigned>& def_vars,
                              vector<unsigned>& introduced_vars) {
    return cond->scopeCheck(def_vars, introduced_vars) && thenexp->scopeCheck(
               def_vars, introduced_vars) && elseexp->scopeCheck(def_vars,
                       introduced_vars);
}

void IfExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    cond->replaceVarId(old_id, new_id);
    thenexp->replaceVarId(old_id, new_id);
    elseexp->replaceVarId(old_id, new_id);
}

void IfExpression::mergeSequences() {
    cond->mergeSequences();
    thenexp->mergeSequences();
    elseexp->mergeSequences();
}

void IfExpression::rewriteVarsteps() {
    if (thenexp->getType()==et_varstep) {
        VarStepExpression* sub=((VarStepExpression*)thenexp)->clone();
        delete thenexp;
        const char* vn=VarName::getInstance()->getFreshVarname();
        VarExpression* tmpvar=new VarExpression(vn,true,true);
        ForExpression* forexp=new ForExpression(tmpvar,sub,tmpvar->clone());
        thenexp=forexp;
    } else {
        thenexp->rewriteVarsteps();
    }

    if (elseexp->getType()==et_varstep) {
        VarStepExpression* sub=((VarStepExpression*)elseexp)->clone();
        delete elseexp;
        VarExpression* tmpvar=new VarExpression(VarName::getInstance()->getFreshVarname(),true,true);
        ForExpression* forexp=new ForExpression(tmpvar,sub,tmpvar->clone());
        elseexp=forexp;
    } else {
        elseexp->rewriteVarsteps();
    }
}

void IfExpression::extractFSAMap(FSAMap* fsamap, unsigned parent_var) {
    cond->extractFSAMap(fsamap, parent_var);
    thenexp->extractFSAMap(fsamap, parent_var);
    elseexp->extractFSAMap(fsamap, parent_var);
}

void IfExpression::extractParVarMap(ParVarMap* parvarmap) {
    cond->extractParVarMap(parvarmap);
    thenexp->extractParVarMap(parvarmap);
    elseexp->extractParVarMap(parvarmap);
}

void IfExpression::extractDependencies(vector<DependencySet*>* depset) {
    cond->extractDependencies(depset);
    thenexp->extractDependencies(depset);
    elseexp->extractDependencies(depset);
}

Expression* IfExpression::placeSignOffs(vector<SignOffExpression*>& signoffs) {
    cond=(CondExpression*) cond->placeSignOffs(signoffs);
    thenexp=thenexp->placeSignOffs(signoffs);
    elseexp=elseexp->placeSignOffs(signoffs);
    return this;
}

void IfExpression::rewriteAggregateFuncts() {
    cond->rewriteAggregateFuncts();
    thenexp->rewriteAggregateFuncts();
    elseexp->rewriteAggregateFuncts();
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

void* IfExpression::eval(Environment* env, unsigned modus) {

    switch (modus) {
        case EVAL_QUERY:
            // we evaluate the correct one and distribute signoff
            // statements for the other one; this simulates
            // if-rewriting as described in the GCX ICDE'07 paper
            if (cond->evalCond(env, modus)) {
                    thenexp->eval(env,EVAL_QUERY);
                   elseexp->eval(env,EVAL_SIGNOFF);
            } else {
                elseexp->eval(env,EVAL_QUERY);
                thenexp->eval(env,EVAL_SIGNOFF);
            }
            break;

        case EVAL_QUERY_SILENT:
            Error::getInstance()->throwError("IfExpression: Illegal Evaluation Mode.");
            break;

        case EVAL_SIGNOFF:
            cond->eval(env,EVAL_SIGNOFF);
            thenexp->eval(env,EVAL_SIGNOFF);
            elseexp->eval(env,EVAL_SIGNOFF);
            break;
    }
    
    return NULL;
}

bool IfExpression::containsDirectOutput() {

    // check for direct output in thenexp
    switch (thenexp->getType()) {
        case et_nodeconstr:
        case et_const:
            return true;
        case et_sequence:
        case et_if:
            if (thenexp->containsDirectOutput()) {
                return true;
            }
        default:
            break;
    }

    // check for direct output in elsexp
    switch (elseexp->getType()) {
        case et_nodeconstr:
        case et_const:
            return true;
        case et_sequence:
        case et_if:
            if (elseexp->containsDirectOutput()) {
                return true;
            }
        default:
            break;
    }

    // no direct output present
    return false;
}
