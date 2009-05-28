
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
 *  @brief Header file for docexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for docexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef DOCEXPRESSION_H
#define DOCEXPRESSION_H

#include "expression.h"
#include "outputstream.h"

/*! @class DocExpression
 *  @brief Represents XQuery standard function <tt>fn:doc</tt>.
 *  @details Represents XQuery standard function <tt>fn:doc</tt>, which takes a
 *    path to a file, e.g. <tt>fn:doc(\"examples/query.xq\")</tt>, as argument and
 *    returns the file located through the entered path (of the file).
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class DocExpression:public Expression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    DocExpression();

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ DocExpression();

    /*! @fn void print(OutputStream& dos) const
     *  @copydoc Expression::print(OutputStream& dos) const
     */
    void print(OutputStream & dos) const;

    /*! @fn inline const char* getPath()
     *  @brief Returns the argument of <tt>fn:doc</tt>.
     *  @details Returns the argument of <tt>fn:doc</tt>, i.e. returns the path to a file.
     *  @retval const char* Pointer to a char object.
     */
    inline const char *getPath() {
        return path;
    }
    /*! @fn inline void setPath(const char* _path)
     *  @brief Sets the argument of <tt>fn:doc</tt>.
     *  @details Sets the argument of <tt>fn:doc</tt>, i.e. sets the path to a file.
     *  @param[in] _path Pointer to a char object (path to a file).
     *  @retval void
     */ inline void setPath(const char *_path) {
        path = _path;
    }

    /*! @fn inline bool isEmptyPath()
     *  @brief Returns if the argument of <tt>fn:doc</tt> is empty.
     *  @details Returns if the argument of <tt>fn:doc</tt> is empty, i.e. if the input query contains no <tt>fn:doc</tt> expression.
     *  @retval bool <tt>true</tt> if the input query contains one or more <tt>fn:doc</tt> expressions, <tt>false</tt> otherwise. 
     */
    inline bool isEmptyPath() {
        return path == NULL;
    }

    /*! @fn bool isPathEqualTo(const char* _path)
     *  @brief Returns if the argument (path to a file) of this <tt>fn:doc</tt> is equal to the entered argument (path to a file).
     *  @details Returns if the argument (path to a file) of this <tt>fn:doc</tt> is equal to the entered argument (path to a file).
     *  @param[in] _path Pointer to a char object (path to a file), which should be checked for equality.
     *  @retval bool <tt>true</tt> if both are equal, <tt>false</tt> otherwise. 
     */
    bool isPathEqualTo(const char *_path);

  private:
    /*! @var const char* path
     *  @brief The entered argument (path to a file) of <tt>fn:doc</tt>.
     *  @details The entered argument (path to a file) of <tt>fn:doc</tt>, which is the first argument of the constructor.
     *              This member variable is <tt>NULL</tt> on creation of object.
     */
    const char *path;
};

#endif // DOCEXPRESSION_H
