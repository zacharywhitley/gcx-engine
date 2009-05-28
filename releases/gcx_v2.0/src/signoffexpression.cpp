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
 *  @brief Implementations of header file signoffexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file signoffexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
#include "signoffexpression.h"

SignOffExpression::SignOffExpression(VarExpression* _var, Role* _role) :
Expression(et_signoff), var(_var), varstep(NULL), exec_varstep(NULL), role(_role) {}

SignOffExpression::SignOffExpression(VarStepExpression* _varstep, Role* _role) :
Expression(et_signoff), var(NULL), varstep(_varstep), 
exec_varstep(varstep->cloneWithoutFinalDosNodeAndAttributes()),role(_role) {}

SignOffExpression::~SignOffExpression() {
    delete varstep;
    delete var;
    delete exec_varstep;
    // we do not delete member Role* role: this will be done by the RoleList
}

bool SignOffExpression::scopeCheck(vector<unsigned>& def_vars,
                                   vector<unsigned>& introduced_vars) {
    if (var) {
        return var->scopeCheck(def_vars, introduced_vars);
    } else {
        return varstep->scopeCheck(def_vars, introduced_vars);
    }
}

void SignOffExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    if (var) {
        var->replaceVarId(old_id, new_id);
    } else {
        varstep->replaceVarId(old_id, new_id);
        exec_varstep->replaceVarId(old_id, new_id);
    }
}

void SignOffExpression::print(ostream& o) const {
    if (var) {
        o << "signOff(" << (*var) << "," << (*role) << ")";
    } else {
        o << "signOff(" << (*varstep) << "," << (*role) << ")";
        o << " [" << *exec_varstep << "]";
    }
}

void* SignOffExpression::eval(Environment* env, unsigned modus) {

    // we bind to exec_varstep here; this means we do not consider the
    // final /dos::node() step and atribute [1] when issuing signOffs.
    // This implements the idea of aggregated (also called cumulative)
    // roles: only the top level node gets assigned the role, so the
    // signoff is only send to this one.
    BufferIterator* bit=var?
        new BufferIterator(env->getNodeBinding(var->getId()), NULL):
        new BufferIterator(env->getNodeBinding(exec_varstep->getVar()->getId()),
                                exec_varstep->getPath()->isEmptyPath()?NULL:exec_varstep->getPath());
    
    while (BufferNode* cur=bit->getNext(READ_UP_TO_CLOSE_NONE, LOCK_CONTEXT_ALWAYS_CLEAR)) {
        // remove role from node
        cur->removeRoleFromNode(role);
    }
    delete bit;

    return NULL;
}

unsigned SignOffExpression::getVar() {
    if (var) {
        return var->getId();
    } else {
        return varstep->getVar()->getId();
    }
}
