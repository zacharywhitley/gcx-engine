
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
 *  @brief Header file for varstepcondexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for varstepcondexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef VARSTEPCONDEXPRESSION_H
#define VARSTEPCONDEXPRESSION_H

#include "condexpression.h"
#include "varstepexpression.h"
#include "existscondexpression.h"

/*! @class VarstepCondExpression
 *  @brief Represents XQuery condition <tt>if(\$x)</tt> or <tt>if(\$x/bib/book)</tt>.
 *  @details Represents XQuery condition <tt>if(\$x)</tt> or <tt>if(\$x/bib/book)</tt>, which
 *    takes either a VarExpression, e.g. <tt>if(\$x)</tt>, or a VarStepExpression, e.g.
 *    <tt>if(\$x/bib/book)</tt> or <tt>if(/bib//book)</tt> respectively, as argument
 *    and returns <tt>true</tt> if the sequence located through the entered VarExpression
 *    or the entered VarStepExpression respectively contains one or more items, <tt>false</tt>
 *    otherwise.
 *  @note This expression does not exists instead it is rewritten to <tt>fn:exists</tt>.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class VarstepCondExpression:public CondExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a VarExpression.
     *  @param[in] _var Pointer to a VarExpression object.
     */
    VarstepCondExpression(VarExpression * _var);

    /*! @brief Constructor.
     *  @details Constructor - creating object for a VarStepExpression.
     *  @param[in] _varstep Pointer to a VarStepExpression object.
     */
    VarstepCondExpression(VarStepExpression * _varstep);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ VarstepCondExpression();

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

    /*! @fn virtual void extractDependencies(vector<DependencySet*>* depset)
     *  @copydoc Expression::extractDependencies(vector<DependencySet*>* depset)
     */
    virtual void extractDependencies(vector < DependencySet * >*depset);

    /*! @fn void rewriteVarstepCondExps()
     *  @copydoc Expression::rewriteVarstepCondExps()
     */
    void rewriteVarstepCondExps();

    /*! @fn virtual void print(OutputStream& dos) const
     *  @copydoc Expression::print(OutputStream& dos) const
     */
    virtual void print(OutputStream & dos) const;

    /*! @fn virtual void init(BufferNode* root)
     *  @copydoc Expression::init(BufferNode* root)
     */
    virtual void init(BufferNode * root);

    /*! @fn virtual bool evalCond(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc CondExpression::evalCond(OutputStream& eos, Environment* env, unsigned modus)
     */
    virtual bool evalCond(OutputStream & eos, Environment * env,
                          unsigned modus);

  private:
    /*! @var VarExpression* var
     *  @brief The entered VarExpression.
     *  @details The entered VarExpression, which is the argument of the constructor.
     *    This member variable is <tt>NULL</tt> if the constructor was called with a VarStepExpression.
     */
     VarExpression * var;

    /*! @var VarStepExpression* varstep
     *  @brief The entered VarStepExpression.
     *  @details The entered VarStepExpression, which is the argument of the constructor.
     *    This member variable is <tt>NULL</tt> if the constructor was called with a VarExpression. 
     */
    VarStepExpression *varstep;

    /*! @var ExistsCondExpression* econd
     *  @brief The rewritten ExistsCondExpression of the entered VarExpression or VarStepExpression.
     *  @details The rewritten ExistsCondExpression of the entered VarExpression or VarStepExpression
     *    after calling method rewriteVarstepCondExps(). This member variable is <tt>NULL</tt>
     *    on creation of object but should not be <tt>NULL</tt> anymore after method
     *    rewriteVarstepCondExps() has been executed.
     */
    ExistsCondExpression *econd;
};

#endif // VARSTEPCONDEXPRESSION_H
