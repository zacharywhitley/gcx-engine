
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
 *  @brief Header file for stringconstexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for stringconstexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef STRINGCONSTEXPRESSION_H
#define STRINGCONSTEXPRESSION_H

#include <sstream>
#include "constexpression.h"

/*! @class StringConstExpression
 *  @brief Represents an XQuery string value.
 *  @details Represents an XQuery string value, e.g. <tt>"foo"</tt>,
 *    which takes therefore a string as argument and returns this string.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class StringConstExpression:public ConstExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a string.
     *  @param[in] _str Pointer to a char object.
     */
    StringConstExpression(const char *_str);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ StringConstExpression();

    /*! @fn void print(OutputStream& dos) const
     *  @copydoc Expression::print(OutputStream& dos) const
     */
    void print(OutputStream & dos) const;

    /*! @fn void eval(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc ConstExpression::eval(OutputStream& eos, Environment* env, unsigned modus)
     */
    void eval(OutputStream & eos, Environment * env, unsigned modus);

    /*! @fn virtual Value* getNextValue()
     *  @copydoc ConstExpression::getNextValue()
     */
    virtual Value *getNextValue();

  private:
    /*! @var const char* str
     *  @brief The entered string.
     *  @details The entered string, which is the argument of the constructor.
     */
    const char *str;
};

#endif // STRINGCONSTEXPRESSION_H
