
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
 *  @brief Implementations of header file exception.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file exception.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "exception.h"

Exception::Exception(EXCEPTION_TYPE _type, const char *_error_msg,
                     ERROR_ID _error_id,
                     EXCEPTION_LEVEL _exception_level):type(_type),
error_msg(strdup(_error_msg)),
error_id(_error_id),
exception_level(_exception_level) {
}

Exception::~Exception() {
    free((char *) error_msg);
}

void Exception::terminate(int exit_code) {
    exit(exit_code);
}

const char *Exception::getDebugMsg() {
    std::ostringstream o;
    o << endl << endl;
    o << "!=============================================!" << endl;
    o << "!=================TERMINATION=================!" << endl;
    o << "Process Aborted Due To ";
    switch (type) {
        case exct_argument:
            o << "\"InvalidArgumentException\"";
            break;
        case exct_iostream:
            o << "\"IOStreamException\"";
            break;
        case exct_parse:
            o << "\"ParseException\"";
            break;
        case exct_runtime:
            o << "\"RuntimeException\"";
            break;
        case exct_cast:
            o << "\"InvalidCastException\"";
            break;
        case exct_impl:
            o << "\"NotImplementedException\"";
            break;
    }
    switch (exception_level) {
        case excl_warning:
            o << " [WARNING]";
            break;
        case excl_error:
            o << " [ERROR]";
            break;
        case excl_fatal:
            o << " [FATAL]";
            break;
    }
    o << endl;
    o << "Error Code ";
    switch (error_id) {
        case eid_notset:
            o << eid_notset << ": Not Specified";
            break;
        case eid_argument:
            o << eid_argument << ": Invalid Argument";
            break;
        case eid_stream:
            o << eid_stream << ": I/O Stream";
            break;
        case eid_stream_input:
            o << eid_stream_input << ": Input Stream";
            break;
        case eid_stream_output:
            o << eid_stream_output << ": Output Stream";
            break;
        case eid_parse:
            o << eid_parse << ": Parsing";
            break;
        case eid_parse_query:
            o << eid_parse_query << ": Query Parsing";
            break;
        case eid_parse_xml:
            o << eid_parse_xml << ": XML Parsing";
            break;
        case eid_runtime:
            o << eid_runtime << ": Runtime";
            break;
        case eid_runtime_illegalmode:
            o << eid_runtime_illegalmode << ": Illegal Evaluation Mode";
            break;
        case eid_runtime_bit:
            o << eid_runtime_bit << ": BufferIterator Error";
            break;
        case eid_runtime_tokenconfig:
            o << eid_runtime_tokenconfig << ": TokenConfiguration Error";
            break;
        case eid_runtime_ptlabel:
            o << eid_runtime_ptlabel << ": ProjectionTreeLabel Error";
            break;
        case eid_cast:
            o << eid_cast << ": Cast";
            break;
        case eid_cast_stringnumeric:
            o << eid_cast_numericstring << ": String To Numeric Cast";
            break;
        case eid_cast_numericstring:
            o << eid_cast_stringnumeric << ": Numeric To String Cast";
            break;
        case eid_cast_summation:
            o << eid_cast_summation << ": Summation Calculation";
            break;
        case eid_cast_power:
            o << eid_cast_power << ": Power Calculation";
            break;
        case eid_cast_radical:
            o << eid_cast_radical << ": Radical Calculation";
            break;
        case eid_cast_division:
            o << eid_cast_division << ": Division Calculation";
            break;
        case eid_impl:
            o << eid_impl << ": Not Implemented";
            break;
    }
    o << endl;
    if (error_msg) {
        o << "Debug Message:" << endl << error_msg;
    }
    o << endl;
    o << "!=============================================!" << endl;
    o << "!=============================================!" << endl;

    return o.str().c_str();
}
