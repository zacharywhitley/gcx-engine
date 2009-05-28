
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
 *  @brief Header file for signoffexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for signoffexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef SIGNOFFEXPRESSION_H
#define SIGNOFFEXPRESSION_H

#include "expression.h"
#include "varstepexpression.h"
#include "bufferiterator.h"

/*! @class SignOffExpression
 *  @brief Represents a single (signOff-) expression of the form <tt>signOff(variable/path, role)</tt>.
 *  @details Represents a single (signOff-) expression of the form <tt>signOff(variable/path, role)</tt>, which takes
 *    therefore either a VarExpression, e.g. <tt>signOff(\$x, r<sub>i</sub>)</tt>, or a VarStepExpression, e.g.
 *    <tt>signOff(\$x/bib/book/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>(), r<sub>i</sub>)</tt>
 *    or <tt>signOff(/bib//book[position</tt><tt>()=1], r<sub>i</sub>)</tt> respectively,
 *    and a Role as arguments and removes this certain role from all affected buffer nodes, i.e.
 *    removes role <tt>r<sub>i</sub></tt> from all BufferNode that are located through the entered
 *    VarExpression or the entered VarStepExpression respectively.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class SignOffExpression:public Expression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a VarExpression and a Role.
     *  @param[in] _var Pointer to a VarExpression object.
     *  @param[in] _role Pointer to a Role object.
     */
    SignOffExpression(VarExpression * _var, Role * _role);

    /*! @brief Constructor.
     *  @details Constructor - creating object for a VarStepExpression and a Role.
     *  @param[in] _varstep Pointer to a VarStepExpression object.
     *  @param[in] _role Pointer to a Role object.
     */
    SignOffExpression(VarStepExpression * _varstep, Role * _role);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ SignOffExpression();

    /*! @fn virtual void scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars)
     *  @copydoc Expression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars)
     */
    virtual void scopeCheck(vector < unsigned >&def_vars,
                            vector < unsigned >&introduced_vars,
                            vector < unsigned >&violating_vars);

    /*! @fn virtual void replaceVarId(unsigned old_id, unsigned new_id)
     *  @copydoc Expression::replaceVarId(unsigned old_id, unsigned new_id)
     */
    virtual void replaceVarId(unsigned old_id, unsigned new_id);

    /*! @fn virtual void print(OutputStream& dos) const
     *  @copydoc Expression::print(OutputStream& dos) const
     */
    virtual void print(OutputStream & dos) const;

    /*! @fn void init(BufferNode* root)
     *  @copydoc Expression::init(BufferNode* root)
     */
    void init(BufferNode * root);

    /*! @fn virtual void eval(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc Expression::eval(OutputStream& eos, Environment* env, unsigned modus)
     */
    virtual void eval(OutputStream & eos, Environment * env, unsigned modus);

    /*! @fn virtual unsigned getVar()
     *  @brief Returns the (numeric value of the) variable from the underlying VarExpression or VarStepExpression respectively.
     *  @details Returns the (numeric value of the) variable from the underlying VarExpression or VarStepExpression respectively.
     *  @retval unsigned Numeric value of the variable from the underlying VarExpression or VarStepExpression respectively.
     */
    virtual unsigned getVar();

  private:
    /*! @var VarExpression* var
     *  @brief The entered VarExpression.
     *  @details The entered VarExpression, which is the first argument of the constructor.
     *    This member variable is <tt>NULL</tt> if the constructor was called with a VarStepExpression.
     */
     VarExpression * var;

    /*! @var VarStepExpression* varstep
     *  @brief The entered VarStepExpression.
     *  @details The entered VarStepExpression, which is the first argument of the constructor.
     *    This member variable is <tt>NULL</tt> if the constructor was called with a VarExpression.
     */
    VarStepExpression *varstep;

    /*! @var VarExpression* exec_var
     *  @brief The cloned VarExpression of the original VarExpression.
     *  @details The cloned VarExpression of the original VarExpression.
     *    This member variable is <tt>NULL</tt> if the constructor was called with a VarStepExpression.
     *  @note Because of strict separation of VarExpression and VarStepExpression it is possible that a cloned
     *    VarStepExpression results in a VarExpression. This, for example, will be the case for
     *    <tt>signOff(\$x/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>(), r<sub>i</sub>)</tt>.    
     */
    VarExpression *exec_var;

    /*! @var VarStepExpression* exec_varstep
     *  @brief The cloned VarStepExpression of the original VarStepExpression.
     *  @details The cloned VarStepExpression of the original VarStepExpression without the tail PathStepExpression
     *    <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt> if present.
     *    This member variable is <tt>NULL</tt> if the constructor was called with a VarExpression.
     *  @note Because of strict separation of VarExpression and VarStepExpression it is possible that a cloned
     *    VarStepExpression results in a VarExpression. This, for example, will be the case for
     *    <tt>signOff(\$x/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>(), r<sub>i</sub>)</tt>. 
     */
    VarStepExpression *exec_varstep;

    /*! @var Role* role
     *  @brief The entered Role.
     *  @details The entered Role, which is the second argument of the constructor and defines
     *    the Role which should be be removed from all through VarExpression or VarStepExpression
     *    respectively located BufferNode.
     */
    Role *role;

    /*! @var BufferIterator* bit
     *  @brief The initialized BufferIterator for the entered/cloned VarExpression or VarStepExpression.
     *  @details The initialized BufferIterator for the entered/cloned VarExpression or VarStepExpression
     *    after calling method init(). This member variable is <tt>NULL</tt> on creation of object
     *    but should not be <tt>NULL</tt> anymore after method init() has been executed.
     */
    BufferIterator *bit;
};

#endif // SIGNOFFEXPRESSION_H
