
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
 *  @brief Header file for orcondexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for orcondexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef ORCONDEXPRESSION_H
#define ORCONDEXPRESSION_H

#include "condexpression.h"

/*! @class OrCondExpression
 *  @brief Represents XQuery disjunction <tt>or</tt> of two conditions.
 *  @details Represents XQuery disjunction <tt>or</tt> of two conditions, which takes
 *    therefore two CondExpression, e.g. <tt>fn:false() or fn:true()</tt> or
 *    <tt>\$x/bib/book != /bib//book or fn:count(\$x) < 3</tt>, as arguments and returns
 *    the <tt>or</tt> disjunction of both conditions, i.e. returns <tt>true</tt> if at
 *    least one CondExpression is <tt>true</tt>, <tt>false</tt> otherwise.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class OrCondExpression:public CondExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for two CondExpression.
     *  @param[in] _left Pointer to a CondExpression object.
     *  @param[in] _right Pointer to a CondExpression object.
     */
    OrCondExpression(CondExpression * _left, CondExpression * _right);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ OrCondExpression();

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
    /*! @var CondExpression* left
     *  @brief The entered CondExpression on the left hand side of disjunction <tt>or</tt>.
     *  @details The entered CondExpression on the left hand side of disjunction <tt>or</tt>,
     *    which is the first argument of the constructor.
     */
     CondExpression * left;

    /*! @var CondExpression* right
     *  @brief The entered CondExpression on the right hand side of disjunction <tt>or</tt>.
     *  @details The entered CondExpression on the right hand side of disjunction <tt>or</tt>,
     *    which is the second argument of the constructor.
     */
    CondExpression *right;
};

#endif // ORCONDEXPRESSION_H
