
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
 *  @brief Header file for roundingexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for roundingexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef ROUNDINGEXPRESSION_H
#define ROUNDINGEXPRESSION_H

#include <math.h>
#include "operandexpression.h"
#include "aggregatefunctexpression.h"
#include "outputstream.h"

/*! @class RoundingExpression
 *  @brief Represents base class of all rounding function expressions.
 *  @details Represents base class of all rounding function expressions.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class RoundingExpression:public OperandExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for an EXP_TYPE and a AggregateFunctExpression.
     *  @param[in] _type EXP_TYPE.
     *  @param[in] _aggfunct Pointer to a AggregateFunctExpression object.
     */
    RoundingExpression(EXP_TYPE _type, AggregateFunctExpression * _aggfunct);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ RoundingExpression();

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
    virtual void eval(OutputStream & eos, Environment * env, unsigned modus) =
        0;

    /*! @fn inline bool isSingleValued()
     *  @copydoc OperandExpression::isSingleValued()
     */
    inline bool isSingleValued() {
        return true;
    }
    /*! @fn virtual void prepareOperand(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc OperandExpression::prepareOperand(OutputStream& eos, Environment* env, unsigned modus)
     */ virtual void prepareOperand(OutputStream & eos, Environment * env,
                                    unsigned modus);

    /*! @fn inline void unprepareOperand()
     *  @copydoc OperandExpression::unprepareOperand()
     */
    inline void unprepareOperand() {
        initial = true;
    }

    /*! @fn virtual Value* getNextValue()
     *  @copydoc OperandExpression::getNextValue()
     */
    virtual Value *getNextValue();

  private:
    /*! @fn virtual long double calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& non_value)
     *  @brief Result calculation of the corresponding rounding function.
     *  @details Result calculation of the corresponding rounding function.
     *  @param[in] eos Pointer to a OutputStream object.
     *  @param[in] env Pointer to a Environment object.
     *  @param[in] modus Evaluation modus.
     *  @param[in] non_value Indicator if rounding function is called on an non value, i.e. is
     *    called on an empty sequence of the corresponding aggregate function.
     *  @retval long double Result of the corresponding rounding function.
     */
    virtual long double calculateValue(OutputStream & eos, Environment * env,
                                       unsigned modus, bool & non_value) = 0;

  protected:
    /*! @var AggregateFunctExpression* aggfunct
     *  @brief The entered AggregateFunctExpression.
     *  @details The entered AggregateFunctExpression, which is the argument of the constructor.
     */
    AggregateFunctExpression * aggfunct;
};

#endif // ROUNDINGEXPRESSION_H
