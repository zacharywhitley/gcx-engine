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
 * 	@brief Header file for role.cpp.
 * 	@details Header file specifying constructors, destructor and functions for role.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef ROLE_H
#define ROLE_H

#include <iostream>
#include "typeenums.h"

// forward declarations
class PathExpression;
class SignOffExpression;

//! @namespace std
using namespace std;

/*! @class Role
 * 	@brief
 * 	@details
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class Role {

public:
    Role(unsigned _id, ROLE_TYPE _type, unsigned _basing_var,
         unsigned _basing_fsa, PathExpression* _rel_path,
         PathExpression* _var_path);
    virtual ~Role();

    inline unsigned getId() { return id; }
    inline void setId(unsigned _id) { id=_id; }
    inline ROLE_TYPE getType() { return type; }
    inline unsigned getBasingVar() { return basing_var; }
    inline unsigned getBasingFSA() { return basing_fsa; }
    inline PathExpression* getConcatenationPath() { return concat_path; }
	bool isPosRole();
	bool isDosRole();

    SignOffExpression* getSignOffExpression();

    void print(ostream& o) const;
    virtual void printExtended();

private:
    unsigned id; // unique role identifier
    ROLE_TYPE type;
    unsigned basing_var;
    unsigned basing_fsa;
    PathExpression* rel_path;
    PathExpression* var_path; // path from the var (may be NULL)
    PathExpression* concat_path;
};

/*!
 * operator << overloading
 */
inline ostream& operator<<(ostream& o, const Role& role) {
    role.print(o);
    return o;
}

#endif // ROLE_H
