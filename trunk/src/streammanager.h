
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
 *  @brief Header file for streammanager.cpp.
 *  @details Header file specifying constructors, destructor and functions for streammanager.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#include "inputstream.h"
#include "outputstream.h"

//! @namespace std
using namespace std;

/*! @class StreamManager
 *  @brief Represents stream managing class.
 *  @details Represents stream managing class, which handles all input/ouput streams.
 *  @note SINGLETON PATTERN!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class StreamManager {

  public:
    /*! @fn static void initInstance(InputStream* _query_istream, InputStream* _xml_istream, OutputStream* _debug_ostream, OutputStream* _eval_ostream)
     *  @brief Initializes instance of StreamManager.
     *  @details Initializes instance of StreamManager, i.e. initializes this object.
     *  @note CALL THIS METHOD BEFORE METHOD getInstance()!
     *  @param[in] _query_istream Pointer to a InputStream object.
     *  @param[in] _xml_istream Pointer to a InputStream object.
     *  @param[in] _debug_ostream Pointer to a OutputStream object.
     *  @param[in] _eval_ostream Pointer to a OutputStream object.
     *  @retval void
     */
    static void initInstance(InputStream * _query_istream,
                             InputStream * _xml_istream,
                             OutputStream * _debug_ostream,
                             OutputStream * _eval_ostream);

    /*! @fn static StreamManager* getInstance()
     *  @brief Returns instance of StreamManager.
     *  @details Returns instance of StreamManager, i.e. of this object.
     *  @note DO NOT CALL THIS METHOD BEFORE METHOD initInstance(InputStream* _query_istream, InputStream* _xml_istream, OutputStream* _debug_ostream, OutputStream* _eval_ostream) WAS CALLED!
     *  @retval StreamManager* Pointer to a StreamManager object.
     */
    static StreamManager *getInstance();

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ StreamManager();

    /*! @fn void print()
     *  @brief Prints information of all used input/output streams.
     *  @details Prints information of all used input/output streams.
     *  @retval void
     */
    void print();

    /*! @fn inline InputStream* getQueryInputStream()
     *  @brief Returns the query input stream.
     *  @details Returns the query input stream.
     *  @retval InputStream* Pointer to a InputStream object.
     */
    inline InputStream *getQueryInputStream() {
        return query_istream;
    }
    /*! @fn inline InputStream* getXMLInputStream()
     *  @brief Returns the XML document input stream.
     *  @details Returns the XML document input stream.
     *  @retval InputStream* Pointer to a InputStream object.
     */ inline InputStream *getXMLInputStream() {
        return xml_istream;
    }

    /*! @fn inline OutputStream* getDebugOutputStream()
     *  @brief Returns the debug output stream.
     *  @details Returns the debug output stream.
     *  @retval OutputStream* Pointer to a OutputStream object.
     */
    inline OutputStream *getDebugOutputStream() {
        return debug_ostream;
    }

    /*! @fn inline OutputStream* getEvalOutputStream()
     *  @brief Returns the evaluate/result output stream.
     *  @details Returns the evaluate/result output stream.
     *  @retval OutputStream* Pointer to a OutputStream object.
     */
    inline OutputStream *getEvalOutputStream() {
        return eval_ostream;
    }

  private:
    /*! @var static StreamManager* instance
     *  @brief Instance of StreamManager.
     *  @details Instance of StreamManager to avoid duplicate existence of this object.
     */
    static StreamManager *instance;

    /*! @brief Constructor.
     *  @details Constructor - creating object for two InputStream and two OutputStream.
     *  @param[in] _query_istream Pointer to a InputStream object.
     *  @param[in] _xml_istream Pointer to a InputStream object.
     *  @param[in] _debug_ostream Pointer to a OutputStream object.
     *  @param[in] _eval_ostream Pointer to a OutputStream object.
     */
    StreamManager(InputStream * _query_istream, InputStream * _xml_istream,
                  OutputStream * _debug_ostream, OutputStream * _eval_ostream);

    /*! @var InputStream* query_istream
     *  @brief The entered query InputStream.
     *  @details The entered query InputStream, which is the first argument of the constructor.
     */
    InputStream *query_istream;

    /*! @var InputStream* xml_istream
     *  @brief The entered XML document InputStream.
     *  @details The entered XML document InputStream, which is the second argument of the constructor.
     */
    InputStream *xml_istream;

    /*! @var OutputStream* debug_ostream
     *  @brief The entered debug OutputStream.
     *  @details The entered debug OutputStream, which is the third argument of the constructor.
     */
    OutputStream *debug_ostream;

    /*! @var OutputStream* eval_ostream
     *  @brief The entered evaluate/result OutputStream.
     *  @details The entered evaluate/result OutputStream, which is the fourth argument of the constructor.
     */
    OutputStream *eval_ostream;
};

#endif // STREAMMANAGER_H
