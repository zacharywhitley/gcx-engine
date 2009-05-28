
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
 *  @brief Header file for condoperandexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for condoperandexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef CONDOPERANDEXPRESSION_H
#define CONDOPERANDEXPRESSION_H

#include "condexpression.h"
#include "operandexpression.h"

/*! @class CondOperandExpression
 *  @brief Represents XQuery comparison of two operands with relational operators
 *    <tt>\<</tt>, <tt>\<=</tt>, <tt>\>=</tt>, <tt>\></tt>, <tt>=</tt> or <tt>!=</tt>.
 *  @details Represents XQuery comparison of two operands with relational operators
 *    <tt>\<</tt>, <tt>\<=</tt>, <tt>\>=</tt>, <tt>\></tt>, <tt>=</tt> or <tt>!=</tt>,
 *    which takes therefore two OperandExpression, e.g. <tt>\$x/bib/book</tt>, <tt>"foo"</tt>
 *    or <tt>fn:count(\$x)</tt>, and a COMP_TYPE, e.g. <tt>=</tt> or <tt>!=</tt>, as arguments
 *    and returns the evaluated value of the two OperandExpression with respect to COMP_TYPE,
 *    i.e. returns <tt>true</tt> if the two OperandExpression are in correct relationship to
 *    the COMP_TYPE, <tt>false</tt> otherwise.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class CondOperandExpression:public CondExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a COMP_TYPE and two OperandExpression.
     *  @param[in] _left Pointer to a OperandExpression object.
     *  @param[in] _op COMP_TYPE.
     *  @param[in] _right Pointer to a OperandExpression object.
     */
    CondOperandExpression(OperandExpression * _left, COMP_TYPE _op,
                          OperandExpression * _right);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ CondOperandExpression();

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

    /*! @fn virtual bool evalCond(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc CondExpression::evalCond(OutputStream& eos, Environment* env, unsigned modus)
     */
    virtual bool evalCond(OutputStream & eos, Environment * env,
                          unsigned modus);

  private:
    /*! @fn bool evalRelOp(Value* left, COMP_TYPE op, Value* right)
     *  @brief Evaluates the two entered Value objects with respect to the entered COMP_TYPE.
     *  @details Evaluates the two entered Value objects with respect to the entered COMP_TYPE
     *    to <tt>true</tt> or <tt>false</tt>.
     *  @param[in] left Pointer to a Value object.
     *  @param[in] op COMP_TYPE.
     *  @param[in] right Pointer to a Value object.
     *  @retval bool <tt>true</tt> if the two Value objects are in correct relationship with
     *    respect to the entered COMP_TYPE, <tt>false</tt> otherwise.
     */
     bool evalRelOp(Value * left, COMP_TYPE op, Value * right);

    /*! @fn COMP_TYPE invertOperator(COMP_TYPE ot)
     *  @brief Inverts the entered COMP_TYPE.
     *  @details Inverts the entered COMP_TYPE, e.g. <tt>\<</tt> results in <tt>\></tt>.
     *  @param[in] ot COMP_TYPE.
     *  @retval COMP_TYPE The inverted COMP_TYPE of the original one.
     */
    COMP_TYPE invertOperator(COMP_TYPE ot);

    /*! @var OperandExpression* left
     *  @brief The entered OperandExpression on the left hand side of the entered COMP_TYPE.
     *  @details The entered OperandExpression on the left hand side of the entered COMP_TYPE,
     *    which is the first argument of the constructor.
     */
    OperandExpression *left;

    /*! @var COMP_TYPE op
     *  @brief The entered COMP_TYPE between the two entered OperandExpression.
     *  @details The entered COMP_TYPE between the two entered OperandExpression,
     *    which is the second argument of the constructor.
     */
    COMP_TYPE op;

    /*! @var OperandExpression* right
     *  @brief The entered OperandExpression on the right hand side of the entered COMP_TYPE.
     *  @details The entered OperandExpression on the right hand side of the entered COMP_TYPE,
     *    which is the third argument of the constructor.
     */
    OperandExpression *right;
};

#endif // CONDOPERANDEXPRESSION_H
