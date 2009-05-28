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
 * 	@brief Implementations of header file varexpression.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file varexpression.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "varexpression.h"

VarExpression::VarExpression(bool _print_var) :
Expression(et_var), id(TAGID_ROOT), print_var(_print_var) {}

VarExpression::VarExpression(const char* _name, bool _print_var) :
Expression(et_var), id(VarName::getInstance()->insertVarname(_name)), print_var(_print_var) {
	free((char*)_name);
}

VarExpression::VarExpression(const char* _name, bool _force, bool _print_var) :
Expression(et_var), id(VarName::getInstance()->insertVarname(_name, _force)), print_var(_print_var) {
	free((char*)_name);
}

VarExpression::~VarExpression() {}

bool VarExpression::scopeCheck(vector<unsigned>& def_vars,
                               vector<unsigned>& introduced_vars) {
    for (unsigned i=0; i<def_vars.size(); i++) {
        if (id==def_vars[i]) {
            return true;
        }
    }
    cerr << "Scope Error For Variable $" << VarName::getInstance()->getVarname(id) << "..." << endl;

    return false;
}

void VarExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    if (id==old_id) {
        id=new_id;
    }
}

void VarExpression::extractDependencies(vector<DependencySet*>* depset) {
    for (unsigned i=0; i<depset->size(); i++) {
        if ((*depset)[i]->getVar()==id) {
            (*depset)[i]->insertTuple(NULL, false, true, false);
            break;
        }
    }
}

void VarExpression::print(ostream& o) const {
    if (print_var) {
    	o << "$" << VarName::getInstance()->getVarname(id);
    }
}

void* VarExpression::eval(Environment* env, unsigned modus) {

	switch (modus) {

		case EVAL_QUERY:
		case EVAL_QUERY_SILENT:
		{
    		BufferIterator* bit=new BufferIterator(env->getNodeBinding(id), NULL);
    		BufferNode* cur=bit->getNext(READ_UP_TO_CLOSE_BASE);
			delete bit;
			
    		if (modus==EVAL_QUERY) {
		    	cur->print(); // will return NULL in this case
		    } else {
				return (void*)cur;
			}
		}

		case EVAL_SIGNOFF:
			// nothing to be done
			break;
	}

	return NULL;
}

VarExpression* VarExpression::clone() {
    VarExpression* varexp=new VarExpression(print_var);
    varexp->setId(id);

    return varexp;
}
