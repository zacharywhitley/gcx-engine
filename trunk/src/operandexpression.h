
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
 *  @brief Header file for operandexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for operandexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef OPERANDEXPRESSION_H
#define OPERANDEXPRESSION_H

#include "expression.h"
#include "value.h"

/*! @class OperandExpression
 *  @brief Represents base clase of all XQuery operands of a condition.
 *  @details Represents base clase of all XQuery operands of a condition, i.e. represents a operand of a CondOperandExpression,
 *   	which takes either a VarExpression, e.g. <tt>\$x</tt>, <tt>\$z</tt> or <tt>\$root</tt>,
 *   	a VarStepExpression, e.g. <tt>\$x/bib//book</tt> or <tt>//node()/text()</tt>, a AggregateFunctExpression,
 *   	e.g. <tt>fn:count(\$x)</tt> or <tt>fn:sum(/bib//book)</tt>, a RoundingExpression, e.g. <tt>fn:avg(fn:sum(/bib//book))</tt>
 *  		or <tt>fn:round(fn:avg(/bib//book))</tt>, or a ConstExpression, e.g. <tt>"foo"</tt> or <tt>8</tt>, as argument and returns
 *  		the evaluated value of the corresponding Expression.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class OperandExpression:public Expression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for two EXP_TYPE.
     *  @param[in] _type EXP_TYPE.
     *  @param[in] _op_type EXP_TYPE.
     */
    OperandExpression(EXP_TYPE _type, EXP_TYPE _op_type);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ OperandExpression();

    /*! @fn virtual bool isSingleValued()
     *  @brief Returns if the result/iteration of the operand is a single value.
     *  @details Returns if the result/iteration of the operand is a single value, e.g
     *                      a AggregateFunctExpression returns a single value whereas
     *                      a VarStepExpression may return more than one value. 
     *  @retval bool <tt></tt> if the result/iteration is a single value, <tt>false</tt> otherwise.
     */
    virtual bool isSingleValued() = 0;

    /*! @fn virtual void prepareOperand(OutputStream& eos, Environment* env, unsigned modus)
     *  @brief Initializes the operand if necessary.
     *  @details Initializes the operand if necessary.
     *  @param[in] eos Pointer to a OutputStream object.
     *  @param[in] env Pointer to a Environment object.
     *  @param[in] modus Evaluation modus.
     *  @retval void
     */
    virtual void prepareOperand(OutputStream & eos, Environment * env,
                                unsigned modus) = 0;

    /*! @fn virtual void unprepareOperand()
     *  @brief Resets the operand if necessary.
     *  @details Resets the operand if necessary.
     *  @retval void
     */
    virtual void unprepareOperand() = 0;

    /*! @fn virtual Value* getNextValue()
     *  @brief Returns the next value of the operand.
     *  @details Returns the next value of the operand for iteration.
     *  @retval Value* Pointer to a Value object.
     */
    virtual Value *getNextValue() = 0;

    /*! @fn inline const EXP_TYPE getOperandType()
     *  @brief Returns the type of the operand.
     *  @details Returns the type of the operand.
     *  @retval const EXP_TYPE EXP_TYPE.
     */
    inline const EXP_TYPE getOperandType() {
        return op_type;
  } protected:
    /*! @var EXP_TYPE op_type
     *  @brief The entered EXP_TYPE.
     *  @details The entered EXP_TYPE, which is the second argument of the constructor.
     */
    EXP_TYPE op_type;

    /*! @var bool initial
     *  @brief Indicator for method getNextValue() if first call/iteration.
     *  @details Indicator for method getNextValue() if first call/iteration.
     */
    bool initial;

    /*! @var Value cur_val
     *  @brief Container for the result of the operand.
     *  @details Container for the result of the operand.
     */
    Value cur_val;
};

#endif // OPERANDEXPRESSION_H
