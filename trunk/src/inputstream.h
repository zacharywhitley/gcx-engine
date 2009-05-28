
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
 *  @brief Header file for inputstream.cpp.
 *  @details Header file specifying constructors, destructor and functions for inputstream.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include <sstream>
#include "typeenums.h"
#include "iostreamexception.h"

//! @namespace std
using namespace std;

/*! @class InputStream
 *  @brief Represents base class of all input streams.
 *  @details Represents base class of all input streams.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class InputStream {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for an ISTREAM_TYPE and a string (e.g. path to a file).
     *  @param[in] _type ISTREAM_TYPE.
     *  @param[in] _arg Pointer to a char object.
     */
    InputStream(ISTREAM_TYPE _type, const char *_arg);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ InputStream();

    /*! @fn virtual void open()
     *  @brief Opens the corresponding input stream.
     *  @details Opens the corresponding input stream.
     *  @retval void
     *  @exception IOStreamException If an error occured (e.g. if it is not possible to open a file)
     */
    virtual void open() = 0;

    /*! @fn virtual void close()
     *  @brief Closes the corresponding input stream.
     *  @details Closes the corresponding input stream.
     *  @retval void
     */
    virtual void close() = 0;

    /*! @fn virtual FILE* getParserStream()
     *  @brief Returns the (file-)input stream needed for YACC parser.
     *  @details Returns the (file-)input stream needed for YACC parser.
     *  @retval FILE* Pointer to a file object.
     */
    virtual FILE *getParserStream() = 0;

    /*! @fn virtual bool isReadable()
     *  @brief Returns/Verifies if the corresponding input stream is readable.
     *  @details Returns/Verifies if the corresponding input stream is readable.
     *  @retval bool <tt>true</tt> if the corresponding input stream is readable,
     *    <tt>false</tt> otherwise.
     */
    virtual bool isReadable() = 0;

    /*! @fn inline const ISTREAM_TYPE getType()
     *  @brief Returns the ISTREAM_TYPE of the corresponding input stream.
     *  @details Returns the ISTREAM_TYPE of the corresponding input stream.
     *  @retval ISTREAM_TYPE.
     */
    inline const ISTREAM_TYPE getType() {
        return type;
    }
    /*! @fn inline const char* getArg()
     *  @brief Returns the entered argument (e.g. path to a file) of the corresponding input stream.
     *  @details Returns the entered argument (e.g. path to a file) of the corresponding input stream.
     *  @retval const char* Pointer to a char object.
     */ inline const char *getArg() {
        return arg;
    }

    /*! @fn inline void setArg(const char* _arg)
     *  @brief Sets the argument (e.g. path to a file) of the corresponding input stream.
     *  @details Sets the argument (e.g. path to a file) of the corresponding input stream.
     *  @param[in] _arg Pointer to a char object.
     *  @retval void
     */
    inline void setArg(const char *_arg) {
        arg = _arg;
    }

  protected:
    /*! @var ISTREAM_TYPE type
     *  @brief The entered ISTREAM_TYPE.
     *  @details The entered ISTREAM_TYPE, which is the first argument of the constructor.
     */
    ISTREAM_TYPE type;

    /*! @var const char* arg
     *  @brief The entered string (e.g. path to a file).
     *  @details The entered string (e.g. path to a file), which is the second argument of the constructor.
     */
    const char *arg;
};

#endif // INPUTSTREAM_H
