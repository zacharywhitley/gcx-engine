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
#include "nodenodetestcondexpression.h"
#include "streampreprocessor.h"
#include "varname.h"
#include "bufferiteration.h"
#include "charnode.h"
#include "error.h"

// varstep - varstep comparison
NodeNodeTestCondExpression::NodeNodeTestCondExpression(
    VarStepExpression* _left, COMP_TYPE _op, VarStepExpression* _right)
    : NodeTestCondExpression(e_ctypenctest), left_v(_left->getVar()),
        left_ps(_left->getPathStep()), op(_op),
        right_v(_right->getVar()), right_ps(_right->getPathStep()) {}    

// varstep - var comparison
NodeNodeTestCondExpression::NodeNodeTestCondExpression(
    VarStepExpression* _left, COMP_TYPE _op, VarExpression* _right)
    : NodeTestCondExpression(e_ctypenctest), left_v(_left->getVar()),
        left_ps(_left->getPathStep()), op(_op), right_v(_right), 
        right_ps(NULL) {}

// var - varstep comparison
NodeNodeTestCondExpression::NodeNodeTestCondExpression(
    VarExpression* _left, COMP_TYPE _op, VarStepExpression* _right)
    : NodeTestCondExpression(e_ctypenctest), left_v(_left),
        left_ps(NULL), op(_op), right_v(_right->getVar()), 
        right_ps(_right->getPathStep()) {}

// var - var comparison
NodeNodeTestCondExpression::NodeNodeTestCondExpression(
    VarExpression* _left, COMP_TYPE _op, VarExpression* _right)
    : NodeTestCondExpression(e_ctypenctest), left_v(_left),
        left_ps(NULL), op(_op), right_v(_right), right_ps(NULL) {}
        
NodeNodeTestCondExpression::~NodeNodeTestCondExpression() {
    delete left_v;
    if (left_ps!=NULL)
        delete left_ps;
    delete right_v;
    if (right_ps!=NULL)
        delete right_ps;
}

void NodeNodeTestCondExpression::print(ostream& o) const {

    o << *left_v;
    if (left_ps!=NULL)
        o << *left_ps;
        switch (op) {
	case e_lt:
	  o << "<";
	  break;
	case e_leq:
	  o << "<=";
	  break;
	case e_eq:
	  o << "=";
	  break;
	case e_geq:
	  o << ">=";
	  break;
	case e_gt:
	  o << ">";
	  break;
	case e_neq:
	  o << "!=";
	  break;
        }
    o << *right_v;
    if (right_ps!=NULL)
    o << *right_ps;
}

bool NodeNodeTestCondExpression::scopeCheck(vector<unsigned>& def_vars,
											vector<unsigned>& introduced_vars) {
    return left_v->scopeCheck(def_vars,introduced_vars)
        && right_v->scopeCheck(def_vars,introduced_vars);
}

void NodeNodeTestCondExpression::replaceVarId(unsigned old_id,unsigned new_id) {
	left_v->replaceVarId(old_id,new_id);
	right_v->replaceVarId(old_id,new_id);
}

void NodeNodeTestCondExpression::extractProjectionTree(ProjectionTree* pt) {
		
	if (left_ps!=NULL && left_ps->getType()==e_typepathstep) {
		pt->createChildNode(left_v->getId(),
							-1,
							left_ps->getAxisType(),
							left_ps->getNodeTest(),
							true,
							false,
							e_out,
							RoleList::getInstance()->freshRole(left_v->getId(),
																ROLETYPE_NONEXISTENCE,
																left_ps));
	} else {
		pt->createDOSStarChildNode(left_v->getId(),
									RoleList::getInstance()->freshRole(left_v->getId(),
																		ROLETYPE_NONEXISTENCE,
																		left_ps),
									e_out);
	}
	
	if (right_ps!=NULL && right_ps->getType()==e_typepathstep) {
		pt->createChildNode(right_v->getId(),
							-1,
							right_ps->getAxisType(),
							right_ps->getNodeTest(),
							true,
							false,
							e_out,
							RoleList::getInstance()->freshRole(right_v->getId(),
																ROLETYPE_NONEXISTENCE,
																right_ps));
	} else {
		pt->createDOSStarChildNode(right_v->getId(),
									RoleList::getInstance()->freshRole(right_v->getId(),
																		ROLETYPE_NONEXISTENCE,
																		right_ps),
									e_out);
	}
}

bool NodeNodeTestCondExpression::subsumesVarInAllBranches(unsigned var) {
	return left_v->getId()==var && right_v->getId()==var;
}


