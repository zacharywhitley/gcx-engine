
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
 *  @brief Header file for aggregatefunctavgexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for aggregatefunctavgexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef AGGREGATEFUNCTAVGEXPRESSION_H
#define AGGREGATEFUNCTAVGEXPRESSION_H

#include "aggregatefunctexpression.h"

/*! @def OUTPUT_AVG_ON_EMPTY_SEQUENCE
 *  @brief The value which is outputted or returned respectively if the argument of aggregate
 *    function <tt>fn:avg</tt> contains no items, i.e. is called on an empty sequence.
 *  @details The value which is outputted or returned respectively if the argument of aggregate
 *    function <tt>fn:avg</tt> contains no items, i.e. is called on an empty sequence.
 */
#define OUTPUT_AVG_ON_EMPTY_SEQUENCE ""

/*! @class AggregateFunctAvgExpression
 *  @brief Represents XQuery standard function <tt>fn:avg</tt>.
 *  @details Represents XQuery standard function <tt>fn:avg</tt>, which takes either a
 *    VarExpression, e.g. <tt>fn:avg(\$x)</tt>, or a VarStepExpression, e.g.
 *    <tt>fn:avg(\$x/bib/book)</tt> or <tt>fn:avg(/bib//book)</tt> respectively,
 *    as argument and returns the average value of the sequence located through
 *    the entered VarExpression or the entered VarStepExpression respectively.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class AggregateFunctAvgExpression:public AggregateFunctExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a VarExpression.
     *  @param[in] _var Pointer to a VarExpression object.
     */
    AggregateFunctAvgExpression(VarExpression * _var);

    /*! @brief Constructor.
     *  @details Constructor - creating object for a VarStepExpression.
     *  @param[in] _varstep Pointer to a VarStepExpression object.
     */
    AggregateFunctAvgExpression(VarStepExpression * _varstep);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ AggregateFunctAvgExpression();

    /*! @fn virtual void eval(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc AggregateFunctExpression::eval(OutputStream& eos, Environment* env, unsigned modus)
     *  @exception InvalidCastException If it is not possible to cast all affected PCDATA values of
     *    an XML document to numeric values or some value calculation will cause an (internal) error.
     */
    virtual void eval(OutputStream & eos, Environment * env, unsigned modus);

  private:
    /*! @fn virtual long double calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence)
     *  @copydoc AggregateFunctExpression::calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence)
     */
     virtual long double calculateValue(OutputStream & eos, Environment * env,
                                        unsigned modus, bool & empty_sequence);
};

#endif // AGGREGATEFUNCTAVGEXPRESSION_H
