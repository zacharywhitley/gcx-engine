
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
 *  @brief Header file for value.h 
 *  @details Header file specifying constructors, destructor and functions for value.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef VALUE_H
#define VALUE_H

#include "typeenums.h"
#include "miscfunctions.h"

/*! @struct Value
 *  @brief An XML value, used to represent typed XML PCDATA elements.
 *  @details GCX currently supports to basic types, namely xsd::string
 *    and xsd::numeric. This struct provides fields for both string
 *    and numeric content and automatically casts values if
 *    requested.
 *  @note The GCX type system is not complete, but implements a minimalistic
 *    but useful part of the XML/XQuery type system.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
struct Value {

    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    Value();

    /*! @brief Constructor.
     *  @details Constructor - creating object for a value of type xsd::string.
     *  @param[in] _str_val Pointer to a char object.
     */
    Value(const char *_str_val);

    /*! @brief Constructor.
     *  @details Constructor - creating object for a value of type xsd::numeric.
     *  @param[in] _num_val Numeric value.
     */
     Value(long double _num_val);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ Value();

    /*! @fn inline const char* getStrVal()
     *  @brief Returns the string representation of this value.
     *  @details Straightforwardly returns the corresponding field of the struct.
     *  @retval const char* Pointer to the string representation of this value.
     */
    inline const char *getStrVal() {
        return str_val;
    }
    /*! @fn inline long double getNumVal()
     *  @brief Returns the numeric representation of this value.
     *  @details If the field xsd_numeric is not initialized, a cast will be
     *    triggered, trying to convert xsd_string into a numeric value.
     *  @retval long double The numeric representations of this value, if any.
     *  @exception Throws an error (InvalidCastException) if the string cannot be converted.
     */ inline long double getNumVal() {
        switch (type) {
            case xsd_numeric:
                return num_val;
            case xsd_string:
                return MiscFunctions::getNumericFromString(str_val);
            default:
                return 0;       // should never happen
        }
    }

    /*! @fn inline VALUE_TYPE getType()
     *  @brief Returns the type of the Value object.
     *  @details Due to the minimalistic type system, this will either be either
     *    xsd_string, xsd_numeric.
     *  @retval VALUE_TYPE The type of the Value object.
     */
    inline VALUE_TYPE getType() {
        return type;
    }

    /*! @fn void setStrVal(const char* _str_val)
     *  @brief Initializes the str_val member variable.
     *  @details Initializes the str_val member variable, by setting its pointer
     *    to the passes const char*.
     *  @param[in] _str_val The string the pointer should be set to.
     *  @retval void
     */
    void setStrVal(const char *_str_val);

    /*! @fn void setNumVal(long double _num_val)
     *  @brief Initializes the num_val member variable.
     *  @details Initializes the num_val member variable to the passed
     *    numeric value.
     * @param[in] _num_val The numeric value to be set.
     *  @retval void
     */
    void setNumVal(long double _num_val);

    /*! @fn void reset()
     *  @brief Resets the data structure.
     *  @details Resets the data structure, by emptying its member variables.
     *  @retval void
     */
    void reset();

    /*! @var const char* str_val
     *  @brief Field to store the string value.
     *  @details This field may be empty, i.e. point to NULL.
     */
    const char *str_val;

    /*! @var long double num_val
     *  @brief Field to store the numeric value.
     *  @details Field to store the numeric value, if any.
     */
    long double num_val;

    /*! @var VALUE_TYPE type
     *  @brief Specifies the type of instantiated objects.
     *  @details Set either to xsd_numeric or xsd_string.
     */
    VALUE_TYPE type;
};

#endif // VALUE_H
