
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
 *  @brief Header file for aggregatefunctexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for aggregatefunctexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef AGGREGATEFUNCTEXPRESSION_H
#define AGGREGATEFUNCTEXPRESSION_H

#include <iomanip>
#include "operandexpression.h"
#include "varstepexpression.h"
#include "forexpression.h"
#include "miscfunctions.h"

/*! @class AggregateFunctExpression
 *  @brief Represents base class of all aggregate function expressions.
 *  @details Represents base class of all aggregate function expressions.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class AggregateFunctExpression:public OperandExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for an EXP_TYPE and a VarExpression.
     *  @param[in] _type EXP_TYPE.
     *  @param[in] _var Pointer to a VarExpression object.
     */
    AggregateFunctExpression(EXP_TYPE _type, VarExpression * _var);

    /*! @brief Constructor.
     *  @details Constructor - creating object for an EXP_TYPE and a VarStepExpression.
     *  @param[in] _type EXP_TYPE.
     *  @param[in] _varstep Pointer to a VarStepExpression object.
     */
     AggregateFunctExpression(EXP_TYPE _type, VarStepExpression * _varstep);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ AggregateFunctExpression();

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

    /*! @fn void rewriteAggregateFuncts()
     *  @copydoc Expression::rewriteAggregateFuncts()
     */
    void rewriteAggregateFuncts();

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
    /*! @fn virtual long double calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence)
     *  @brief Result calculation of the corresponding aggregate function.
     *  @details Result calculation of the corresponding aggregate function.
     *  @param[in] eos Pointer to a OutputStream object.
     *  @param[in] env Pointer to a Environment object.
     *  @param[in] modus Evaluation modus.
     *  @param[in] empty_sequence Indicator if aggreagte function is called on an empty sequence.
     *  @retval long double Result of the corresponding aggregate function.
     */
    virtual long double calculateValue(OutputStream & eos, Environment * env,
                                       unsigned modus, bool & empty_sequence) =
        0;

  protected:
    /*! @var VarExpression* var
     *  @brief The entered VarExpression.
     *  @details The entered VarExpression, which is the second argument of the constructor.
     *    This member variable is <tt>NULL</tt> if the constructor was called with a VarStepExpression.
     */
    VarExpression * var;

    /*! @var VarStepExpression* varstep
     *  @brief The entered VarStepExpression.
     *  @details The entered VarStepExpression, which is the second argument of the constructor.
     *    This member variable is <tt>NULL</tt> if the constructor was called with a VarExpression.
     */
    VarStepExpression *varstep;

    /*! @var ForExpression* forexp
     *  @brief The rewritten ForExpression of the entered VarExpression or VarStepExpression.
     *  @details The rewritten ForExpression of the entered VarExpression or VarStepExpression
     *    after calling method rewriteAggregateFuncts(). This member variable is <tt>NULL</tt>
     *    on creation of object but should not be <tt>NULL</tt> anymore after method
     *    rewriteAggregateFuncts() has been executed.
     */
    ForExpression *forexp;

    /*! @var BufferIterator* bit
     *  @brief The initialized BufferIterator for the entered VarExpression or VarStepExpression.
     *  @details The initialized BufferIterator for the entered VarExpression or VarStepExpression
     *    after calling method init(). This member variable is <tt>NULL</tt> on creation of object
     *    but should not be <tt>NULL</tt> anymore after method init() has been executed.
     */
    BufferIterator *bit;
};

#endif // AGGREGATEFUNCTEXPRESSION_H
