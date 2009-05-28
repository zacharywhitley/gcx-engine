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
 * 	@brief Header file for existscondexpression.cpp.
 * 	@details Header file specifying constructors, destructor and functions for existscondexpression.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef EXISTSCONDEXPRESSION_H
#define EXISTSCONDEXPRESSION_H

#include "condexpression.h"
#include "varstepexpression.h"
#include "bufferiterator.h"

/*! @class ExistsCondExpression
 * 	@brief Represents XQuery standard function <tt>fn:exists</tt>.
 * 	@details Represents XQuery standard function <tt>fn:exists</tt>, which takes either a
 * 			VarExpression, e.g. <tt>fn:exists(\$x)</tt>, or a VarStepExpression, e.g.
 * 			<tt>fn:exists(\$x/bib/book)</tt> or <tt>fn:exists(/bib//book)</tt> respectively, as argument
 * 			and returns <tt>true</tt> if the sequence located through the entered VarExpression
 * 			or the entered VarStepExpression respectively contains one or more items, <tt>false</tt>
 *  		otherwise.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class ExistsCondExpression : public CondExpression {

public:
	/*! @brief Constructor.
	 *  @details Constructor - creating object for a VarExpression.
	 * 	@param[in] _var Pointer to a VarExpression object.
	 */	
	ExistsCondExpression(VarExpression* _var);
	
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a VarStepExpression.
	 * 	@param[in] _varstep Pointer to a VarStepExpression object.
	 */
    ExistsCondExpression(VarStepExpression* _varstep);
    
	/*! @brief Destructor.
	 * 	@details Destructor.
	 */
    virtual ~ExistsCondExpression();

	/*! @fn virtual bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 *  @copydoc Expression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 */
    virtual bool scopeCheck(vector<unsigned>& def_vars,
                                vector<unsigned>& introduced_vars);
    
	/*! @fn virtual void replaceVarId(unsigned old_id, unsigned new_id)
	 *  @copydoc Expression::replaceVarId(unsigned old_id, unsigned new_id)
	 */
    virtual void replaceVarId(unsigned old_id, unsigned new_id);
    
	/*! @fn virtual void extractDependencies(vector<DependencySet*>* depset)
	 *  @copydoc Expression::extractDependencies(vector<DependencySet*>* depset)
	 */
	virtual void extractDependencies(vector<DependencySet*>* depset);
	
	/*! @fn virtual void print(ostream& o) const
	 *  @copydoc Expression::print(ostream& o) const
	 */
    virtual void print(ostream& o) const;

	/*! @fn virtual bool evalCond(Environment* env, unsigned modus)
	 *  @copydoc CondExpression::evalCond(Environment* env, unsigned modus)
	 */
    virtual bool evalCond(Environment* env, unsigned modus);

private:
	/*! @var VarExpression* var
	 * 	@brief The entered VarExpression.
	 * 	@details The entered VarExpression, which is the argument of the constructor.
	 * 			This member variable is <tt>NULL</tt> if the constructor was called with a VarStepExpression.
	 */
	VarExpression* var;
	
	/*! @var VarStepExpression* varstep
	 * 	@brief The entered VarStepExpression.
	 * 	@details The entered VarStepExpression, which is the argument of the constructor.
	 * 			This member variable is <tt>NULL</tt> if the constructor was called with a VarExpression. 
	 */
    VarStepExpression* varstep;
};

#endif // EXISTSCONDEXPRESSION_H
