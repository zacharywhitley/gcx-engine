
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
 *  @brief Header file for stdininputstream.cpp.
 *  @details Header file specifying constructors, destructor and functions for stdininputstream.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef STDININPUTSTREAM_H
#define STDININPUTSTREAM_H

#include "inputstream.h"

/*! @class StdinInputStream
 *  @brief Represents a standard (console) input stream.
 *  @details Represents a standard (console) input stream.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class StdinInputStream:public InputStream {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    StdinInputStream();

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ StdinInputStream();

    /*! @fn void open()
     *  @copydoc InputStream::open()
     */
    void open();

    /*! @fn void close()
     *  @copydoc InputStream::close()
     */
    void close();

    /*! @fn FILE* getParserStream()
     *  @copydoc InputStream::getParserStream()
     */
    FILE *getParserStream();

    /*! @fn bool isReadable()
     *  @copydoc InputStream::isReadable()
     */
    bool isReadable();
};

#endif // STDININPUTSTREAM_H
