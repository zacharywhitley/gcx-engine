
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
 *  @brief Header file for commentexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for commentexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef COMMENTEXPRESSION_H
#define COMMENTEXPRESSION_H

#include "expression.h"
#include "outputstream.h"

/*! @class CommentExpression
 *  @brief Represents an XQuery comment expression.
 *  @details Represents an XQuery comment expression, e.g. <tt>(: foo :)</tt>
 *    or <tt>(: (: foo :) (: bar :) :)</tt>, which takes therefore a string as argument.
 *  @note Currently comment expressions are only supported in front of a query and not inside a query!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class CommentExpression:public Expression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a string (comment).
     *  @param[in] _comment Pointer to a char object.
     */
    CommentExpression(const char *_comment);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ CommentExpression();

    /*! @fn virtual void print(OutputStream& dos) const
     *  @copydoc Expression::print(OutputStream& dos) const
     */
    virtual void print(OutputStream & dos) const;

  private:
    /*! @var const char* comment
     *  @brief The entered string (comment).
     *  @details The entered string (comment), which is the argument of the constructor.
     */
    const char *comment;
};

#endif // COMMENTEXPRESSION_H
