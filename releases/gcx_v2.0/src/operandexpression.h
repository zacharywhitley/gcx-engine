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
 * 	@brief Header file for operandexpression.cpp.
 * 	@details Header file specifying constructors, destructor and functions for operandexpression.cpp.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef OPERANDEXPRESSION_H
#define OPERANDEXPRESSION_H

#include "expression.h"
#include "varstepexpression.h"
#include "aggregatefunctexpression.h"
#include "constexpression.h"

/*! @class OperandExpression
 * 	@brief Represents an XQuery operand of a condition.
 * 	@details Represents an XQuery operand of a condition, i.e. represents a operand of a CondOperandExpression,
 * 			which takes either a VarExpression, e.g. <tt>\$x</tt>, <tt>\$z</tt>	or <tt>\$root</tt>,
 * 			a VarStepExpression, e.g. <tt>\$x/bib//book</tt> or <tt>//node()/text()</tt>, a AggregateFunctExpression,
 * 			e.g. <tt>fn:count(\$x)</tt> or <tt>fn:sum(/bib//book)</tt>, or a ConstExpression, e.g. <tt>"foo"</tt>,
 * 			as argument and returns the evaluated value of the corresponding Expression.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class OperandExpression : public Expression {

public:
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a VarExpression.
	 * 	@param[in] _var Pointer to a VarExpression object.
	 */
    OperandExpression(VarExpression* _var);
    
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a VarStepExpression.
	 * 	@param[in] _varstep Pointer to a VarStepExpression object.
	 */
    OperandExpression(VarStepExpression* _varstep);
    
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a AggregateFunctExpression.
	 * 	@param[in] _aggfunct Pointer to a AggregateFunctExpression object.
	 */
    OperandExpression(AggregateFunctExpression* _aggfunct);
    
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a ConstExpression.
	 * 	@param[in] _cstring Pointer to a ConstExpression object.
	 */
    OperandExpression(ConstExpression* _cstring);
    
	/*! @brief Destructor.
	 * 	@details Destructor.
	 */
    virtual ~OperandExpression();
    
	/*! @fn virtual bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 * 	@copydoc Expression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 */
	virtual bool scopeCheck(vector<unsigned>& def_vars,
	                                vector<unsigned>& introduced_vars);
	
	/*! @fn virtual void replaceVarId(unsigned old_id, unsigned new_id)
	 *  @copydoc Expression::replaceVarId(unsigned old_id, unsigned new_id)
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
     
	/*! @fn virtual void* eval(Environment* env, unsigned modus)
	 * 	@copydoc Expression::eval(Environment* env, unsigned modus)
	 */
    virtual void* eval(Environment* env, unsigned modus);
    
    // TODO: document this!!!
    virtual Expression* getExp();    
    
private:
	/*! @var VarExpression* var
	 * 	@brief The entered VarExpression.
	 * 	@details The entered VarExpression, which is the argument of the constructor.
	 * 			This member variable is <tt>NULL</tt> if the constructor was called with a VarStepExpression,
	 * 			a AggregateFunctExpression or a ConstExpression.
	 */
	VarExpression* var;
	
	/*! @var VarStepExpression* varstep
	 * 	@brief The entered VarStepExpression.
	 * 	@details The entered VarStepExpression, which is the argument of the constructor.
	 * 			This member variable is <tt>NULL</tt> if the constructor was called with a VarExpression,
	 * 			a AggregateFunctExpression or a ConstExpression.
	 */
	VarStepExpression* varstep;
	
	/*! @var AggregateFunctExpression* aggfunct
	 * 	@brief The entered AggregateFunctExpression.
	 * 	@details The entered AggregateFunctExpression, which is the argument of the constructor.
	 * 			This member variable is <tt>NULL</tt> if the constructor was called with a VarExpression,
	 * 			a VarStepExpression or a ConstExpression.
	 */
	AggregateFunctExpression* aggfunct;
	
	/*! @var ConstExpression* cstring
	 * 	@brief The entered ConstExpression.
	 * 	@details The entered ConstExpression, which is the argument of the constructor.
	 * 			This member variable is <tt>NULL</tt> if the constructor was called with a VarExpression,
	 * 			a VarStepExpression or a AggregateFunctExpression.
	 */
	ConstExpression* cstring;
};

#endif // OPERANDEXPRESSION_H
