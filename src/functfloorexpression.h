
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
 *  @brief Header file for functfloorexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for functfloorexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef FUNCTFLOOREXPRESSION_H
#define FUNCTFLOOREXPRESSION_H

#include "roundingexpression.h"

/*! @def OUTPUT_FLOOR_ON_NON_VALUE
 *  @brief The value which is outputted or returned respectively if the argument of rounding
 *    function <tt>fn:floor</tt> has no result, i.e. the corresponding aggregate function
 *    is called on an empty sequence.
 *  @details The value which is outputted or returned respectively if the argument of rounding
 *    function <tt>fn:floor</tt> has no result, i.e. the corresponding aggregate function
 *    is called on an empty sequence.
 */
#define OUTPUT_FLOOR_ON_NON_VALUE ""

/*! @class FunctFloorExpression
 *  @brief Represents XQuery standard function <tt>fn:floor</tt>.
 *  @details Represents XQuery standard function <tt>fn:floor</tt>, which takes a
 *    AggregateFunctExpression, e.g. <tt>fn:floor(fn:count(\$x/bib/book))</tt> or
 *    <tt>fn:floor(fn:sum(/bib//book))</tt>, as argument and returns the floor result
 *    of the entered AggregateFunctExpression.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class FunctFloorExpression:public RoundingExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a AggregateFunctExpression.
     *  @param[in] _aggfunct Pointer to a AggregateFunctExpression object.
     */
    FunctFloorExpression(AggregateFunctExpression * _aggfunct);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ FunctFloorExpression();

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

#endif // FUNCTFLOOREXPRESSION_H
