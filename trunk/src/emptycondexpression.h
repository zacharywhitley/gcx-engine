
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
 *  @brief Header file for emptycondexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for emptycondexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef EMPTYCONDEXPRESSION_H
#define EMPTYCONDEXPRESSION_H

#include "condexpression.h"
#include "varstepexpression.h"
#include "notcondexpression.h"
#include "existscondexpression.h"

/*! @class EmptyCondExpression
 *  @brief Represents XQuery standard function <tt>fn:empty</tt>.
 *  @details Represents XQuery standard function <tt>fn:empty</tt>, which takes either a
 *    VarExpression, e.g. <tt>fn:empty(\$x)</tt>, or a VarStepExpression, e.g.
 *    <tt>fn:empty(\$x/bib/book)</tt> or <tt>fn:empty(/bib//book)</tt> respectively, as argument
 *    and returns <tt>true</tt> if the sequence located through the entered VarExpression
 *    or the entered VarStepExpression respectively contains no items, <tt>false</tt>
 *    otherwise.
 *  @note This expression does not exists instead it is rewritten to <tt>fn:not(fn:exists)</tt>.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class EmptyCondExpression:public CondExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a VarExpression.
     *  @param[in] _var Pointer to a VarExpression object.
     */
    EmptyCondExpression(VarExpression * _var);

    /*! @brief Constructor.
     *  @details Constructor - creating object for a VarStepExpression.
     *  @param[in] _varstep Pointer to a VarStepExpression object.
     */
    EmptyCondExpression(VarStepExpression * _varstep);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ EmptyCondExpression();

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

    /*! @fn virtual void extractFSAMap(FSAMap* fsamap, unsigned parent_var)
     *  @copydoc Expression::extractFSAMap(FSAMap* fsamap, unsigned parent_var)
     */
    virtual void extractFSAMap(FSAMap * fsamap, unsigned parent_var);

    /*! @fn virtual void extractParVarMap(ParVarMap* parvarmap)
     *  @copydoc Expression::extractParVarMap(ParVarMap* parvarmap)
     */
    virtual void extractParVarMap(ParVarMap * parvarmap);

    /*! @fn virtual void extractDependencies(vector<DependencySet*>* depset)
     *  @copydoc Expression::extractDependencies(vector<DependencySet*>* depset)
     */
    virtual void extractDependencies(vector < DependencySet * >*depset);

    /*! @fn virtual Expression* placeSignOffs(vector<SignOffExpression*>& signoffs)
     *  @copydoc Expression::placeSignOffs(vector<SignOffExpression*>& signoffs)
     */
    virtual Expression *placeSignOffs(vector < SignOffExpression * >&signoffs);

    /*! @fn void rewriteEmptyFuncts()
     *  @copydoc Expression::rewriteEmptyFuncts()
     */
    void rewriteEmptyFuncts();

    /*! @fn virtual void rewriteAggregateFuncts()
     *  @copydoc Expression::rewriteAggregateFuncts()
     */
    virtual void rewriteAggregateFuncts();

    /*! @fn virtual void print(OutputStream& dos) const
     *  @copydoc Expression::print(OutputStream& dos) const
     */
    virtual void print(OutputStream & dos) const;

    /*! @fn virtual void init(BufferNode* root)
     *  @copydoc Expression::init(BufferNode* root)
     */
    virtual void init(BufferNode * root);

    /*! @fn virtual void eval(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc Expression::eval(OutputStream& eos, Environment* env, unsigned modus)
     */
    virtual void eval(OutputStream & eos, Environment * env, unsigned modus);

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

    /*! @var NotCondExpression* ncond
     *  @brief The rewritten NotCondExpression of this XQuery function <tt>fn:empty</tt> or of the
     *    entered VarExpression or VarStepExpression respectively.
     *  @details The rewritten NotCondExpression of this XQuery function <tt>fn:empty</tt> or of the
     *    entered VarExpression or VarStepExpression respectively after calling method rewriteEmptyFuncts().
     *    This member variable is <tt>NULL</tt> on creation of object but should not be <tt>NULL</tt> anymore
     *    after method rewriteEmptyFuncts() has been executed.
     */
    NotCondExpression *ncond;
};

#endif // EMPTYCONDEXPRESSION_H
