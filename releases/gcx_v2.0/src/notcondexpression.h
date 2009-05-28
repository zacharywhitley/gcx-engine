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
 * 	@brief Header file for notcondexpression.cpp.
 * 	@details Header file specifying constructors, destructor and functions for notcondexpression.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef NOTCONDEXPRESSION_H
#define NOTCONDEXPRESSION_H

#include "condexpression.h"

/*! @class NotCondExpression
 * 	@brief Represents XQuery standard function <tt>fn:not</tt>.
 * 	@details Represents XQuery standard function <tt>fn:not</tt>, which takes a
 * 			CondExpression, e.g. <tt>fn:not(fn:false())</tt> or <tt>fn:not(fn:count(\$x) < "3")</tt>,
 * 			as argument	and returns the negotiation of the entered CondExpression.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class NotCondExpression : public CondExpression {

public:
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a CondExpression.
	 * 	@param[in] _exp Pointer to a CondExpression object.
	 */
    NotCondExpression(CondExpression* _exp);
    
	/*! @brief Destructor.
	 * 	@details Destructor.
	 */
    virtual ~NotCondExpression();
    
	/*! @fn virtual bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 * 	@copydoc Expression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 */
    virtual bool scopeCheck(vector<unsigned>& def_vars,
                                    vector<unsigned>& introduced_vars);
    
	/*! @fn virtual void replaceVarId(unsigned old_id, unsigned new_id)
	 * 	@copydoc Expression::replaceVarId(unsigned old_id, unsigned new_id)
	 */
    virtual void replaceVarId(unsigned old_id, unsigned new_id);
    
	/*! @fn virtual void extractFSAMap(FSAMap* fsamap, unsigned parent_var)
	 * 	@copydoc Expression::extractFSAMap(FSAMap* fsamap, unsigned parent_var)
	 */
    virtual void extractFSAMap(FSAMap* fsamap, unsigned parent_var);
    	
	/*! @fn virtual void extractParVarMap(ParVarMap* parvarmap)
	 * 	@copydoc Expression::extractParVarMap(ParVarMap* parvarmap)
	 */
	virtual void extractParVarMap(ParVarMap* parvarmap);
    	
	/*! @fn virtual void extractDependencies(vector<DependencySet*>* depset)
	 * 	@copydoc Expression::extractDependencies(vector<DependencySet*>* depset)
	 */
	virtual void extractDependencies(vector<DependencySet*>* depset);
	
	/*! @fn virtual Expression* placeSignOffs(vector<SignOffExpression*>& signoffs)
	 *  @copydoc Expression::placeSignOffs(vector<SignOffExpression*>& signoffs)
	 */
	virtual Expression* placeSignOffs(vector<SignOffExpression*>& signoffs);
	
	/*! @fn virtual void rewriteAggregateFuncts()
	 * 	@copydoc Expression::rewriteAggregateFuncts()
	 */
    virtual void rewriteAggregateFuncts();

	/*! @fn virtual void print(ostream& o) const
	 * 	@copydoc Expression::print(ostream& o) const
	 */
    virtual void print(ostream& o) const;

	/*! @fn virtual void* eval(Environment* env, unsigned modus)
	 * 	@copydoc Expression::eval(Environment* env, unsigned modus)
	 */
	virtual void* eval(Environment* env, unsigned modus);

	/*! @fn virtual bool evalCond(Environment* env, unsigned modus)
	 * 	@copydoc CondExpression::evalCond(Environment* env, unsigned modus)
	 */
	virtual bool evalCond(Environment* env, unsigned modus);

private:
	/*! @var CondExpression* exp
	 *  @brief The entered CondExpression.
	 * 	@details The entered CondExpression, which is the argument of the constructor.
	 */
    CondExpression* exp;
};

#endif // NOTCONDEXPRESSION_H
