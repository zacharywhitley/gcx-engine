
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
 *  @brief Header file for falsecondexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for falsecondexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef FALSECONDEXPRESSION_H
#define FALSECONDEXPRESSION_H

#include "condexpression.h"

/*! @class FalseCondExpression
 *  @brief Represents XQuery standard function <tt>fn:false</tt>.
 *  @details Represents XQuery standard function <tt>fn:false</tt>, e.g. <tt>fn:false()</tt>,
 *    which takes no arguments and returns the boolean value <tt>false</tt>.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class FalseCondExpression:public CondExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    FalseCondExpression();

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ FalseCondExpression();

    /*! @fn void print(OutputStream& dos) const
     *  @copydoc Expression::print(OutputStream& dos) const
     */
    void print(OutputStream & dos) const;

    /*! @fn bool evalCond(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc CondExpression::evalCond(OutputStream& eos, Environment* env, unsigned modus)
     */
    bool evalCond(OutputStream & eos, Environment * env, unsigned modus);
};

#endif // FALSECONDEXPRESSION_H
