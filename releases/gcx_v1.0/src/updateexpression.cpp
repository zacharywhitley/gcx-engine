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
#include "updateexpression.h"
#include "varname.h"
#include "streampreprocessor.h"
#include "bufferchilditerator.h"
#include "bufferdescendantiterator.h"

UpdateExpression::UpdateExpression(unsigned _var,
                                    PathExpression* _path,
                                    int _index)
    : Expression(e_typeupdate), var(_var), path(_path),
	 index(_index) {}

UpdateExpression::~UpdateExpression() {
    delete path;
}

void UpdateExpression::print(ostream& o) const {

    o << "signOff($" << VarName::getInstance()->getVar(var);
	if (path!=NULL) {
		o << *path << ",";
	} else {
		o << ",";
	}
	o << index << ")";
}

bool UpdateExpression::scopeCheck(vector<unsigned>& def_vars,
									vector<unsigned>& introduced_vars) {

    for (unsigned i=0; i<def_vars.size(); i++) {
        if (var==def_vars[i])
            return true;
    }

    cout << "Scoping Error for Var $" 
        << VarName::getInstance()->getVar(var)
        << "..." << endl;

    return false;
}

void UpdateExpression::replaceVarId(unsigned old_id,unsigned new_id) {
	if (var==old_id) {
		var=new_id;
	}
}

void UpdateExpression::eval(Environment* env, unsigned modus) {

    // updates are NOT evaluated in query evaluation mode
    if (modus==Expression::EVAL_QUERY)
    	return;

    BufferNode* base=env->getVar(var);

	if (base->type==TYPE_TAG) {
	    while (!base->isClosed()) {
	        StreamPreProcessor::getInstance()->readNext();
	    }
	}

    vector<BufferNode*>* matches=new vector<BufferNode*>;
    base->matchingNodes(path,matches); // collect all matches

    for (unsigned i=0; i<matches->size(); i++) {
        ((*matches)[i])->distributeIndexUpdate(index);
    }
    delete matches;
}
