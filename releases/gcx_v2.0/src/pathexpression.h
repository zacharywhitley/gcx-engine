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
 *     @brief Header file for pathexpression.cpp.
 *     @details Header file specifying constructors, destructor and functions for pathexpression.cpp.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#ifndef PATHEXPRESSION_H
#define PATHEXPRESSION_H

#include "expression.h"
#include "pathexpressionadornment.h"
#include "pathsteptagexpression.h"

// forward declarations
class PathExpressionAdornment;

/*! @class PathExpression
 *     @brief
 *     @details
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
class PathExpression : public Expression {

public:
    PathExpression();
    virtual ~PathExpression();

    inline vector<PathStepExpression*>* getPathSteps() { return &pathsteps; }
    inline void addPathStep(PathStepExpression* ps) { pathsteps.push_back(ps); }
    inline unsigned getPathSize() { return pathsteps.size(); }
    inline PathStepExpression* getPathStepAt(unsigned i) { return pathsteps[i]; }
    
    PathStepExpression* getTailPathStep();
    
    PathStepExpression* getPathStepAfterTextNodeTest();

    inline bool isEmptyPath() { return pathsteps.size()==0; } 

    bool isDosNodePath();
    bool hasInnerTextNodeTest();
    bool hasTerminatingTextNodeTest();
    bool hasFollowingDescendantOrDosFrom(unsigned ps_idx);
    bool hasPreviousDescendantOrDosUpTo(unsigned ps_idx);
    
    bool isSyntacticallyEqualTo(PathExpression* path);

    // @note The current implementations do not consider PathStepAttribute and therefore
    //       invoking this method on any path would maybe lead to wrong results!!!
    bool isSemanticallyContainedIn(PathExpression* path);
    
    unsigned getWeight();
    
    void replacePathStepAt(unsigned idx, PathStepExpression* ps);
    PathExpressionAdornment* getAdornment(); 
    bool selectsNoNode();
    bool containsStarDescendantSequence(unsigned pos);
    
    PathExpression* clone();
    PathExpression* cloneWithoutFinalDosNodeAndAttributes();

    virtual void print(ostream& o) const;
    bool mightHasChildDescendantConflict(PathExpression* path);

private:
    vector<PathStepExpression*> pathsteps;
    PathExpressionAdornment* adornment;
};

#endif // PATHEXPRESSION_H
