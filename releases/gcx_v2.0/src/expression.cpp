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
 * 	@brief Implementations of header file expression.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file expression.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "expression.h"

Expression::Expression(EXP_TYPE _type) :
pp(PrettyPrint::getInstance()), tagmap(TagMap::getInstance()), type(_type) {}

Expression::~Expression() {}

bool Expression::scopeCheck(vector<unsigned>& def_vars,
                            vector<unsigned>& introduced_vars) {
    return true;
}

void Expression::replaceVarId(unsigned old_id, unsigned new_id) {}

void Expression::mergeSequences() {}

void Expression::rewriteVarsteps() {}

void Expression::extractFSAMap(FSAMap* fsamap, unsigned parent_var) {}

void Expression::extractParVarMap(ParVarMap* parvarmap) {}

void Expression::extractDependencies(vector<DependencySet*>* depset) {}

Expression* Expression::placeSignOffs(vector<SignOffExpression*>& signoffs) {
    return this;
}

void Expression::rewriteAggregateFuncts() {}

void* Expression::eval(Environment* env, unsigned modus) {
    return NULL;
}

bool Expression::containsDirectOutput() {
    return false;
}
