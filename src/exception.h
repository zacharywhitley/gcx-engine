
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
 *  @brief Header file for exception.cpp.
 *  @details Header file specifying constructors, destructor and functions for exception.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <cstdlib>
#include <sstream>
#include <string.h>
#include "typeenums.h"

//! @namespace std
using namespace std;

/*! @class Exception
 *  @brief Represents base class of all exceptions.
 *  @details Represents base class of all exceptions.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class Exception {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for an EXCEPTION_TYPE, a string (as error message),
     *                      an ERROR_ID and an EXCEPTION_LEVEL.
     *  @param[in] _type EXCEPTION_TYPE.
     *  @param[in] _error_msg Pointer to a char object.
     *  @param[in] _error_id ERROR_ID.
     *  @param[in] _exception_level EXCEPTION_LEVEL.
     */
    Exception(EXCEPTION_TYPE _type, const char *_error_msg,
              ERROR_ID _error_id =
              eid_notset, EXCEPTION_LEVEL _exception_level = excl_fatal);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ Exception();

    /*! @fn void terminate(int exit_code)
     *  @brief Terminates the application immediately.
     *  @details Terminates the application immediately, returning the entered parameter to the operating system.
     *  @param[in] exit_code Returned parameter to the operating system.
     *  @retval void
     */
    void terminate(int exit_code = EXIT_FAILURE);

    /*! @fn const char* getDebugMsg()
     *  @brief Returns the error message of the corresponding exception.
     *  @details Returns the error message of the corresponding exception.
     *  @retval const char* Pointer to a char object.
     */
    const char *getDebugMsg();

    /*! @fn inline const EXCEPTION_TYPE getType()
     *  @brief Returns the EXCEPTION_TYPE of the corresponding exception.
     *  @details Returns the EXCEPTION_TYPE of the corresponding exception.
     *  @retval EXCEPTION_TYPE.
     */
    inline const EXCEPTION_TYPE getType() {
        return type;
    }
    /*! @fn inline const char* getErrorMsg()
     *  @brief Returns the error message of the corresponding exception.
     *  @details Returns the error message of the corresponding exception.
     *  @retval const char* Pointer to a char object.
     */ inline const char *getErrorMsg() {
        return error_msg;
    }

    /*! @fn inline ERROR_ID getErrorId()
     *  @brief Returns the ERROR_ID of the corresponding exception.
     *  @details Returns the ERROR_ID of the corresponding exception.
     *  @retval ERROR_ID.
     */
    inline ERROR_ID getErrorId() {
        return error_id;
    }

    /*! @fn inline const EXCEPTION_LEVEL getExceptionLevel()
     *  @brief Returns the EXCEPTION_LEVEL of the corresponding exception.
     *  @details Returns the EXCEPTION_LEVEL of the corresponding exception.
     *  @retval const EXCEPTION_LEVEL.
     */
    inline const EXCEPTION_LEVEL getExceptionLevel() {
        return exception_level;
    }

  protected:
    /*! @var EXCEPTION_TYPE type
     *  @brief The entered EXCEPTION_TYPE.
     *  @details The entered EXCEPTION_TYPE, which is the first argument of the constructor.
     */
    EXCEPTION_TYPE type;

    /*! @var const char* error_msg
     *  @brief The entered string (as error message).
     *  @details The entered string (as error message), which is the second argument of the constructor.
     */
    const char *error_msg;

    /*! @var ERROR_ID error_id
     *  @brief The entered ERROR_ID.
     *  @details The entered ERROR_ID, which is the third argument of the constructor.
     */
    ERROR_ID error_id;

    /*! @var EXCEPTION_LEVEL exception_level
     *  @brief The entered EXCEPTION_LEVEL.
     *  @details The entered EXCEPTION_LEVEL, which is the fourth argument of the constructor.
     */
    EXCEPTION_LEVEL exception_level;
};

#endif // EXCEPTION_H
