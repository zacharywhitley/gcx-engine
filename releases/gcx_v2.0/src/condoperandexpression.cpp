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
 * 	@brief Implementations of header file condoperandexpression.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file condoperandexpression.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "condoperandexpression.h"

CondOperandExpression::CondOperandExpression(OperandExpression* _left, COMP_TYPE _op, OperandExpression* _right) :
CondExpression(ect_operand), left(_left), op(_op), right(_right) {}

CondOperandExpression::~CondOperandExpression() {
	delete left;
	delete right;
}

bool CondOperandExpression::scopeCheck(vector<unsigned>& def_vars,
                                			vector<unsigned>& introduced_vars) {
	return left->scopeCheck(def_vars, introduced_vars) && right->scopeCheck(def_vars, introduced_vars);
}

void CondOperandExpression::replaceVarId(unsigned old_id, unsigned new_id) {
	left->replaceVarId(old_id, new_id);
	right->replaceVarId(old_id, new_id);
}
	
void CondOperandExpression::extractFSAMap(FSAMap* fsamap, unsigned parent_var) {
	left->extractFSAMap(fsamap, parent_var);
	right->extractFSAMap(fsamap, parent_var);
}
	
void CondOperandExpression::extractParVarMap(ParVarMap* parvarmap) {
	left->extractParVarMap(parvarmap);
	right->extractParVarMap(parvarmap);
}
	
void CondOperandExpression::extractDependencies(vector<DependencySet*>* depset) {
	left->extractDependencies(depset);
	right->extractDependencies(depset);
}
	
Expression* CondOperandExpression::placeSignOffs(vector<SignOffExpression*>& signoffs) {
	left=(OperandExpression*) left->placeSignOffs(signoffs);
	right=(OperandExpression*) right->placeSignOffs(signoffs);
	return this;
}

void CondOperandExpression::rewriteAggregateFuncts() {
	left->rewriteAggregateFuncts();
	right->rewriteAggregateFuncts();
}

void CondOperandExpression::print(ostream& o) const {
	o << (*left) << " ";
	switch (op) {
    	case ct_lt:
    		o << "<";
    		break;
    	case ct_leq:
    		o << "<=";
    		break;
    	case ct_eq:
    		o << "=";
    		break;
    	case ct_gt:
    		o << ">";
    		break;    		
    	case ct_geq:
    		o << ">=";
    		break;
    	case ct_neq:
    		o << "!=";
    		break;
    }
	o << " " << (*right);
}

