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
 * 	@brief Header file for aggregatefunctexpression.cpp.
 * 	@details Header file specifying constructors, destructor and functions for aggregatefunctexpression.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef AGGREGATEFUNCTEXPRESSION_H
#define AGGREGATEFUNCTEXPRESSION_H

#include <iomanip>
#include "expression.h"
#include "varstepexpression.h"
#include "forexpression.h"
#include "miscfunctions.h"

/*! @class AggregateFunctExpression
 * 	@brief Represents base class of all aggregate function expressions.
 * 	@details Represents base class of all aggregate function expressions.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class AggregateFunctExpression : public Expression {

public:	
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for an EXP_TYPE and a VarExpression.
	 * 	@param[in] _type EXP_TYPE.
	 * 	@param[in] _var Pointer to a VarExpression object.
	 */
	AggregateFunctExpression(EXP_TYPE _type, VarExpression* _var);
	
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for an EXP_TYPE and a VarStepExpression.
	 * 	@param[in] _type EXP_TYPE.
	 * 	@param[in] _varstep Pointer to a VarStepExpression object.
	 */
	AggregateFunctExpression(EXP_TYPE _type, VarStepExpression* _varstep);
	
	/*! @brief Destructor.
	 * 	@details Destructor.
	 */
	virtual ~AggregateFunctExpression();	
	
	/*! @fn virtual bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 *  @copydoc Expression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 */
	virtual bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars);
	
	/*! @fn virtual void replaceVarId(unsigned old_id, unsigned new_id)
	 *  @copydoc Expression::replaceVarId(unsigned old_id, unsigned new_id)
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
	 *  @copydoc Expression::extractDependencies(vector<DependencySet*>* depset)
	 */
	virtual void extractDependencies(vector<DependencySet*>* depset);
	
	/*! @fn virtual Expression* placeSignOffs(vector<SignOffExpression*>& signoffs)
	 *  @copydoc Expression::placeSignOffs(vector<SignOffExpression*>& signoffs)
	 */
	virtual Expression* placeSignOffs(vector<SignOffExpression*>& signoffs);
	
	/*! @fn void rewriteAggregateFuncts()
	 *  @copydoc Expression::rewriteAggregateFuncts()
	 */
	void rewriteAggregateFuncts();

	/*! @fn virtual void print(ostream& o) const
	 *  @copydoc Expression::print(ostream& o) const
	 */
	virtual void print(ostream& o) const;
	
	/*! @fn virtual void* eval(Environment* env, unsigned modus)
	 *  @copydoc Expression::eval(Environment* env, unsigned modus)
	 */
	virtual void* eval(Environment* env, unsigned modus)=0;	
	
protected:
	/*! @var VarExpression* var
	 *  @brief The entered VarExpression.
	 *  @details The entered VarExpression, which is the second argument of the constructor.
	 * 			This member variable is <tt>NULL</tt> if the constructor was called with a
	 * 			VarStepExpression.
	 */
	VarExpression* var;
	
	/*! @var VarStepExpression* varstep
	 *  @brief The entered VarStepExpression.
	 *  @details The entered VarStepExpression, which is the second argument of the constructor.
	 *			This member variable is <tt>NULL</tt> if the constructor was called with a
	 * 			VarExpression.
	 */
	VarStepExpression* varstep;
	
	/*! @var ForExpression* forexp
	 *  @brief The rewritten ForExpression of the entered VarExpression or VarStepExpression.
	 *  @details The rewritten ForExpression of the entered VarExpression or VarStepExpression
	 * 			after calling method rewriteAggregateFuncts(). This member variable is
	 * 			<tt>NULL</tt> on creation of object but should not be <tt>NULL</tt> anymore
	 * 			after method rewriteAggregateFuncts() has been executed. If this member variable
	 * 			is still <tt>NULL</tt> (after execution of method rewriteAggregateFuncts()) an
	 * 			Error is thrown inside the eval() method of the	corresponding aggregate function.
	 */
	ForExpression* forexp;
};

#endif // AGGREGATEFUNCTEXPRESSION_H
