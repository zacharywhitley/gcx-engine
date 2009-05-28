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
 * 	@brief Header file for miscfunctions.cpp.
 * 	@details Header file specifying constructors, destructor and functions for miscfunctions.cpp.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef MISCFUNCTIONS_H
#define MISCFUNCTIONS_H

#include <sstream>
#include <math.h>
#include <string>
#include <errno.h>

//! @namespace std
using namespace std; 

/*! @def ROUND_TO_DECIMAL_PLACE
 *  @brief The number of right-of-comma positions from which is started rounding.
 *  @details The number of right-of-comma positions from which is started rounding.
 */
#define ROUND_TO_DECIMAL_PLACE 16

/*! @def REPLACE_DECIMAL_POINT_BEFORE
 *  @brief Enable replacing of german comma-character <tt>,</tt> to english comma-character
 * 			<tt>.</tt> in strings first before converting into a numeric value.
 *  @details Enable replacing of german comma-character <tt>,</tt> to english comma-character
 * 			<tt>.</tt> in strings first before converting into a numeric value.
 */
#define REPLACE_DECIMAL_POINT_BEFORE true

/*! @def REPLACE_DECIMAL_POINT_AFTER
 *  @brief Enable replacing of english comma-character <tt>.</tt> to german comma-character
 *  		<tt>,</tt> in numeric values after all calculation have been made.
 *  @details Enable replacing of english comma-character <tt>.</tt> to german comma-character
 *  		<tt>,</tt> in numeric values after all calculation have been made.
 */
#define REPLACE_DECIMAL_POINT_AFTER false

/*! @class MiscFunctions
 * 	@brief Contains different static help functions.
 * 	@details The functions comprise type conversion functions and math functions.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class MiscFunctions {

public:	
	/*! @brief Converts const char* to long double.
	 *  @details
	 *  @param[in] str The string to be converted.
	 *  @exception Error Throws an error if conversion fails.
	 *  @retval long double The converted value. 
	 */
	static long double getNumericFromString(const char* str);
	
	/*! @brief Converts a long double to const char*.
	 *  @details The caller is responsible for deleting the return value using delete[].
	 *  @param[in] num The string to be converted.
	 *  @exception Error Throws an error if conversion fails.
	 *  @retval const char* The converted value.
	 */
	static const char* getStringFromNumerical(long double num); 
	
	/*! @brief Computes the sum of two long double values.
	 *  @param[in] left The left operand.
	 *  @param[in] right The right operand.
	 *  @exception Error Throws an error if summation fails. 
	 *  @retval long double The computed sum.
	 */ 
	static long double getSummationFrom(long double left, long double right);
	
	/*! @brief Computes the difference of two lrong double values.
	 *  @param[in] left The left operand.
	 *  @param[in] right The right operand.
	 *  @exception Error Throws an error if difference computation fails. 
	 *  @retval long double The computed difference.
	 */ 
	static long double getSubtractionFrom(long double left, long double right);
	
	/*! @brief Computes the power of two long doubles. 
	 *  @param[in] base The base.
	 *  @param[in] exponent The exponent.
	 *  @exception Error Throws an error if power computation fails. 
	 *  @retval long double The computed power.
	 */ 
	static long double getPowerFrom(long double base, long double exponent);
	
	/*! @brief Computes the radical of two long doubles. 
	 *  @param[in] base The base.
	 *  @param[in] radix The radix.
	 *  @exception Error Throws an error if radical computation fails. 
	 *  @retval long double The computed radical.
	 */ 
	static long double getRadicalFrom(long double base, long double radix);
	
	/*! @brief Computes the division of two long double values.
	 *  @param[in] numerator The numerator.
	 *  @param[in] denominator The denominator.
	 *  @exception Error Throws an error if division fails. 
	 *  @retval long double The computed division.
	 */ 
	static long double getDivisionFrom(long double numerator, long double denominator);
	
	/*! @brief Round a long double value.
	 *  @details The standard decimal places are defined by ROUND_TO_DECIMAL_PLACE,
	 *           but may be specified optionally.
	 *  @param[in] num The value to be rounded.
	 *  @param[in] places The number of decimal places.
	 *  @retval long double The rounded number.
	 */
	static long double getRoundFrom(long double num, unsigned places=ROUND_TO_DECIMAL_PLACE);

	/*! @brief Removes whitespaces at beginning and end of string 
     *  @details The returned const char* has to be deleted using delete[].
	 *  @param[in] s The string to be trimmed. 
	 *  @retval const char* The trimmed string.
	 */
	static const char* trim(const char* s);

	/*! @brief Checks whether a string needs to be trimmed.
     *  @details Returns true if there is a space character at the first or last position.
     *  @param[in] s The string to be checked.
     *  @retval bool True if space needs to be trimmed.
     */
	static bool needsTrim(const char*);
};

#endif // MISCFUNCTIONS_H
