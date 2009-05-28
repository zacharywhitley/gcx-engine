
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
 *  @brief Header file for outputstream.cpp.
 *  @details Header file specifying constructors, destructor and functions for outputstream.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

#include <sstream>
#include <iostream>
#include "typeenums.h"
#include "expression.h"
#include "role.h"
#include "pathstepattribute.h"
#include "pathstepexpression.h"
#include "iostreamexception.h"

#if defined(LINUX) || defined(linux) || defined(_LINUX) || defined(_linux) || defined(__LINUX__) || defined(__linux__)

/*! @def NEWLINE
 *  @brief Operating system specific newline character.
 *  @details Operating system specific newline character.
 */
#define NEWLINE	"\n"
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)

/*! @def NEWLINE
 *  @brief Operating system specific newline character.
 *  @details Operating system specific newline character.
 */
#define NEWLINE	"\n"
#else

/*! @def NEWLINE
 *  @brief Operating system specific newline character.
 *  @details Operating system specific newline character.
 */
#define NEWLINE	"\n"
#endif

/*! @def INDENT_TOKEN
 *  @brief Indent character.
 *  @details Indent character.
 */
#define INDENT_TOKEN	" "

/*! @def INDENT_MULTIPLICATOR
 *  @brief Multiplicator specifiying number of indent characters.
 *  @details Multiplicator specifiying number of indent characters.
 */
#define INDENT_MULTIPLICATOR	2

/*! @def INDENT_SINGLE
 *  @brief Single indent character multiplicator specifiying one indent character to output.
 *  @details Single indent character multiplicator specifiying one indent character to output.
 */
#define INDENT_SINGLE	1

/*! @def INDENT_DOUBLE
 *  @brief Double indent character multiplicator specifiying two indent character to output.
 *  @details Double indent character multiplicator specifiying two indent character to output.
 */
#define INDENT_DOUBLE	2

//! @namespace std
using namespace std;

/*! @class OutputStream
 *  @brief Represents base class of all output streams.
 *  @details Represents base class of all output streams.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class OutputStream {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for an OSTREAM_TYPE and a string (e.g. path to a file).
     *  @param[in] _type OSTREAM_TYPE.
     *  @param[in] _arg Pointer to a char object.
     */
    OutputStream(OSTREAM_TYPE _type, const char *_arg);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ OutputStream();

    /*! @fn virtual void open()
     *  @brief Opens the corresponding output stream.
     *  @details Opens the corresponding output stream.
     *  @retval void
     *  @exception IOStreamException If an error occured (e.g. if it is not possible to open a file)
     */
    virtual void open() = 0;

    /*! @fn virtual void close()
     *  @brief Closes the corresponding output stream.
     *  @details Closes the corresponding output stream.
     *  @retval void
     */
    virtual void close() = 0;

    /*! @fn inline void decrementIndents(int n)
     *  @brief Decrements the number of indents to be outputted.
     *  @details Decrements the number of indents to be outputted.
     *  @param[in] n Number of indents to be decremented.
     *  @retval void
     */
    inline void decrementIndents(int n = INDENT_SINGLE) {
        indents = ((indents - n) > 0) ? (indents - n) : 0;
    }
    /*! @fn inline void incrementIndents(int n)
     *  @brief Increments the number of indents to be outputted.
     *  @details Increments the number of indents to be outputted.
     *  @param[in] n Number of indents to be incremented.
     *  @retval void
     */ inline void incrementIndents(int n = INDENT_SINGLE) {
        indents = indents + n;
    }

    /*! @fn inline void resetIndents()
     *  @brief Resets the number of indents to be outputted.
     *  @details Resets the number of indents to be outputted, i.e. sets <tt>indents = 0</tt>.
     *  @retval void
     */
    inline void resetIndents() {
        indents = 0;
    }

    /*! @fn virtual void writeIndents()
     *  @brief Writes all indent characters to the corresponding output stream.
     *  @details Writes all indent characters to the corresponding output stream, i.e.
     *              writes <tt>INDENT_TOKEN</tt> <tt>INDENT_MULTIPLICATOR * indents</tt> times.
     *  @retval void
     */
    virtual void writeIndents();

    /*! @fn virtual void write(const char* str)
     *  @brief Writes a string to the corresponding output stream.
     *  @details Writes a string to the corresponding output stream.
     *  @param[in] str Pointer to a char object (string to be written).
     *  @retval void
     */
    virtual void write(const char *str) = 0;

    /*! @fn virtual void writeln(const char* str)
     *  @brief Writes a string with an ending newline character to the corresponding output stream.
     *  @details Writes a string with an ending newline character to the corresponding output stream.
     *  @param[in] str Pointer to a char object (string to be written).
     *  @retval void
     */
    virtual void writeln(const char *str) = 0;

    /*! @fn virtual bool isWriteable()
     *  @brief Returns/Verifies if the corresponding output stream is writeable.
     *  @details Returns/Verifies if the corresponding output stream is writeable.
     *  @retval bool <tt>true</tt> if the corresponding output stream is writeable,
     *    <tt>false</tt> otherwise.
     */
    virtual bool isWriteable() = 0;

    /*! @fn virtual void flush ()
     *  @brief Flushes the corresponding output stream.
     *  @details Flushes the corresponding output stream.
     *  @retval void
     */
    virtual void flush() = 0;

    /*! @fn inline const OSTREAM_TYPE getType()
     *  @brief Returns the OSTREAM_TYPE of the corresponding output stream.
     *  @details Returns the OSTREAM_TYPE of the corresponding output stream.
     *  @retval OSTREAM_TYPE.
     */
    inline const OSTREAM_TYPE getType() {
        return type;
    }

    /*! @fn inline const char* getArg()
     *  @brief Returns the entered argument (e.g. path to a file) of the corresponding output stream.
     *  @details Returns the entered argument (e.g. path to a file) of the corresponding output stream.
     *  @retval const char* Pointer to a char object.
     */
    inline const char *getArg() {
        return arg;
    }

  private:
    /*! @var int indents
     *  @brief The number of indents to be outputted.
     *  @details The number of indents to be outputted.
     */
    int indents;

  protected:
    /*! @var OSTREAM_TYPE type
     *  @brief The entered OSTREAM_TYPE.
     *  @details The entered OSTREAM_TYPE, which is the first argument of the constructor.
     */
    OSTREAM_TYPE type;

    /*! @var const char* arg
     *  @brief The entered string (e.g. path to a file).
     *  @details The entered string (e.g. path to a file), which is the second argument of the constructor.
     */
    const char *arg;
};

