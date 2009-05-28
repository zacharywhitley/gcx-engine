
/*
 | Author: Michael Schmidt;
 |         Gunnar Jehl 
 |
 | ************************* SOFTWARE LICENSE AGREEMENT ***********************
 | This source code is published under the BSD License.
 |
 | See file 'LICENSE.txt' that comes with this distribution or
 | http://dbis.informatik.uni-freiburg.de/index.php?project=GCX/license.php
 | for the full license agreement.
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
 | ****************************************************************************
*/

/*! @file
 *  @brief Header file for miscfunctions.cpp.
 *  @details Header file specifying constructors, destructor and functions for miscfunctions.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef MISCFUNCTIONS_H
#define MISCFUNCTIONS_H

#include <sstream>
#include <math.h>
#include <string>
#include <errno.h>
#include "invalidcastexception.h"

//! @namespace std
using namespace std;

/*! @def ROUND_TO_DECIMAL_PLACE
 *  @brief The number of right-of-comma positions from which is started rounding.
 *  @details The number of right-of-comma positions from which is started rounding.
 */
#define ROUND_TO_DECIMAL_PLACE 16

/*! @def REPLACE_DECIMAL_POINT_BEFORE
 *  @brief Enable replacing of german comma-character <tt>,</tt> to english comma-character
 *    <tt>.</tt> in strings first before converting into a numeric value.
 *  @details Enable replacing of german comma-character <tt>,</tt> to english comma-character
 *    <tt>.</tt> in strings first before converting into a numeric value.
 */
#define REPLACE_DECIMAL_POINT_BEFORE	true

/*! @def REPLACE_DECIMAL_POINT_AFTER
 *  @brief Enable replacing of english comma-character <tt>.</tt> to german comma-character
 *    <tt>,</tt> in numeric values after all calculation have been made.
 *  @details Enable replacing of english comma-character <tt>.</tt> to german comma-character
 *    <tt>,</tt> in numeric values after all calculation have been made.
 */
#define REPLACE_DECIMAL_POINT_AFTER		false

/*! @class MiscFunctions
 *  @brief Represents helper class with methods needed for conversion and mathematical calculations.
 *  @details Represents helper class with methods needed for conversion and mathematical calculations.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class MiscFunctions {

  public:
    /*! @fn static long double getNumericFromString(const char* str)
     *  @brief Converts a string into a numeric value.
     *  @details Converts a string into a numeric value.
     *  @param[in] str Pointer to a char object (string to be converted).
     *  @exception InvalidCastException If conversion fails.
     *  @retval long double The converted numeric value. 
     */
    static long double getNumericFromString(const char *str);

    /*! @fn static const char* getStringFromNumerical(long double num)
     *  @brief Converts a numeric value into a string.
     *  @details Converts a numeric value into a string.
     *  @param[in] num Numeric value to be converted.
     *  @exception InvalidCastException If conversion fails.
     *  @retval const char* The converted string value. 
     */
    static const char *getStringFromNumerical(long double num);

    /*! @fn static long double getSummationFrom(long double left, long double right)
     *  @brief Calculates the summation of two numeric values.
     *  @details Calculates the summation of two numeric values.
     *  @param[in] left The summand on the left hand side.
     *  @param[in] right The summand on the right hand side.
     *  @exception InvalidCastException If calculation fails.
     *  @retval long double The result of the summation. 
     */
    static long double getSummationFrom(long double left, long double right);

    /*! @fn static long double getSubtractionFrom(long double left, long double right)
     *  @brief Calculates the subtraction of two numeric values.
     *  @details Calculates the subtraction of two numeric values.
     *  @param[in] left The subtrahend on the left hand side.
     *  @param[in] right The subtrahend on the right hand side.
     *  @exception InvalidCastException If calculation fails.
     *  @retval long double The result of the subtraction. 
     */
    static long double getSubtractionFrom(long double left, long double right);

    /*! @fn static long double getPowerFrom(long double base, long double exponent)
     *  @brief Calculates the power of a number (base) to an exponent.
     *  @details Calculates the power of a number (base) to an exponent.
     *  @param[in] base The base.
     *  @param[in] exponent The exponent.
     *  @exception InvalidCastException If calculation fails.
     *  @retval long double The result. 
     */
    static long double getPowerFrom(long double base, long double exponent);

    /*! @fn static long double getRadicalFrom(long double base, long double radix)
     *  @brief Calculates the radical of a number (base) to an radix.
     *  @details Calculates the radical of a number (base) to an radix.
     *  @param[in] base The base.
     *  @param[in] radix The radix.
     *  @exception InvalidCastException If calculation fails.
     *  @retval long double The result. 
     */
    static long double getRadicalFrom(long double base, long double radix);

    /*! @fn static long double getDivisionFrom(long double numerator, long double denominator)
     *  @brief Calculates the division of two numbers.
     *  @details Calculates the division of two numbers.
     *  @param[in] numerator The numerator.
     *  @param[in] denominator The denominator.
     *  @exception InvalidCastException If calculation fails.
     *  @retval long double The result of the division. 
     */
    static long double getDivisionFrom(long double numerator,
                                       long double denominator);

    /*! @fn static long double getRoundFrom(long double num, unsigned places)
     *  @brief Rounds a number to a specific number of decimal places.
     *  @details Rounds a number to a specific number of decimal places.
     *  @param[in] num The number to be rounded.
     *  @param[in] places The number of decimal places:
     *  @exception InvalidCastException If calculation fails.
     *  @retval long double The (rounded) result. 
     */
    static long double getRoundFrom(long double num, unsigned places =
                                    ROUND_TO_DECIMAL_PLACE);

    /*! @fn static bool needsTrim(const char* s)
     *  @brief Returns if a string needs to be trimmed.
     *  @details Returns if a string needs to be trimmed, i.e. returns
     *    <tt>true</tt> if there is a space character at the first or last position in it.
     *  @param[in] s Pointer to a char object (string to be checked).
     *  @retval bool <tt>true</tt> if there is a space character at the first or last position in it,
     *    <tt>false</tt> otherwise. 
     */
    static bool needsTrim(const char *s);

    /*! @fn static const char* trim(const char* s)
     *  @brief Returns a string without all spaces at the beginning and/or the end of it.
     *  @details Returns a string without all spaces at the beginning and/or the end of it.
     *  @param[in] s Pointer to a char object (string to be trimmed).
     *  @retval const char* The trimmed string. 
     */
    static const char *trim(const char *s);
};

#endif // MISCFUNCTIONS_H
