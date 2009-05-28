
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
 *  @brief Header file for fileoutputstream.cpp.
 *  @details Header file specifying constructors, destructor and functions for fileoutputstream.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef FILEOUTPUTSTREAM_H
#define FILEOUTPUTSTREAM_H

#include "outputstream.h"

/*! @class FileOutputStream
 *  @brief Represents a file output stream.
 *  @details Represents a file output stream.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class FileOutputStream:public OutputStream {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a string (path to a file).
     *  @param[in] _path Pointer to a char object.
     */
    FileOutputStream(const char *_path);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ FileOutputStream();

    /*! @fn void open()
     *  @copydoc OutputStream::open()
     */
    void open();

    /*! @fn void close()
     *  @copydoc OutputStream::close()
     */
    void close();

    /*! @fn void write(const char* str)
     *  @copydoc OutputStream::write(const char* str)
     */
    void write(const char *str);

    /*! @fn void writeln(const char* str)
     *  @copydoc OutputStream::writeln(const char* str)
     */
    void writeln(const char *str);

    /*! @fn bool isWriteable()
     *  @copydoc OutputStream::isWriteable()
     */
    bool isWriteable();

    /*! @fn void flush()
     *  @copydoc OutputStream::flush()
     */
    void flush();

  private:
    /*! @var FILE* file
     *  @brief The opened file for writing.
     *  @details The opened file for writing, which was found for the path given as argument
     *    of the constructor. This member variable is <tt>NULL</tt> on creation of object
     *    but should not be <tt>NULL</tt> anymore after method open() has been executed.
     */
     FILE * file;
};

#endif // FILEOUTPUTSTREAM_H
