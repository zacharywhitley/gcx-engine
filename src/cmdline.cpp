
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
 *  @brief Implementations of header file cmdline.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file cmdline.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "cmdline.h"

CmdLine::CmdLine(ISTREAM_TYPE _query_istream_type,
                 ISTREAM_TYPE _xml_istream_type,
                 OSTREAM_TYPE _eout_ostream_type,
                 OSTREAM_TYPE _dout_ostream_type, const char *_query_arg,
                 const char *_xml_arg, const char *_eout_arg,
                 const char *_dout_arg, bool _debug,
                 bool _preprocess_stream_debug,
                 bool
                 _preprocess_stream_no_debug):query_istream_type
    (_query_istream_type),
xml_istream_type(_xml_istream_type),
eout_ostream_type(_eout_ostream_type),
dout_ostream_type(_dout_ostream_type),
query_arg(_query_arg),
xml_arg(_xml_arg),
eout_arg(_eout_arg),
dout_arg(_dout_arg),
debug(_debug),
preprocess_stream_debug(_preprocess_stream_debug),
preprocess_stream_no_debug(_preprocess_stream_no_debug) {
}

CmdLine::~CmdLine() {
    //delete[] query_arg;
    //delete[] xml_arg;
    //delete[] eout_arg;
    //delete[] dout_arg;
}


void CmdLine::checkIntegrity() {
    bool error = false;
    bool require_arg = true;
    bool require_endl = false;

    std::ostringstream o;
    switch (query_istream_type) {
        case it_file:
            break;
        case it_null:
            require_arg = false;
            break;
        case it_socket:
            break;
        case it_stdin:
            require_arg = false;
            break;
    }
    if (require_arg && !query_arg) {
        error = true;
        if (require_endl) {
            o << endl;
        }
        o << "Query Input Stream Requires Argument And Parameter \"--query <query stream>\"";
        require_endl = true;
    }
    require_arg = true;
    switch (eout_ostream_type) {
        case ot_file:
            break;
        case ot_null:
            require_arg = false;
            break;
        case ot_socket:
            break;
        case ot_stdout:
            require_arg = false;
            break;
    }
    if (require_arg && !eout_arg) {
        error = true;
        if (require_endl) {
            o << endl;
        }
        o << "Result Output Stream Requires Argument And Parameter \"--eout <eval stream>\"";
        require_endl = true;
    }
    require_arg = true;
    switch (dout_ostream_type) {
        case ot_file:
            break;
        case ot_null:
            require_arg = false;
            break;
        case ot_socket:
            break;
        case ot_stdout:
            require_arg = false;
            break;
    }
    if (require_arg && !dout_arg) {
        error = true;
        if (require_endl) {
            o << endl;
        }
        o << "Debug Output Stream Requires Argument And Parameter \"--dout <debug stream>\"";
        require_endl = true;
    }
    if (xml_istream_type == it_stdin && query_istream_type == it_stdin) {
        error = true;
        if (require_endl) {
            o << endl;
        }
        o << "Cannot Read Both XML File and Query From STDIN In Parallel";
        require_endl = true;
    }
    if (query_istream_type == it_null) {
        error = true;
        if (require_endl) {
            o << endl;
        }
        o << "Invalid Query Input Stream (Query Cannot Be Read From Null Stream)";
        require_endl = true;
    }
    if (error) {
        throw InvalidArgumentException(o.str().c_str());
    }
}
