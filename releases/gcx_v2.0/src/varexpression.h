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
 * 	@brief Header file for varexpression.cpp.
 * 	@details Header file specifying constructors, destructor and functions for varexpression.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef VAREXPRESSION_H
#define VAREXPRESSION_H

#include "expression.h"
#include "bufferiterator.h"

/*! @class VarExpression
 * 	@brief Represents an XQuery variable.
 * 	@details Represents an XQuery variable, e.g. <tt>\$x</tt>, <tt>\$z</tt> or <tt>\$root</tt>,
 * 			which takes a string as argument and returns the current binding to a BufferNode for a variable.
 *  @note Variables are internally represented by numbers!
 * 	@note To get a variable name from a numeric value a look up in VarName is needed!
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class VarExpression : public Expression {

public:
	/*! @brief Constructor.
	 *  @details Constructor - creating object for variable <tt>\$root</tt>.
	 *  @param[in] _print_var <tt>true</tt> if variable should be printed, <tt>false</tt> otherwise.
	 */
    VarExpression(bool _print_var);
    
	/*! @brief Constructor.
	 *  @details Constructor - creating object for a string.
	 * 	@param[in] _name Pointer to a char object.
	 *  @param[in] _print_var <tt>true</tt> if variable should be printed, <tt>false</tt> otherwise.
	 */
    VarExpression(const char* _name, bool _print_var);
    
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a string.
	 * 	@param[in] _name Pointer to a char object.
	 * 	@param[in] _force <tt>true</tt> if force insertion of variable into VarName, <tt>false</tt> otherwise.
	 *  @param[in] _print_var <tt>true</tt> if variable should be printed, <tt>false</tt> otherwise.
	 */
    VarExpression(const char* _name, bool _force, bool _print_var);
    
	/*! @brief Destructor.
	 * 	@details Destructor.
	 */
    virtual ~VarExpression();
    
	/*! @fn bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 *  @copydoc Expression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 */
    bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars);
    
	/*! @fn void replaceVarId(unsigned old_id, unsigned new_id)
	 * 	@copydoc Expression::replaceVarId(unsigned old_id, unsigned new_id)
	 */
    void replaceVarId(unsigned old_id, unsigned new_id);
    
	/*! @fn void extractDependencies(vector<DependencySet*>* depset)
	 *  @copydoc Expression::extractDependencies(vector<DependencySet*>* depset)
	 */
	void extractDependencies(vector<DependencySet*>* depset);
	
	/*! @fn void print(ostream& o) const
	 * 	@copydoc Expression::print(ostream& o) const
	 */
	void print(ostream& o) const;
	    
	/*! @fn void* eval(Environment* env, unsigned modus)
	 *  @copydoc Expression::eval(Environment* env, unsigned modus)
	 */
	void* eval(Environment* env, unsigned modus);
	
    /*! @fn inline void setId(unsigned _id)
     *  @brief Sets a new (numeric) value for a variable.
     *  @details Sets a new (numeric) value for a variable, i.e. sets a new (numeric) value for the member variable.
     *  @param[in] _id New (numeric) value for a variable.
     *  @retval void
	 */
	inline void setId(unsigned _id) { id=_id; }
	
    /*! @fn inline unsigned getId()
     *  @brief Returns the current (numeric) value of a variable.
     *  @details Returns the current (numeric) value of a variable, i.e. returns the member variable.
     *  @retval unsigned Current (numeric) value of a variable.
	 */
    inline unsigned getId() { return id; }

    /*! @fn VarExpression* clone()
     *  @brief Returns a new, independent copy of a variable or of this object respectively.
     *  @details Returns a new, independent copy of a variable or of this object respectively.
     *  @retval VarExpression* Copy of a variable or of this object respectively.
	 */
    VarExpression* clone();

private:
	/*! @var unsigned id
	 * 	@brief The entered variable.
	 * 	@details The entered variable, which is the (first) argument of the constructor and
	 * 			internally represented by a number.
	 */
    unsigned id;
    
	/*! @var bool print_var
	 * 	@brief The entered boolean value if variable should be printed.
	 * 	@details The entered boolean value if variable should be printed, which is the (second) argument of the constructor.
	 */
    bool print_var;
};

#endif // VAREXPRESSION_H
