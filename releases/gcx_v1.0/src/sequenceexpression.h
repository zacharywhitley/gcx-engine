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
#ifndef SEQUENCEEXPRESSION_H
#define SEQUENCEEXPRESSION_H

#include "expression.h"
#include "updateexpression.h"

class SequenceExpression : public Expression {

public:
    SequenceExpression(Expression* _left,Expression*_right);
    virtual ~SequenceExpression();

    inline vector<Expression*>* getExps() { return &exps; }

    virtual void print(ostream& o) const;
    virtual bool scopeCheck(vector<unsigned>& def_vars,
							vector<unsigned>& introduced_vars);
	virtual void replaceVarId(unsigned old_id,unsigned new_id);

    virtual void mergeSequences();
	virtual void rewriteVarsteps();
    virtual void extractProjectionTree(ProjectionTree* pt);
	virtual void extractPathEnvironment(PathEnvironment* penv);
	virtual bool subsumesVarInAllBranches(unsigned var);
	
    virtual void eval(Environment* env,unsigned modus);
    virtual void extractFSAMap(FSAMap* fsamap,unsigned parent_var);
    virtual Expression* placeUpdates(vector<UpdateExpression*>& updates);

private:
    vector<Expression*> exps;
};

#endif // SEQUENCEEXPRESSION_H
