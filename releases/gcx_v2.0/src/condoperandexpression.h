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
 * 	@brief Header file for condoperandexpression.cpp.
 * 	@details Header file specifying constructors, destructor and functions for condoperandexpression.cpp.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef CONDOPERANDEXPRESSION_H
#define CONDOPERANDEXPRESSION_H

#include "condexpression.h"
#include "operandexpression.h"

/*! @class CondOperandExpression
 * 	@brief Represents XQuery comparison of two operands with relational operators
 * 			<tt>\<</tt>, <tt>\<=</tt>, <tt>\>=</tt>, <tt>\></tt>, <tt>=</tt> or <tt>!=</tt>.
 * 	@details Represents XQuery comparison of two operands with relative operators
 * 			<tt>\<</tt>, <tt>\<=</tt>, <tt>\>=</tt>, <tt>\></tt>, <tt>=</tt> or <tt>!=</tt>,
 * 			which takes therefore two OperandExpression, e.g. <tt>\$x/bib/book</tt>, <tt>"foo"</tt> or <tt>fn:count(\$x)</tt>,
 * 			and a COMP_TYPE, e.g. <tt>=</tt> or <tt>!=</tt>, as arguments and returns the
 * 			evaluated value of the two OperandExpression with respect to COMP_TYPE,
 * 			i.e. returns <tt>true</tt> if the two OperandExpression are in correct
 * 			relationship to the COMP_TYPE, <tt>false</tt> otherwise.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class CondOperandExpression : public CondExpression {

public:
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a COMP_TYPE and two OperandExpression.
	 * 	@param[in] _left Pointer to a OperandExpression object.
	 * 	@param[in] _op COMP_TYPE.
	 * 	@param[in] _right Pointer to a OperandExpression object.
	 */	
	CondOperandExpression(OperandExpression* _left, COMP_TYPE _op, OperandExpression* _right);
    
	/*! @brief Destructor.
	 * 	@details Destructor.
	 */
    virtual ~CondOperandExpression();

	/*! @fn virtual bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 *  @copydoc Expression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 */
    virtual bool scopeCheck(vector<unsigned>& def_vars,
                                vector<unsigned>& introduced_vars);
    
	/*! @fn virtual void replaceVarId(unsigned old_id, unsigned new_id)
	 * 	@copydoc Expression::replaceVarId(unsigned old_id, unsigned new_id)
	 */
    virtual void replaceVarId(unsigned old_id, unsigned new_id);
    
	/*! @fn virtual void extractFSAMap(FSAMap* fsamap, unsigned parent_var)
	 *  @copydoc Expression::extractFSAMap(FSAMap* fsamap, unsigned parent_var)
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
	 * 	@copydoc Expression::placeSignOffs(vector<SignOffExpression*>& signoffs)
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

	/*! @fn virtual bool evalCond(Environment* env, unsigned modus)
	 *  @copydoc CondExpression::evalCond(Environment* env, unsigned modus)
	 */
    virtual bool evalCond(Environment* env, unsigned modus);

private:
    /*! @fn bool evalRelOp(const char* left, COMP_TYPE op, const char* right)
     * 	@brief Evaluates the two entered strings with respect to the entered COMP_TYPE.
     * 	@details Evaluates the two entered strings with respect to the entered COMP_TYPE
     * 			to <tt>true</tt> or <tt>false</tt>.
     * 	@param[in] left Pointer to a char object.
     * 	@param[in] op COMP_TYPE.
     *  @param[in] right Pointer to a char object.
     * 	@retval bool <tt>true</tt> if the two strings are in correct relationship with respect to the
     * 			entered COMP_TYPE, <tt>false</tt> otherwise.
     */
	bool evalRelOp(const char* left, COMP_TYPE op, const char* right);
	
	/*! @var OperandExpression* left
	 * 	@brief The entered OperandExpression on the left side of the entered COMP_TYPE.
	 * 	@details The entered OperandExpression on the left side of the entered COMP_TYPE,
	 * 			which is the first argument of the constructor.
	 */
	OperandExpression* left;
	
	/*! @var COMP_TYPE op
	 * 	@brief The entered COMP_TYPE between the two entered OperandExpression.
	 * 	@details The entered COMP_TYPE between the two entered OperandExpression,
	 * 			which is the second argument of the constructor.
	 */
	COMP_TYPE op;
	
	/*! @var OperandExpression* right
	 *  @brief The entered OperandExpression on the right side of the entered COMP_TYPE.
	 *  @details The entered OperandExpression on the right side of the entered COMP_TYPE,
	 * 			which is the third argument of the constructor.
	 */
	OperandExpression* right;
};

#endif // CONDOPERANDEXPRESSION_H
