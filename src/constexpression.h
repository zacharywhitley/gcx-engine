
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
 *  @brief Header file for constexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for constexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef CONSTEXPRESSION_H
#define CONSTEXPRESSION_H

#include "outputstream.h"
#include "operandexpression.h"

/*! @class ConstExpression
 *  @brief Represents base class of all constant (numeric or string) expressions.
 *  @details Represents base class of all constant (numeric or string) expressions.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class ConstExpression:public OperandExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for an EXP_TYPE.
     *  @param[in] _type EXP_TYPE.
     */
    ConstExpression(EXP_TYPE _type);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ ConstExpression();

    /*! @fn virtual void eval(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc Expression::eval(OutputStream& eos, Environment* env, unsigned modus)
     */
    virtual void eval(OutputStream & eos, Environment * env, unsigned modus) =
        0;

    /*! @fn inline bool isSingleValued()
     *  @copydoc OperandExpression::isSingleValued()
     */
    inline bool isSingleValued() {
        return true;
    }
    /*! @fn inline void prepareOperand(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc OperandExpression::prepareOperand(OutputStream& eos, Environment* env, unsigned modus)
     */ inline void prepareOperand(OutputStream & eos, Environment * env,
                                   unsigned modus) {
        initial = true;
    }

    /*! @fn inline void unprepareOperand()
     *  @copydoc OperandExpression::unprepareOperand()
     */
    inline void unprepareOperand() {
        initial = true;
    }

    /*! @fn virtual Value* getNextValue()
     *  @copydoc OperandExpression::getNextValue()
     */
    virtual Value *getNextValue() = 0;
};

#endif // CONSTEXPRESSION_H
