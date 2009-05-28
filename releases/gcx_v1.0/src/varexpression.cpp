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
#include "varexpression.h"
#include "pathenvironment.h"
#include "streampreprocessor.h"

VarExpression::VarExpression()
	: Expression(e_typevar), id(0) {}

VarExpression::VarExpression(const char* _name) 
    : Expression(e_typevar), id(VarName::getInstance()->addVar(_name)) {}

VarExpression::VarExpression(const char* _name,bool force) 
    : Expression(e_typevar), id(VarName::getInstance()->addVar(_name,force)) {}

VarExpression::~VarExpression() {
}

void VarExpression::print(ostream& o) const {

  if(id != 0) // do not display root variable

    o << "$" << VarName::getInstance()->getVar(id);
}

bool VarExpression::scopeCheck(vector<unsigned>& def_vars,
								vector<unsigned>& introduced_vars) {

    for (unsigned i=0; i<def_vars.size(); i++) {
        if (id==def_vars[i])
            return true;
    }

    cout << "Scoping Error for Var $" 
        << VarName::getInstance()->getVar(id)
        << "..." << endl;

    return false;
}

void VarExpression::replaceVarId(unsigned old_id,unsigned new_id) {
	if (id==old_id) {
		id=new_id;
	}
}

void VarExpression::extractProjectionTree(ProjectionTree* pt) {
	pt->createDOSStarChildNode(id,
								RoleList::getInstance()->
									freshRole(id,ROLETYPE_NONEXISTENCE,NULL),
								e_out);
}

bool VarExpression::subsumesVarInAllBranches(unsigned var) {
//	return var==id;
	return false;
}

void VarExpression::eval(Environment* env, unsigned modus) {

    // don't care about non-evaluation modi
    if (modus!=Expression::EVAL_QUERY) 
        return;

    BufferNode* varnode=env->getVar(id);
	if (varnode->type==TYPE_TAG) {
	    while (!varnode->isClosed()) {
	        StreamPreProcessor::getInstance()->readNext();
	    }
	}
    varnode->print();
}

VarExpression* VarExpression::clone() {
	VarExpression* vexp=new VarExpression();
	vexp->setId(id);

	return vexp;
}
