
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
 *  @brief Header file for functroundhalftoevenexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for functroundhalftoevenexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef FUNCTROUNDHALFTOEVENEXPRESSION_H
#define FUNCTROUNDHALFTOEVENEXPRESSION_H

#include "roundingexpression.h"

/*! @def OUTPUT_ROUNDHALFTOEVEN_ON_NON_VALUE
 *  @brief The value which is outputted or returned respectively if the argument of rounding
 *    function <tt>fn:round-half-to-even</tt> has no result, i.e. the corresponding aggregate function
 *    is called on an empty sequence.
 *  @details The value which is outputted or returned respectively if the argument of rounding
 *    function <tt>fn:round-half-to-even</tt> has no result, i.e. the corresponding aggregate function
 *    is called on an empty sequence.
 */
#define OUTPUT_ROUNDHALFTOEVEN_ON_NON_VALUE ""

/*! @class FunctRoundHalfToEvenExpression
 *  @brief Represents XQuery standard function <tt>fn:round-half-to-even</tt>.
 *  @details Represents XQuery standard function <tt>fn:round-half-to-even</tt>, which takes a
 *    AggregateFunctExpression, e.g. <tt>fn:round-half-to-even(fn:count(\$x/bib/book))</tt> or
 *    <tt>fn:round-half-to-even(fn:sum(/bib//book))</tt>, as argument and returns the round half to
 *    even result of the entered AggregateFunctExpression.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class FunctRoundHalfToEvenExpression:public RoundingExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a AggregateFunctExpression.
     *  @param[in] _aggfunct Pointer to a AggregateFunctExpression object.
     */
    FunctRoundHalfToEvenExpression(AggregateFunctExpression * _aggfunct);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ FunctRoundHalfToEvenExpression();

    /*! @fn virtual void eval(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc RoundingExpression::eval(OutputStream& eos, Environment* env, unsigned modus)
     */
    virtual void eval(OutputStream & eos, Environment * env, unsigned modus);

  private:
    /*! @fn virtual long double calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& non_value)
     *  @copydoc RoundingExpression::calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& non_value)
     */
     virtual long double calculateValue(OutputStream & eos, Environment * env,
                                        unsigned modus, bool & non_value);
};

#endif // FUNCTROUNDHALFTOEVENEXPRESSION_H