bool CondOperandExpression::evalCond(Environment* env, unsigned modus) {

	switch (modus) {
		
		case EVAL_QUERY:
		{
			EXP_TYPE l_type = left->getType();
			
			VarStepExpression* l_varstep = NULL;
			if (l_type==et_operandvarstep) {
				l_varstep = (VarStepExpression*)left->getExp();
				l_varstep->initBit(env);
			}

			EXP_TYPE r_type = right->getType();
			
			VarStepExpression* r_varstep = NULL;
			if (r_type==et_operandvarstep) {
				r_varstep = (VarStepExpression*)right->getExp();
				r_varstep->initBit(env);
			}

			void* l_op = left->eval(env, EVAL_QUERY_SILENT);
			void* r_op = right->eval(env, EVAL_QUERY_SILENT);
			
			BufferNode* l_node = NULL;
			BufferNode* l_context = NULL;
			char* l_str = NULL;			

			if (l_type==et_operandvar) {
				l_node = (BufferNode*)l_op;
			} else if (l_type==et_operandvarstep) {
				l_context = (BufferNode*)l_op;
			} else if (l_type==et_operandaggregatefunct) {
				l_str = (char*)l_op;
			} else if (l_type==et_operandconst) {
				l_str = (char*)l_op;
			}
			
			BufferNode* r_node = NULL;
			BufferNode* r_context = NULL;
			char* r_str = NULL;			

			if (r_type==et_operandvar) {
				r_node = (BufferNode*)r_op;
			} else if (r_type==et_operandvarstep) {
				r_context = (BufferNode*)r_op;
			} else if (r_type==et_operandaggregatefunct) {
				r_str = (char*)r_op;
			} else if (r_type==et_operandconst) {
				r_str = (char*)r_op;
			}
			
			bool result = false;
			if (l_node) {
				const char* l_pcd = l_node->getPCDataRepresentation();
				if (r_node) {
					const char* r_pcd = r_node->getPCDataRepresentation();
					result = evalRelOp(l_pcd, op, r_pcd);
					free((char*)r_pcd);		
				} else if (r_context) {
					while (r_context) {
						const char* r_pcd = r_context->getPCDataRepresentation();
						result = evalRelOp(l_pcd, op, r_pcd);
						free((char*)r_pcd);						
						r_context = result?NULL:(BufferNode*)right->eval(env, EVAL_QUERY_SILENT);
					}
				} else if (r_str) {
					result = evalRelOp(l_pcd, op, r_str);
				}
				free((char*)l_pcd);
			} else if (l_context) {
				if (r_node) {
					const char* r_pcd = r_node->getPCDataRepresentation();
					while (l_context) {
						const char* l_pcd = l_context->getPCDataRepresentation();
						result = evalRelOp(l_pcd, op, r_pcd);
						free((char*)l_pcd);
						l_context = result?NULL:(BufferNode*)left->eval(env, EVAL_QUERY_SILENT);
					}
					free((char*)r_pcd);
				}  else if (r_context) {
					bool empty_set = false;
					while (l_context && !empty_set) {
						const char* l_pcd = l_context->getPCDataRepresentation();
						r_varstep->resetBit();
						r_context = (BufferNode*)right->eval(env, EVAL_QUERY_SILENT);
						empty_set = r_context==NULL;
						while (r_context) {
							const char* r_pcd = r_context->getPCDataRepresentation();
							result = evalRelOp(l_pcd, op, r_pcd);
							free((char*)r_pcd);
							r_context = result?NULL:(BufferNode*)right->eval(env, EVAL_QUERY_SILENT);
						}
						free((char*)l_pcd);
						l_context = result?NULL:(BufferNode*)left->eval(env, EVAL_QUERY_SILENT);
					}
				} else if (r_str) {
					while (l_context) {
						const char* l_pcd = l_context->getPCDataRepresentation();
						result = evalRelOp(l_pcd, op, r_str);
						free((char*)l_pcd);
						l_context = result?NULL:(BufferNode*)left->eval(env, EVAL_QUERY_SILENT);
					}
				}
			} else if (l_str) {
				if (r_node) {
					const char* r_pcd = r_node->getPCDataRepresentation();
					result = evalRelOp(l_str, op, r_pcd);
					free((char*)r_pcd);
				} else if (r_context) {
					while (r_context) {
						const char* r_pcd = r_context->getPCDataRepresentation();
						result = evalRelOp(l_str, op, r_pcd);
						free((char*)r_pcd);
						r_context = result?NULL:(BufferNode*)right->eval(env, EVAL_QUERY_SILENT);
					}			
				} else if (r_str) {
					result = evalRelOp(l_str, op, r_str);
				}
			} 

			if (l_type==et_operandaggregatefunct) {
				delete[] l_str;
			}
			if (r_type==et_operandaggregatefunct) {
				delete[] r_str;
			}
	
			return result;
		}

		case EVAL_QUERY_SILENT:
			Error::getInstance()->
				throwError("CondOperandExpression: Illegal Evaluation Mode.");
			break;

		case EVAL_SIGNOFF:
			left->eval(env,EVAL_SIGNOFF);
			right->eval(env,EVAL_SIGNOFF);
			break;
	}

	return false; // never reached
}

bool CondOperandExpression::evalRelOp(const char* left, COMP_TYPE comp, const char* right) {
	switch (comp) {
		case ct_lt:
			return strcmp(left,right)<0;
		case ct_leq:
			return strcmp(left,right)<=0;
		case ct_eq:
			return strcmp(left,right)==0;
		case ct_gt:
			return strcmp(left,right)>0;
		case ct_geq:
			return strcmp(left,right)>=0;
		case ct_neq:
			return strcmp(left,right)!=0;
		default: // should never happen
			return false;
	}
}
