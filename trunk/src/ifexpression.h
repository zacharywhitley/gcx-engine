
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
 *  @brief Header file for ifexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for ifexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef IFEXPRESSION_H
#define IFEXPRESSION_H

#include "expression.h"
#include "condexpression.h"
#include "varstepexpression.h"
#include "forexpression.h"

/*! @class IfExpression
 *  @brief Represents XQuery conditional expression <tt>if-then-else</tt>.
 *  @details Represents XQuery conditional expression <tt>if-then-else</tt>, which takes therefore
 *    one CondExpression and two Expression, e.g. <tt>if (fn:exists(\$x)) then \$x/title else()</tt>,
 *    as arguments and returns the <tt>then</tt> Expression if the CondExpression is <tt>true</tt>,
 *    the <tt>else</tt> Expression otherwise.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class IfExpression:public Expression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for one CondExpression and two Expression.
     *  @param[in] _cond Pointer to a CondExpression object.
     *  @param[in] _thenexp Pointer to a Expression object.
     *  @param[in] _elseexp Pointer to a Expression object.
     */
    IfExpression(CondExpression * _cond, Expression * _thenexp,
                 Expression * _elseexp);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ IfExpression();

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

    /*! @fn virtual void mergeSequences()
     *  @copydoc Expression::mergeSequences()
     */
    virtual void mergeSequences();

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

    /*! @fn virtual void rewriteWhereExps()
     *  @copydoc Expression::rewriteWhereExps()
     */
    virtual void rewriteWhereExps();

    /*! @fn virtual void rewriteEmptyFuncts()
     *  @copydoc Expression::rewriteEmptyFuncts()
     */
    virtual void rewriteEmptyFuncts();

    /*! @fn virtual void rewriteVarstepCondExps()
     *  @copydoc Expression::rewriteVarstepCondExps()
     */
    virtual void rewriteVarstepCondExps();

    /*! @fn virtual void rewriteAggregateFuncts()
     *  @copydoc Expression::rewriteAggregateFuncts()
     */
    virtual void rewriteAggregateFuncts();

    /*! @fn virtual void rewriteVarsteps()
     *  @copydoc Expression::rewriteVarsteps()
     */
    virtual void rewriteVarsteps();

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

    /*! @fn virtual bool containsDirectOutput()
     *  @copydoc Expression::containsDirectOutput()
     */
    virtual bool containsDirectOutput();

  private:
    /*! @var CondExpression* cond
     *  @brief The entered CondExpression.
     *  @details The entered CondExpression, which is the first argument of the constructor and
     *    defines the <tt>if</tt>-condition.
     */
     CondExpression * cond;

    /*! @var Expression* thenexp
     *  @brief The entered Expression.
     *  @details The entered Expression, which is the second argument of the constructor and
     *    defines the <tt>then</tt>-case.
     */
    Expression *thenexp;

    /*! @var Expression* elseexp
     *  @brief The entered Expression.
     *  @details The entered Expression, which is the third argument of the constructor and
     *    defines the <tt>else</tt>-case.
     */
    Expression *elseexp;
};

#endif // IFEXPRESSION_H
