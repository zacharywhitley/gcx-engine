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
 *     @brief Implementations of header file operandexpression.h.
 *     @details Implementations of constructors, destructor and functions of the corresponding header file operandexpression.h.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#include "operandexpression.h"

OperandExpression::OperandExpression(VarExpression* _var) :
Expression(et_operandvar), var(_var), varstep(NULL), aggfunct(NULL), cstring(NULL) {}
    
OperandExpression::OperandExpression(VarStepExpression* _varstep) :
Expression(et_operandvarstep), var(NULL), varstep(_varstep), aggfunct(NULL), cstring(NULL) {}
    
OperandExpression::OperandExpression(AggregateFunctExpression* _aggfunct) :
Expression(et_operandaggregatefunct), var(NULL), varstep(NULL), aggfunct(_aggfunct), cstring(NULL) {}
    
OperandExpression::OperandExpression(ConstExpression* _cstring) :
Expression(et_operandconst), var(NULL), varstep(NULL), aggfunct(NULL), cstring(_cstring) {}
    
OperandExpression::~OperandExpression() {
    delete var;
    delete varstep;
    delete aggfunct;
    delete cstring;
}

bool OperandExpression::scopeCheck(vector<unsigned>& def_vars,
                                vector<unsigned>& introduced_vars) {
    if (var) {
        return var->scopeCheck(def_vars, introduced_vars);
    } else if (varstep) {
        return varstep->scopeCheck(def_vars, introduced_vars);
    } else if (aggfunct) {
        return aggfunct->scopeCheck(def_vars, introduced_vars);
    }
    return true;
}

void OperandExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    if (var) {
        var->replaceVarId(old_id, new_id);
    } else if (varstep) {
        varstep->replaceVarId(old_id, new_id);
    } else if (aggfunct) {
        aggfunct->replaceVarId(old_id, new_id);
    }
}
    
void OperandExpression::extractFSAMap(FSAMap* fsamap, unsigned parent_var) {
    if (var) {
        var->extractFSAMap(fsamap, parent_var);
    } else if (varstep) {
        varstep->extractFSAMap(fsamap, parent_var);
    } else if (aggfunct) {
        aggfunct->extractFSAMap(fsamap, parent_var);
    }
}
    
void OperandExpression::extractParVarMap(ParVarMap* parvarmap) {
    if (aggfunct) {
        aggfunct->extractParVarMap(parvarmap);
    }
}
    
void OperandExpression::extractDependencies(vector<DependencySet*>* depset) {
    if (var) {
        var->extractDependencies(depset);
    } else if (varstep) {
        varstep->extractDependencies(depset);
    } else if (aggfunct) {
        aggfunct->extractDependencies(depset);
    }
}
    
Expression* OperandExpression::placeSignOffs(vector<SignOffExpression*>& signoffs) {
    if (var) {
        var=(VarExpression*) var->placeSignOffs(signoffs);
    } else if (varstep) {
        varstep=(VarStepExpression*) varstep->placeSignOffs(signoffs);
    } else if (aggfunct) {
        aggfunct=(AggregateFunctExpression*) aggfunct->placeSignOffs(signoffs);
    }
    return this;
}
    
void OperandExpression::rewriteAggregateFuncts() {
    if (aggfunct) {
        aggfunct->rewriteAggregateFuncts();
    }
}

void OperandExpression::print(ostream& o) const {
    if (var) {
        o << (*var);
    } else if (varstep) {
        o << (*varstep);
    } else if (aggfunct) {
        o << (*aggfunct);
    } else {
        o << (*cstring);
    }
}
 
void* OperandExpression::eval(Environment* env, unsigned modus) {
    // there is no need to distinguish between the mode here; the
    // only difference is that the return values of this function
    // are used only in mode EVAL_QUERY_SILENT; of course, they do
    // not cause any damage
    if (var) {
        return var->eval(env,modus);
    } else if (varstep) {
        return varstep->eval(env,modus);
    } else if (aggfunct) {
        return aggfunct->eval(env,modus);
    } else {
        return cstring->eval(env,modus);
    }

    return NULL;
}

Expression* OperandExpression::getExp() {
    if (var) {
        return var;
    } else if (varstep) {
        return varstep;
    } else if (aggfunct) {
        return aggfunct;
    } else {
        return cstring;
    }
}
