
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
 *  @brief Implementations of header file streammanager.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file streammanager.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "streammanager.h"

StreamManager *StreamManager::instance = NULL;

void StreamManager::initInstance(InputStream * _query_istream,
                                 InputStream * _xml_istream,
                                 OutputStream * _debug_ostream,
                                 OutputStream * _eval_ostream) {
    instance =
        new StreamManager(_query_istream, _xml_istream, _debug_ostream,
                          _eval_ostream);
}

StreamManager *StreamManager::getInstance() {
    return instance;
}

StreamManager::StreamManager(InputStream * _query_istream, InputStream * _xml_istream, OutputStream * _debug_ostream, OutputStream * _eval_ostream):
query_istream(_query_istream), xml_istream(_xml_istream),
debug_ostream(_debug_ostream), eval_ostream(_eval_ostream) {
}

StreamManager::~StreamManager() {
    delete query_istream;
    delete xml_istream;
    delete debug_ostream;
    delete eval_ostream;
}

void StreamManager::print() {
    (*debug_ostream) << "Query Stream ";
    switch (query_istream->getType()) {
        case it_file:
            (*debug_ostream) << "(Reading From FILE)";
            break;
        case it_null:
            (*debug_ostream) << "(Reading From NULL)";
            break;
        case it_socket:
            (*debug_ostream) << "(Reading From SOCKET)";
            break;
        case it_stdin:
            (*debug_ostream) << "(Reading From STDIN)";
            break;
    }
    if (query_istream->getArg()) {
        (*debug_ostream) << ": \"" << query_istream->getArg() << "\"";
    }
    (*debug_ostream) << NEWLINE;
    (*debug_ostream) << "XML Stream ";
    switch (xml_istream->getType()) {
        case it_file:
            (*debug_ostream) << "(Reading From FILE)";
            break;
        case it_null:
            (*debug_ostream) << "(Reading From NULL)";
            break;
        case it_socket:
            (*debug_ostream) << "(Reading From SOCKET)";
            break;
        case it_stdin:
            (*debug_ostream) << "(Reading From STDIN)";
            break;
    }
    if (xml_istream->getArg()) {
        (*debug_ostream) << ": \"" << xml_istream->getArg() << "\"";
    }
    (*debug_ostream) << NEWLINE;
    (*debug_ostream) << "Debug Stream ";
    switch (debug_ostream->getType()) {
        case ot_file:
            (*debug_ostream) << "(Writing To FILE)";
            break;
        case ot_null:
            (*debug_ostream) << "(Writing To NULL)";
            break;
        case ot_socket:
            (*debug_ostream) << "(Writing To SOCKET)";
            break;
        case ot_stdout:
            (*debug_ostream) << "(Writing To STDIN)";
            break;
    }
    if (debug_ostream->getArg()) {
        (*debug_ostream) << ": \"" << debug_ostream->getArg() << "\"";
    }
    (*debug_ostream) << NEWLINE;
    (*debug_ostream) << "Result Stream ";
    switch (eval_ostream->getType()) {
        case ot_file:
            (*debug_ostream) << "(Writing To FILE)";
            break;
        case ot_null:
            (*debug_ostream) << "(Writing To NULL)";
            break;
        case ot_socket:
            (*debug_ostream) << "(Writing To SOCKET)";
            break;
        case ot_stdout:
            (*debug_ostream) << "(Writing To STDIN)";
            break;
    }
    if (eval_ostream->getArg()) {
        (*debug_ostream) << ": \"" << eval_ostream->getArg() << "\"";
    }
    (*debug_ostream) << NEWLINE;
}
