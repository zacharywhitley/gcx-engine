
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
 *  @brief Header file for iostreamexception.cpp.
 *  @details Header file specifying constructors, destructor and functions for iostreamexception.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef IOSTREAMEXCEPTION_H
#define IOSTREAMEXCEPTION_H

#include "exception.h"

/*! @class IOStreamException
 *  @brief Represents I(input)/O(utput) stream exception.
 *  @details Represents I(input)/O(utput) stream exception.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class IOStreamException:public Exception {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for an string (exception message),
     *    an ERROR_ID and an EXCEPTION_LEVEL.
     *  @param[in] _error_msg Pointer to a char object.
     *  @param[in] _error_id ERROR_ID:
     *  @param[in] _exception_level EXCEPTION_LEVEL.
     */
    IOStreamException(const char *_error_msg, ERROR_ID _error_id = eid_stream,
                      EXCEPTION_LEVEL _exception_level = excl_fatal);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ IOStreamException();
};

#endif // IOSTREAMEXCEPTION_H
