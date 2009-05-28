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
#ifndef ROLE_H
#define ROLE_H

#define ROLETYPE_VARIABLE	 	0
#define ROLETYPE_EXISTENCE	 	1
#define ROLETYPE_NONEXISTENCE	2

#include <iostream>

class PathExpression;
class PathStepExpression;
class UpdateExpression;

using namespace std;

class Role {

public:
	Role(unsigned _id,
			unsigned _type,
			unsigned _basing_var,
			unsigned _basing_fsa,
			PathExpression* _rel_path,
			PathStepExpression* _ps);
	virtual ~Role();

	inline unsigned getId() { return id; }
	inline unsigned getType() { return type; }
	inline unsigned getBasingVar() { return basing_var; }
	inline unsigned getBasingFSA() { return basing_fsa; }
	inline PathExpression* getRelPath() { return rel_path; }
	inline PathStepExpression* getPathStep() { return ps; }

	UpdateExpression* toUpdateExpression();
	
	void print();
	
private:
	unsigned id; // unique role identifier
	
	unsigned type;
	unsigned basing_var;
	unsigned basing_fsa;
	PathExpression* rel_path;
	PathStepExpression* ps; // path step from the var (may be NULL)
};

#endif // ROLE_H
