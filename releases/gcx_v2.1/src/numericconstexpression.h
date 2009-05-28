
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
 *  @brief Header file for numericconstexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for numericconstexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef NUMERICCONSTEXPRESSION_H
#define NUMERICCONSTEXPRESSION_H

#include "constexpression.h"

/*! @class NumericConstExpression
 *  @brief Represents an XQuery numeric value.
 *  @details Represents an XQuery numeric value, e.g. <tt>3</tt>,
 *    which takes therefore a numeric value as argument and returns this (numeric) value.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class NumericConstExpression:public ConstExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a string (numeric value), which will
     *    be casted to a numeric value.
     *  @param[in] _num Pointer to a char object.
     */
    NumericConstExpression(const char *_num);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ NumericConstExpression();

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
    /*! @var long double num
     *  @brief The entered numeric value.
     *  @details The entered numeric value, which was (after casting) the
     *    argument of the constructor.
     */
    long double num;
};

#endif // NUMERICCONSTEXPRESSION_H
