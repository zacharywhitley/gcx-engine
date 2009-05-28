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
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include "environment.h"
#include "expenums.h"
#include "prettyprint.h"
#include <vector>
#include "fsamap.h"
#include "tagmap.h"
#include "projectiontree.h"
#include "rolelist.h"

// forward declarations
class PathEnvironment;
class ProjectionPaths;
class UpdateExpression;

using namespace std;

/*!
 * virtual base class for SSXQ expressions
*/
class Expression {

public:
    Expression(EXP_TYPE _type);
    virtual ~Expression();
    
    virtual void print(ostream& o) const=0;
    virtual bool scopeCheck(vector<unsigned>& def_vars,
							vector<unsigned>& introduced_vars);
    virtual void replaceVarId(unsigned old_id,unsigned new_id);
    inline const EXP_TYPE getType() { return type; }

    // optimization
    virtual void mergeSequences();
	virtual void rewriteVarsteps();

    // information extraction and rewriting
    virtual void extractProjectionTree(ProjectionTree* pt);
	virtual void extractPathEnvironment(PathEnvironment* penv);
	virtual bool subsumesVarInAllBranches(unsigned var);
    virtual void extractFSAMap(FSAMap* fsa,unsigned parent_var);
    virtual Expression* placeUpdates(vector<UpdateExpression*>& updates);

    // evaluation
    static unsigned EVAL_QUERY;
    static unsigned EVAL_UPD;
    static unsigned EVAL_UPD_IN_SEQ;
    virtual void eval(Environment* env, unsigned modus);
    
protected:
    
    PrettyPrint* pp;
    TagMap* tagmap;
    EXP_TYPE type;
    
};

/*!
 * operator << overloading
*/
inline ostream& operator<< (ostream& o, const Expression& exp) {
   exp.print(o);
   return o;
}

#endif // EXPRESSION_H
