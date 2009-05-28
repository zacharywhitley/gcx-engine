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
 * 	@brief Implementations of header file aggregatefunctmaxexpression.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file aggregatefunctmaxexpression.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "aggregatefunctmaxexpression.h"
#include "error.h"

AggregateFunctMaxExpression::AggregateFunctMaxExpression(VarExpression* _var) :
AggregateFunctExpression(et_aggregatefunctmax, _var) {}

AggregateFunctMaxExpression::AggregateFunctMaxExpression(VarStepExpression* _varstep) :
AggregateFunctExpression(et_aggregatefunctmax, _varstep) {}

AggregateFunctMaxExpression::~AggregateFunctMaxExpression() {}

void* AggregateFunctMaxExpression::eval(Environment* env, unsigned modus) {
	if (!forexp) {
		Error::getInstance()->throwError("Pure Var Or Varstep Expression In Aggregate Function.");
	}

	switch (modus) {
		case EVAL_QUERY:
		case EVAL_QUERY_SILENT:
		{
			const char* max_ret = NULL;
			BufferIterator* bit=var?
				new BufferIterator(env->getNodeBinding(var->getId()), NULL):
				new BufferIterator(env->getNodeBinding(varstep->getVar()->getId()),	varstep->getPath());
			
			bool delete_max_ret=false;
			long double count = 0;
			long double result = 0;
			bool empty_sequence = true;
			bool max_type_string = false;
			bool max_type_numeric = false;
			const char* pcd = NULL;
			
			try {
				while (BufferNode* cur=bit->getNext(READ_UP_TO_CLOSE_CONTEXT, LOCK_CONTEXT_ALWAYS_CLEAR)) {
					
					empty_sequence = false;
					count++;
					
					pcd = cur->getPCDataRepresentation();
					
					if (!max_type_string && !max_type_numeric) {
						try {
							result = MiscFunctions::getNumericFromString(pcd);
							max_type_numeric = true;
						} catch (const char* e) {
							max_type_string = true;
						}
					}

					if (max_type_numeric) {
						if (count > 1) {
							result = max(result, MiscFunctions::getNumericFromString(pcd));
						} else {
							result = MiscFunctions::getNumericFromString(pcd);
						}
						free((char*)pcd);
					} else { // max_type_string
						if (count > 1) {
							if (strcmp(max_ret, pcd) > 0) {
								free((char*)pcd);
							} else {					
								delete[] max_ret;
								max_ret = pcd;
							}
						} else {
							max_ret = pcd;
						}
					}
					
					forexp->evalSignOffForBinding(env,cur);
				}
				delete bit;
				
				if (!empty_sequence) {
					if (max_type_numeric) {
						result = MiscFunctions::getRoundFrom(result);
						max_ret = MiscFunctions::getStringFromNumerical(result);
					}					
				} else {
					max_ret = new char[strlen(OUTPUT_MAX_ON_EMPTY_SEQUENCE)+1];
					strcpy((char*)max_ret, OUTPUT_MAX_ON_EMPTY_SEQUENCE);
				}
				delete_max_ret=true;
			} catch (const char* e) {
				Error::getInstance()->throwError(e);
			}

			if (modus==EVAL_QUERY) {
				cout << max_ret;
				if (delete_max_ret)	delete[] max_ret;
			} else {
				return (void*)max_ret;
			}
			break;
		}

		case EVAL_SIGNOFF:
			forexp->eval(env,EVAL_SIGNOFF);
			break;
	}

	return NULL;
}
