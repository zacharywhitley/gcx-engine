
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
 *  @brief Header file for varstepexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for varstepexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef VARSTEPEXPRESSION_H
#define VARSTEPEXPRESSION_H

#include "varexpression.h"
#include "bufferiterator.h"

/*! @class VarStepExpression
 *  @brief Represents an XQuery variable with following path (or a path without a preceding variable).
 *  @details Represents an XQuery variable with following path (or a path without a preceding variable), which takes therefore
 *    a (numeric value of a) variable, e.g. <tt>\$x</tt>, <tt>\$z</tt> or <tt>\$root</tt>, and a PathExpression,
 *    e.g. <tt>/bib//book</tt> or <tt>//node()/text()</tt>, as arguments and returns the sequence of BufferNode located
 *    through the entered PathExpression.
 *  @note ALL PathExpression without a preceeding variable are initialized with variable <tt>\$root</tt>!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class VarStepExpression:public VarExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a variable and a PathExpression.
     *  @param[in] _id Preceeding variable of the entered PathExpression (second argument).
     *  @param[in] _path Pointer to a PathExpression object.
     */
    VarStepExpression(unsigned _id, PathExpression * _path);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ VarStepExpression();

    /*! @fn void extractDependencies(vector<DependencySet*>* depset)
     *  @copydoc VarExpression::extractDependencies(vector<DependencySet*>* depset)
     */
    void extractDependencies(vector < DependencySet * >*depset);

    /*! @fn void print(OutputStream& dos) const
     *  @copydoc VarExpression::print(OutputStream& dos) const
     */
    void print(OutputStream & dos) const;

    /*! @fn void init(BufferNode* root)
     *  @copydoc VarExpression::init(BufferNode* root)
     */
    void init(BufferNode * root);

    /*! @fn void eval(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc VarExpression::eval(OutputStream& eos, Environment* env, unsigned modus)
     */
    void eval(OutputStream & eos, Environment * env, unsigned modus);

    /*! @fn inline PathExpression* getPath()
     *  @brief Returns the PathExpression (path of this object).
     *  @details Returns the PathExpression (path of this object), i.e. returns the member variable.
     *  @retval PathExpression* Pointer to a PathExpression object.
     */
    inline PathExpression *getPath() {
        return path;
    }
    /*! @fn VarStepExpression* clone ()
     *  @brief Returns a new, independent copy of this object.
     *  @details Returns a new, independent copy of this object.
     *  @retval VarStepExpression* Copy of this object.
     */ VarStepExpression *clone();

    /*! @fn VarStepExpression* cloneWithoutFinalDosNodeAndAttributes()
     *  @brief Returns a new, independent copy of this object.
     *  @details Returns a new, independent copy of this object without the
     *    tail PathStepExpression <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt> and
     *    without any PathStepAttribute if present.
     *  @retval VarStepExpression* Copy of this object without the tail path step <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt>
     *    and attributes.
     */
    VarStepExpression *cloneWithoutFinalDosNodeAndAttributes();

    /*! @fn inline bool isSingleValued()
     *  @copydoc VarExpression::isSingleValued()
     */
    inline bool isSingleValued() {
        return false;
    }

    /*! @fn void prepareOperand(OutputStream& eos, Environment* env, unsigned modus)
     *  @copydoc VarExpression::prepareOperand(OutputStream& eos, Environment* env, unsigned modus)
     */
    void prepareOperand(OutputStream & eos, Environment * env, unsigned modus);

    /*! @fn void unprepareOperand()
     *  @copydoc VarExpression::unprepareOperand()
     */
    void unprepareOperand();

    /*! @fn virtual Value* getNextValue()
     *  @copydoc VarExpression::getNextValue()
     */
    virtual Value *getNextValue();

  private:
    /*! @var PathExpression* path
     *  @brief The entered PathExpression.
     *  @details The entered PathExpression, which is the second argument of the constructor.
     */
    PathExpression * path;

    /*! @var BufferIterator* bit
     *  @brief The initialized BufferIterator for this path.
     *  @details The initialized BufferIterator for this path
     *    after calling method init(). This member variable is <tt>NULL</tt> on creation of object
     *    but should not be <tt>NULL</tt> anymore after method init() has been executed.
     */
    BufferIterator *bit;
};

#endif // VARSTEPEXPRESSION_H
