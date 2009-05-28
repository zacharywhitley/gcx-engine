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
 * 	@brief Header file for signoffexpression.cpp.
 * 	@details Header file specifying constructors, destructor and functions for signoffexpression.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef SIGNOFFEXPRESSION_H
#define SIGNOFFEXPRESSION_H

#include "expression.h"
#include "varstepexpression.h"
#include "bufferiterator.h"

/*! @class SignOffExpression
 * 	@brief Represents a single (signOff-) expression of the form <tt>signOff(variable/path, role)</tt>.
 * 	@details Represents a single (signOff-) expression of the form <tt>signOff(variable/path, role)</tt>, which takes therefore either a
 * 			VarExpression, e.g. <tt>signOff(\$x, r<sub>i</sub>)</tt>, or a VarStepExpression, e.g.
 * 			<tt>signOff(\$x/bib/book/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>(), r<sub>i</sub>)</tt>
 * 			or <tt>signOff(\$root/bib//book[position</tt><tt>()=1], r<sub>i</sub>)</tt> respectively,
 * 			and a Role as arguments and removes this certain role from all affected buffer nodes, i.e.
 * 			removes role <tt>r<sub>i</sub></tt> from all BufferNode that are located through
 * 			the entered VarExpression or the entered VarStepExpression respectively.
 *  @note Variables are internally represented by numbers!
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class SignOffExpression : public Expression {

public:
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a VarExpression and a Role.
	 * 	@param[in] _var Pointer to a VarExpression object.
	 *  @param[in] _role Pointer to a Role object.
	 */
    SignOffExpression(VarExpression* _var, Role* _role);
    
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a VarStepExpression and a Role.
	 * 	@param[in] _varstep Pointer to a VarStepExpression object.
	 *  @param[in] _role Pointer to a Role object.
	 */
    SignOffExpression(VarStepExpression* _varstep, Role* _role);
    
	/*! @brief Destructor.
	 * 	@details Destructor.
	 */
    virtual ~SignOffExpression();
    
	/*! @fn virtual bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 *  @copydoc Expression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 */
    virtual bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars);
    
	/*! @fn virtual void replaceVarId(unsigned old_id, unsigned new_id)
	 *  @copydoc Expression::replaceVarId(unsigned old_id, unsigned new_id)
	 */
    virtual void replaceVarId(unsigned old_id, unsigned new_id);

	/*! @fn virtual void print(ostream& o) const
	 *  @copydoc Expression::print(ostream& o) const
	 */
    virtual void print(ostream& o) const;
    
	/*! @fn virtual void* eval(Environment* env, unsigned modus)
	 *  @copydoc Expression::eval(Environment* env, unsigned modus)
	 */
    virtual void* eval(Environment* env, unsigned modus);

	/*! @fn virtual unsigned getVar()
	 *  @brief Returns the (numeric value of the) variable from the underlying VarExpression or VarStepExpression respectively.
	 *  @details Returns the (numeric value of the) variable from the underlying VarExpression or VarStepExpression respectively.
	 *  @retval unsigned Numeric value of the variable from the underlying VarExpression or VarStepExpression respectively.
	 */
    virtual unsigned getVar();

private:
	/*! @var VarExpression* var
	 *  @brief The entered VarExpression.
	 *  @details The entered VarExpression, which is the first argument of
	 *  the constructor. This member variable is <tt>NULL</tt> if the
	 *  constructor was called with a VarStepExpression.
	 */
	VarExpression* var;
	
	/*! @var VarStepExpression* varstep
	 *  @brief The entered VarStepExpression.
	 *  @details The entered VarStepExpression, which is the first argument of
	 *  the constructor. This member variable is <tt>NULL</tt> if the constructor
	 *  was called with a VarExpression.
	 */
	VarStepExpression* varstep;

	/*! @var VarStepExpression* exec_varstep
     *  @brief The varstep expression to be executed.
     *  @details This expression differes from varstep in that it, if the final
     *           pathstep of varstep is dos::node(), it is omited. Otherwise,
     *           it is equal to varstep.
     */
	VarStepExpression* exec_varstep; 
	
	/*! @var Role* role
	 *  @brief The entered Role.
	 *  @details The entered Role, which is the second argument of the constructor and defines
	 * 			the Role which should be be removed from all through VarExpression or VarStepExpression
	 * 			respectively located BufferNode.
	 */
    Role* role;    
};

#endif // SIGNOFFEXPRESSION_H
