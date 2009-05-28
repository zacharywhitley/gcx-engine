
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
 *  @brief Header file for cmdline.cpp.
 *  @details Header file specifying constructors, destructor and functions for cmdline.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef CMDLINE_H
#define CMDLINE_H

#include <sstream>
#include "typeenums.h"
#include "invalidargumentexception.h"

//! @namespace std
using namespace std;

/*! @struct CmdLine
 *  @brief Represents all application call parameters.
 *  @details Represents all application call parameters, which have been passed to save them.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
struct CmdLine {
    /*! @brief Constructor.
     *  @details Constructor - creating object for all parameters.
     *  @param[in] _query_istream_type ISTREAM_TYPE.
     *  @param[in] _xml_istream_type ISTREAM_TYPE.
     *  @param[in] _eout_ostream_type OSTREAM_TYPE.
     *  @param[in] _dout_ostream_type OSTREAM_TYPE.
     *  @param[in] _query_arg Pointer to a char object (e.g. path to a file).
     *  @param[in] _xml_arg Pointer to a char object (e.g. path to a file).
     *  @param[in] _eout_arg Pointer to a char object (e.g. path to a file).
     *  @param[in] _dout_arg Pointer to a char object (e.g. path to a file).
     *  @param[in] _debug <tt>true</tt> in debug-mode, <tt>false</tt> otherwise.
     *  @param[in] _preprocess_stream_debug <tt>true</tt> in preprocess-stream-mode (including debug-mode), <tt>false</tt> otherwise.
     *  @param[in] _preprocess_stream_no_debug <tt>true</tt> in preprocess-stream-mode (excluding debug-mode), <tt>false</tt> otherwise.
     */
    CmdLine(ISTREAM_TYPE _query_istream_type, ISTREAM_TYPE _xml_istream_type,
            OSTREAM_TYPE _eout_ostream_type, OSTREAM_TYPE _dout_ostream_type,
            const char *_query_arg, const char *_xml_arg,
            const char *_eout_arg, const char *_dout_arg, bool _debug,
            bool _preprocess_stream_debug, bool _preprocess_stream_no_debug);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ CmdLine();

    /*! @fn void checkIntegrity()
     *  @brief Checks if required/sufficient parameters have been passed. 
     *  @details Checks if required/sufficient parameters have been passed.
     *  @exception InvalidArgumentException If wrong or missing parameter is given.
     *  @retval void
     */
    void checkIntegrity();

    /*! @var ISTREAM_TYPE query_istream_type
     *  @brief The query input stream type.
     *  @details The query input stream type, which is the first argument of the constructor.
     */
    ISTREAM_TYPE query_istream_type;

    /*! @var ISTREAM_TYPE xml_istream_type
     *  @brief The XML document input stream type.
     *  @details The XML document input stream type, which is the second argument of the constructor.
     */
    ISTREAM_TYPE xml_istream_type;

    /*! @var OSTREAM_TYPE eout_ostream_type
     *  @brief The evaluate/result output stream type.
     *  @details The evaluate/result output stream type, which is the third argument of the constructor.
     */
    OSTREAM_TYPE eout_ostream_type;

    /*! @var OSTREAM_TYPE dout_ostream_type
     *  @brief The debug output stream type.
     *  @details The debug output stream type, which is the fourth argument of the constructor.
     */
    OSTREAM_TYPE dout_ostream_type;

    /*! @var const char* query_arg
     *  @brief The required argument for query input stream.
     *  @details The required argument for query input stream, which is the fifth argument of the constructor.
     */
    const char *query_arg;

    /*! @var const char* xml_arg
     *  @brief The required argument for XML document input stream.
     *  @details The required argument for XML document input stream, which is the sixth argument of the constructor.
     */
    const char *xml_arg;

    /*! @var const char* eout_arg
     *  @brief The required argument for evaluate/result output stream.
     *  @details The required argument for evaluate/result output stream, which is the seventh argument of the constructor.
     */
    const char *eout_arg;

    /*! @var const char* dout_arg
     *  @brief The required argument for debug output stream.
     *  @details The required argument for debug output stream, which is the eigth argument of the constructor.
     */
    const char *dout_arg;

    /*! @var bool debug
     *  @brief Indicator for debug-mode.
     *  @details Indicator for debug-mode, which is the ninth argument of the constructor.
     */
    bool debug;

    /*! @var bool preprocess_stream_debug
     *  @brief Indicator for preprocess-stream-mode (including debug-mode).
     *  @details Indicator for preprocess-stream-mode (including debug-mode), which is the tenth argument of the constructor.
     */
    bool preprocess_stream_debug;

    /*! @var bool preprocess_stream_no_debug
     *  @brief Indicator for preprocess-stream-mode (excluding debug-mode).
     *  @details Indicator for preprocess-stream-mode (excluding debug-mode), which is the eleventh argument of the constructor.
     */
    bool preprocess_stream_no_debug;
};

#endif // CMDLINE_H
