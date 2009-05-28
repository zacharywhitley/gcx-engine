
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
 *  @brief Header file for varexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for varexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef VAREXPRESSION_H
#define VAREXPRESSION_H

#include "expression.h"
#include "bufferiterator.h"
#include "operandexpression.h"

/*! @class VarExpression
 *  @brief Represents an XQuery variable.
 *  @details Represents an XQuery variable, e.g. <tt>\$x</tt>, <tt>\$z</tt> or <tt>\$root</tt>,
 *    which takes a string as argument and returns the current binding to a BufferNode for a variable.
 *  @note Variables are internally represented by numbers!
 *  @note To get a variable name from a numeric value a look up in VarName is needed!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class VarExpression:public OperandExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for variable <tt>\$root</tt>.
     */
    VarExpression();

    /*! @brief Constructor.
     *  @details Constructor - creating object for a string (variable).
     *  @param[in] _name Pointer to a char object.
     */
    VarExpression(const char *_name);

    /*! @brief Constructor.
     *  @details Constructor - creating object for a string (variable).
     *  @param[in] _name Pointer to a char object.
     *  @param[in] _force <tt>true</tt> if force insertion of variable into VarName, <tt>false</tt> otherwise.
     */
     VarExpression(const char *_name, bool _force);

    /*! @brief Constructor.
     *  @details Constructor - creating object for an EXP_TYPE and a numeric value of a variable.
     *  @param[in] _type EXP_TYPE.
     *  @param[in] _id Numeric value of a variable.
     */
     VarExpression(EXP_TYPE _type, unsigned _id);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ VarExpression();

    /*! @fn void scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars)
     *  @copydoc Expression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars)
     */
    void scopeCheck(vector < unsigned >&def_vars,
                    vector < unsigned >&introduced_vars,
                    vector < unsigned >&violating_vars);

    /*! @fn void replaceVarId(unsigned old_id, unsigned new_id)
     *  @copydoc Expression::replaceVarId(unsigned old_id, unsigned new_id)
     */
    void replaceVarId(unsigned old_id, unsigned new_id);

    /*! @fn void extractDependencies(vector<DependencySet*>* depset)
     *  @copydoc Expression::extractDependencies(vector<DependencySet*>* depset)
     */
    void extractDependencies(vector < DependencySet * >*depset);

    /*! @fn void print(OutputStream& dos) const
     *  @copydoc Expression::print(OutputStream& dos) const
     */
    void print(OutputStream & dos) const;

    /*! @fn void init(BufferNode* root)
     *  @copydoc Expression::init(BufferNode* root)
     */
    void init(BufferNode * root);

    /*! @fn void eval(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc Expression::eval(OutputStream& eos, Environment* env, unsigned modus)
     */
    void eval(OutputStream & eos, Environment * env, unsigned modus);

    /*! @fn inline unsigned getId()
     *  @brief Returns the current (numeric) value of a variable.
     *  @details Returns the current (numeric) value of a variable, i.e. returns the member variable.
     *  @retval unsigned Current (numeric) value of a variable.
     */
    inline unsigned getId() {
        return id;
    }
    /*! @fn inline void setId(unsigned _id)
     *  @brief Sets a new (numeric) value for a variable.
     *  @details Sets a new (numeric) value for a variable, i.e. sets a new (numeric) value for the member variable.
     *  @param[in] _id New (numeric) value for a variable.
     *  @retval void
     */ inline void setId(unsigned _id) {
        id = _id;
    }

    /*! @fn VarExpression* clone()
     *  @brief Returns a new, independent copy of a variable or of this object respectively.
     *  @details Returns a new, independent copy of a variable or of this object respectively.
     *  @retval VarExpression* Copy of a variable or of this object respectively.
     */
    VarExpression *clone();

    /*! @fn inline bool isSingleValued()
     *  @copydoc OperandExpression::isSingleValued()
     */
    inline bool isSingleValued() {
        return true;
    }

    /*! @fn void prepareOperand(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc OperandExpression::prepareOperand(OutputStream& eos, Environment* env, unsigned modus)
     */
    void prepareOperand(OutputStream & eos, Environment * env, unsigned modus);

    /*! @fn void unprepareOperand()
     *  @copydoc OperandExpression::unprepareOperand()
     */
    void unprepareOperand();

    /*! @fn virtual Value* getNextValue()
     *  @copydoc OperandExpression::getNextValue()
     */
    virtual Value *getNextValue();

  protected:
    /*! @var unsigned id
     *  @brief The entered variable.
     *  @details The entered variable, which is the (first) argument of the constructor and
     *    internally represented by a number.
     */
    unsigned id;

  private:
    /*! @var BufferIterator* bit
     *  @brief The initialized BufferIterator for this variable.
     *  @details The initialized BufferIterator for this variable
     *    after calling method init(). This member variable is <tt>NULL</tt> on creation of object
     *    but should not be <tt>NULL</tt> anymore after method init() has been executed.
     */
    BufferIterator * bit;
};

#endif // VAREXPRESSION_H
