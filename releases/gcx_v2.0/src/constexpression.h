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
 * 	@brief Header file for constexpression.cpp.
 * 	@details Header file specifying constructors, destructor and functions for constexpression.cpp.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef CONSTEXPRESSION_H
#define CONSTEXPRESSION_H

#include <sstream>
#include "expression.h"

/*! @class ConstExpression
 * 	@brief Represents an XQuery string value.
 * 	@details Represents an XQuery string value, e.g. <tt>"foo"</tt>,
 * 			which takes therefore a string as argument and returns this string.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class ConstExpression : public Expression {

public:
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a string.
	 * 	@param[in] _text Pointer to a char object.
	 */
    ConstExpression(const char* _text);
    
	/*! @brief Destructor.
	 * 	@details Destructor.
	 */
    virtual ~ConstExpression();

	/*! @fn void print(ostream& o) const
	 *  @copydoc Expression::print(ostream& o) const
	 */
    void print(ostream& o) const;
    
	/*! @fn void* eval(Environment* env, unsigned modus)
	 *  @copydoc Expression::eval(Environment* env, unsigned modus)
	 */
    void* eval(Environment* env, unsigned modus);

private:
	/*! @var const char* text
	 *  @brief The entered string.
	 * 	@details The entered string, which is the argument of the constructor.
	 */
    const char* text;
};

#endif // CONSTEXPRESSION_H
