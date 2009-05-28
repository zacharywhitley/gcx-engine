
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
 *  @brief Header file for sequenceexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for sequenceexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef SEQUENCEEXPRESSION_H
#define SEQUENCEEXPRESSION_H

#include "expression.h"
#include "varstepexpression.h"
#include "forexpression.h"
#include "signoffexpression.h"
#include "outputstreammanip.h"

/*! @class SequenceExpression
 *  @brief Represents a sequence of XQuery expressions.
 *  @details Represents a sequence of XQuery expressions, which takes therefore (two) Expression,
 *    e.g. <tt>\$x, \$y</tt> or <tt>//book, fn:count(\$x)</tt>, as arguments and returns the strictly
 *    sequentially evaluated results of the (two) Expression, i.e. outputs or returns the corresponding
 *    Expression values according to their (input) order.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class SequenceExpression:public Expression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for two Expression.
     *  @param[in] _left Pointer to a Expression object.
     *  @param[in] _right Pointer to a Expression object.
     */
    SequenceExpression(Expression * _left, Expression * _right);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ SequenceExpression();

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

    /*! @fn inline void insertExp(Expression* exp)
     *  @brief Inserts a new expression to this sequence of XQuery expressions.
     *  @details Inserts a new expression to this sequence of XQuery expressions.
     *  @param[in] exp Pointer to a Expression object.
     *  @retval void
     */
    inline void insertExp(Expression * exp) {
        exps.push_back(exp);
    }
    /*! @fn inline void unsetRecursiveDelete()
     *  @brief Resets recursive delete of this object.
     *  @details Resets recursive delete of this object, i.e. on delete of this object
     *    subexpressions will not be deleted recursively.
     *  @retval void
     */ inline void unsetRecursiveDelete() {
        delete_recursively = false;
    }

    /*! @fn virtual bool containsDirectOutput()
     *  @copydoc Expression::containsDirectOutput()
     */
    virtual bool containsDirectOutput();

  private:
    /*! @fn inline vector<Expression*>* getExps()
     *  @brief Returns the sequence of XQuery expressions. 
     *  @details Returns the sequence of XQuery expressions, i.e. returns the member variable.
     *  @retval vector<Expression*>* Pointer to a vector object containing Pointer to Expression objects.
     */
    inline vector < Expression * >*getExps() {
        return &exps;
    }

    /*! @var vector<Expression*> exps
     *  @brief The entered (sequence of) XQuery expressions.
     *  @details The entered (sequence of) XQuery expressions, which are the arguments of the constructor.
     */
    vector < Expression * >exps;

    /*! @var bool delete_recursively
     *  @brief Indicator to change delete behaviour when the destructor is called.
     *  @details <tt>true</tt> will delete sequences recursively, <tt>false</tt> otherwise.
     */
    bool delete_recursively;
};

#endif // SEQUENCEEXPRESSION_H
