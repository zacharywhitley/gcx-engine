
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
 *  @brief Implementations of header file fileoutputstream.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file fileoutputstream.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "fileoutputstream.h"

FileOutputStream::FileOutputStream(const char *_path):OutputStream(ot_file,
                                                                   _path),
file(NULL) {
}

FileOutputStream::~FileOutputStream() {
    close();
}

void FileOutputStream::open() {
    file = fopen(arg, "w");
    if (!file) {
        std::ostringstream o;
        o << "Unable To Open File \"" << arg << "\"";
        throw IOStreamException(o.str().c_str(), eid_stream_output);
    }
}

void FileOutputStream::close() {
    fclose(file);
}

void FileOutputStream::write(const char *str) {
    fputs(str, file);
}

void FileOutputStream::writeln(const char *str) {
    fputs(str, file);
    fputs(NEWLINE, file);
}

bool FileOutputStream::isWriteable() {
    return file && !ferror(file);
}

void FileOutputStream::flush() {
    fflush(file);
}
