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
#include "nodeconsttestcondexpression.h"
#include "streampreprocessor.h"
#include "bufferiteration.h"
#include "charnode.h"

NodeConstTestCondExpression::NodeConstTestCondExpression(
    VarExpression* _var, COMP_TYPE _op, const char* _nt) 
    : NodeTestCondExpression(e_ctypenntest), var(_var),
        pathstep(NULL), op(_op), nt(_nt) {}

NodeConstTestCondExpression::NodeConstTestCondExpression(
    VarStepExpression* _varstep, COMP_TYPE _op, const char* _nt) 
    : NodeTestCondExpression(e_ctypenntest), var(_varstep->getVar()), 
        pathstep(_varstep->getPathStep()), op(_op), nt(_nt) {}

NodeConstTestCondExpression::~NodeConstTestCondExpression() {
    if (pathstep!=NULL)
        delete pathstep;
    delete var;
}

void NodeConstTestCondExpression::print(ostream& o) const {
    o << (*var);
    if (pathstep!=NULL) {
        o << (*pathstep);
    }
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
    o << "\"" << nt << "\"";
}

bool NodeConstTestCondExpression::scopeCheck(vector<unsigned>& def_vars,
												vector<unsigned>& introduced_vars) {
    return var->scopeCheck(def_vars,introduced_vars);
}

void NodeConstTestCondExpression::replaceVarId(unsigned old_id,unsigned new_id) {
	var->replaceVarId(old_id,new_id);
}

void NodeConstTestCondExpression::extractProjectionTree(ProjectionTree* pt) {
	if (pathstep!=NULL && pathstep->getType()==e_typepathstep) {
		pt->createChildNode(var->getId(),
							-1,
							pathstep->getAxisType(),
							pathstep->getNodeTest(), 
							true,
							false,
							e_out,
							RoleList::getInstance()->freshRole(var->getId(),
																ROLETYPE_NONEXISTENCE,
																pathstep));
	} else 	{
		pt->createDOSStarChildNode(var->getId(),
									RoleList::getInstance()->freshRole(var->getId(),
																		ROLETYPE_NONEXISTENCE,
																		pathstep),
									e_out);
	}
}

bool NodeConstTestCondExpression::subsumesVarInAllBranches(unsigned _var) {
	return var->getId()==_var;
}
		
bool NodeConstTestCondExpression::evalCond(Environment* env) {

    BufferNode* x=env->getVar(var->getId());
    
    if (pathstep==NULL) {
		if (x->type==TYPE_TAG) {
	        while (!x->isClosed()) {
	             StreamPreProcessor::getInstance()->readNext();
	        }
		}
		const char* lpc=x->getPCDataRepresentation();
        bool ret=NodeTestCondExpression::evalRelOp(lpc,op,nt);
		free((char*)lpc);
		return ret;

    } else {
    
        // create matching iterator
		BufferIterator* bit=
			BufferIteration::createBufferIterator(x,pathstep);
        BufferNode* cur=bit->next();

        while (cur) {

            // assert children to be read already
			if (cur->type==TYPE_TAG) {
	            while (!cur->isClosed()) {
	                StreamPreProcessor::getInstance()->readNext();
	            }
			}

			const char* lpc=cur->getPCDataRepresentation();
			if (NodeTestCondExpression::evalRelOp(lpc,op,nt)) {
				free((char*)lpc);
				delete bit;
				return true;
			}

			free((char*)lpc);
	        cur=bit->next();
        }
        delete bit;
    }
    
    return false; // no match
    
}