/*! @fn inline OutputStream& operator<<(OutputStream& out, const char* str)
 *  @brief Operator <tt>\<\<</tt> overloading for strings.
 *  @details Operator <tt>\<\<</tt> overloading for strings.
 *  @param[in] out Pointer to a OutputStream object.
 *  @param[in] str Pointer to a char object (string to be written).
 *  @retval OutputStream Pointer to a OutputStream object.
 */
inline OutputStream & operator<<(OutputStream & out, const char *str) {
    out.write(str);
    return out;
}

/*! @fn inline OutputStream& operator<<(OutputStream& out, const long double i)
 *  @brief Operator <tt>\<\<</tt> overloading for numbers.
 *  @details Operator <tt>\<\<</tt> overloading for numbers.
 *  @param[in] out Pointer to a OutputStream object.
 *  @param[in] i Number to be written.
 *  @retval OutputStream Pointer to a OutputStream object.
 */
inline OutputStream & operator<<(OutputStream & out, const long double i) {
    std::ostringstream o;
    o << i;
    out.write(o.str().c_str());
    return out;
}

/*! @fn inline OutputStream& operator<<(OutputStream& out, const Expression& exp)
 *  @brief Operator <tt>\<\<</tt> overloading for Expression.
 *  @details Operator <tt>\<\<</tt> overloading for Expression.
 *  @param[in] out Pointer to a OutputStream object.
 *  @param[in] exp Expression to be written.
 *  @retval OutputStream Pointer to a OutputStream object.
 */
inline OutputStream & operator<<(OutputStream & out, const Expression & exp) {
    exp.print(out);
    return out;
}

/*! @fn inline OutputStream& operator<<(OutputStream& out, const PathStepAttribute& pstepattr)
 *  @brief Operator <tt>\<\<</tt> overloading for PathStepAttribute.
 *  @details Operator <tt>\<\<</tt> overloading for PathStepAttribute.
 *  @param[in] out Pointer to a OutputStream object.
 *  @param[in] pstepattr PathStepAttribute to be written.
 *  @retval OutputStream Pointer to a OutputStream object.
 */
inline OutputStream & operator<<(OutputStream & out,
                                 const PathStepAttribute & pstepattr) {
    pstepattr.print(out);
    return out;
}

/*! @fn inline OutputStream& operator<<(OutputStream& out, const Role& role)
 *  @brief Operator <tt>\<\<</tt> overloading for Role.
 *  @details Operator <tt>\<\<</tt> overloading for Role.
 *  @param[in] out Pointer to a OutputStream object.
 *  @param[in] role Role to be written.
 *  @retval OutputStream Pointer to a OutputStream object.
 */
inline OutputStream & operator<<(OutputStream & out, const Role & role) {
    role.print(out);
    return out;
}

#endif // OUTPUTSTREAM_H
