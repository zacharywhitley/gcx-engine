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
#include "expression.h"

unsigned Expression::EVAL_QUERY=0;
unsigned Expression::EVAL_UPD=1;
unsigned Expression::EVAL_UPD_IN_SEQ=2;

Expression::Expression(EXP_TYPE _type) 
    : pp(PrettyPrint::getInstance()), 
        tagmap(TagMap::getInstance()),
        type(_type) {}

Expression::~Expression() {}

/*!
 * default implementation:
 * will be used for every expression not containing variables
*/
bool Expression::scopeCheck(vector<unsigned>& def_vars,
							vector<unsigned>& introduced_vars) {
    return true;
}

void Expression::replaceVarId(unsigned old_id,unsigned new_id) {}
		
void Expression::mergeSequences() {}

void Expression::rewriteVarsteps() {}

void Expression::extractProjectionTree(ProjectionTree* pt) {}

void Expression::extractPathEnvironment(PathEnvironment* penv) {}

bool Expression::subsumesVarInAllBranches(unsigned var) { return false; }

void Expression::extractFSAMap(FSAMap* fsa,unsigned parent_var) {}

Expression* Expression::placeUpdates(vector<UpdateExpression*>& udpates) {
    return this;
}

void Expression::eval(Environment* env, unsigned modus) {}
