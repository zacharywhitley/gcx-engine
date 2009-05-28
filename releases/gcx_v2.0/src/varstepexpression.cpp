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
 * 	@brief Implementations of header file varstepexpression.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file varstepexpression.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "varstepexpression.h"

VarStepExpression::VarStepExpression(VarExpression* _var, PathExpression* _path) :
Expression(et_varstep), var(_var), path(_path), bit(NULL) {}

VarStepExpression::~VarStepExpression() {
    delete var;
    delete path;
    delete bit;
}

bool VarStepExpression::scopeCheck(vector<unsigned>& def_vars,
                                   vector<unsigned>& introduced_vars) {
    return var->scopeCheck(def_vars, introduced_vars);
}

void VarStepExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    var->replaceVarId(old_id, new_id);
}

void VarStepExpression::extractDependencies(vector<DependencySet*>* depset) {
    for (unsigned i=0; i<depset->size(); i++) {
        if ((*depset)[i]->getVar()==var->getId()) {
            (*depset)[i]->insertTuple(path, false, true, false);
            break;
        }
    }
}

void VarStepExpression::print(ostream& o) const {
    o << (*var) << (*path);
}

void* VarStepExpression::eval(Environment* env, unsigned modus) {

	switch (modus) {

		case EVAL_QUERY:
		{
			BufferIterator* bit=new BufferIterator(env->getNodeBinding(var->getId()), path);
			while (BufferNode* cur=bit->getNext(READ_UP_TO_CLOSE_CONTEXT)) {
				cur->print();
			}
			delete bit;
			break;
		}

		case EVAL_QUERY_SILENT:
		{
			return (void*)bit?bit->getNext(READ_UP_TO_CLOSE_CONTEXT):NULL;
		}

		case EVAL_SIGNOFF:
			// nothing to be done
			break;
	}

	return NULL;
}

void VarStepExpression::initBit(Environment* env) {
	delete bit;
	bit=new BufferIterator(env->getNodeBinding(var->getId()), path);
}

void VarStepExpression::resetBit() {
	if (bit) {
		bit->reset();
	}
}

VarStepExpression* VarStepExpression::clone() {
    return new VarStepExpression(var->clone(), path->clone());
}

VarStepExpression* VarStepExpression::cloneWithoutFinalDosNodeAndAttributes() {
    return new VarStepExpression(var->clone(), 
								path->cloneWithoutFinalDosNodeAndAttributes());
}