bool NodeNodeTestCondExpression::evalCond(Environment* env) {
    
    BufferNode* l=env->getVar(left_v->getId());
    BufferNode* r=env->getVar(right_v->getId());

    // var - var
    if (left_ps==NULL && right_ps==NULL) {
        // assert both children being read before execution
		if (l->type==TYPE_TAG && r->type!=TYPE_TAG) {
			while(!(l->isClosed())) {
				StreamPreProcessor::getInstance()->readNext();
			}
		} else if (l->type!=TYPE_TAG && r->type==TYPE_TAG) {
			while(!(r->isClosed())) {
				StreamPreProcessor::getInstance()->readNext();
			}
		} else if (l->type==TYPE_TAG && r->type==TYPE_TAG) {
			while (!(l->isClosed() && r->isClosed())) {
				StreamPreProcessor::getInstance()->readNext();
			}
		}

		const char* lcpc=l->getPCDataRepresentation();
		const char* rcpc=r->getPCDataRepresentation();
        bool ret=NodeTestCondExpression::evalRelOp(rcpc,op,lcpc);
		free((char*)lcpc);
		free((char*)rcpc);
		return ret;
    
    // varstep - var
    } else if (right_ps==NULL) {
        
        // assert non-pathstep node children to be read
		if (r->type==TYPE_TAG) {
	        while (!r->isClosed()) {
	            StreamPreProcessor::getInstance()->readNext();
	        }
		}

        // create matching iterator
        BufferIterator* bit=
			BufferIteration::createBufferIterator(l,left_ps);
        BufferNode* cur=bit->next();

		const char* rcpc=r->getPCDataRepresentation();
        while (cur) {

			// assert that path step has been completely read
			if (cur->type==TYPE_TAG) {
	            while (!cur->isClosed()) {
	                StreamPreProcessor::getInstance()->readNext();
	            }
			}

			const char* lcpc=cur->getPCDataRepresentation();
			if (NodeTestCondExpression::evalRelOp(lcpc,op,rcpc)) {
					delete bit;
					free((char*)lcpc);
					free((char*)rcpc);
					return true;
			} 
			free((char*)lcpc);
           	cur=bit->next();
        }
        free((char*)rcpc);
        delete bit;

    // var - varstep
    } else if (left_ps==NULL) {
        
        // assert non-pathstep node children to be read
		if (l->type==TYPE_TAG) {
	        while (!l->isClosed()) {
 	           StreamPreProcessor::getInstance()->readNext();
	        }
		}
        
        // create matching iterator
        BufferIterator* bit=
			BufferIteration::createBufferIterator(r,right_ps);
        BufferNode* cur=bit->next();

		const char* lcpc=l->getPCDataRepresentation();
        while (cur) {

			if (cur->type==TYPE_TAG) {
	            while (!cur->isClosed()) {
	                StreamPreProcessor::getInstance()->readNext();
	            }
			}

			const char* rcpc=cur->getPCDataRepresentation();
			if (NodeTestCondExpression::evalRelOp(lcpc,op,rcpc)) {
					delete bit;
					free((char*)lcpc);
					free((char*)rcpc);
					return true;
			} 
			free((char*)rcpc);
           	cur=bit->next();
        }
        free((char*)lcpc);
        delete bit;

    // varstep - varstep
    } else {
        
        // compute axis types
        AXIS_TYPE l_axis=left_ps->getAxisType();
        if (!(l_axis==e_child || l_axis==e_descendant)) {
			Error::getInstance()->throwError(
				"Undefined axis type in node-node-test expression");
        }
        AXIS_TYPE r_axis=right_ps->getAxisType();
        if (!(r_axis==e_child || r_axis==e_descendant)) {
			Error::getInstance()->throwError(
				"Undefined axis type in node-node-test expression");
        }
        
        BufferIterator* l_bit=
			BufferIteration::createBufferIterator(l,left_ps);
        BufferNode* l_cur=l_bit->next();

        while (l_cur) {

			if (l_cur->type==TYPE_TAG) {
	            while (!l_cur->isClosed()) {
	                StreamPreProcessor::getInstance()->readNext();
	            }
			}

            // create right hand iterator
        	BufferIterator* r_bit=
				BufferIteration::createBufferIterator(r,right_ps);

			const char* lcpc=l_cur->getPCDataRepresentation();
            BufferNode* r_cur=r_bit->next();
            while (r_cur) {

				if (r_cur->type==TYPE_TAG) {
	                while (!r_cur->isClosed()) {
	                    StreamPreProcessor::getInstance()->readNext();
	                }
				}

				const char* rcpc=r_cur->getPCDataRepresentation();
				if (NodeTestCondExpression::evalRelOp(lcpc,op,rcpc)) {
						delete l_bit;
						delete r_bit;
						free((char*)lcpc);
						free((char*)rcpc);
						return true;
				} 
				free((char*)rcpc);
           		r_cur=r_bit->next();
            }
            delete r_bit;
            
			free((char*)lcpc);
            l_cur=l_bit->next();
        }
        delete l_bit;
    }
    
    return false; // no match
}
