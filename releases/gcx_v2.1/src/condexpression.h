
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
 *  @brief Header file for condexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for condexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef CONDEXPRESSION_H
#define CONDEXPRESSION_H

#include "expression.h"
#include "outputstream.h"

/*! @class CondExpression
 *  @brief Represents base class of all conditional expressions.
 *  @details Represents base class of all conditional expressions.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class CondExpression:public Expression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for an EXP_TYPE.
     *  @param[in] _type EXP_TYPE.
     */
    CondExpression(EXP_TYPE _type);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ CondExpression();

    /*! @fn virtual bool evalCond(OutputStream& eos, Environment* env, unsigned modus)
     *  @brief Evaluates the corresponding CondExpression.
     *  @details Evaluates the corresponding CondExpression.
     *  @param[in] eos Pointer to a OutputStream object.
     *  @param[in] env Pointer to a Environment object.
     *  @param[in] modus Evaluation modus.
     *  @retval bool <tt>true</tt> if the corresponding CondExpression is <tt>true</tt>,
     *    <tt>false</tt> otherwise.
     *  @exception RuntimeException If illegal evaluation mode (argument: unsigned modus).
     */
    virtual bool evalCond(OutputStream & eos, Environment * env,
                          unsigned modus) = 0;
};

#endif // CONDEXPRESSION_H
