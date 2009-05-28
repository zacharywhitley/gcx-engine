
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
 *  @brief Header file for expression.cpp.
 *  @details Header file specifying constructors, destructor and functions for expression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include "environment.h"
#include "typeenums.h"
#include "fsamap.h"
#include "runtimeexception.h"

/*! @def EVAL_QUERY
 *  @brief Predefined variable for use as <tt>third</tt> argument of method eval() and
 *  		represents a query evaluation mode.
 *  @details Predefined variable for use as <tt>third</tt> argument of method eval() and
 *  		represents a query evaluation mode. This is the standard evaluation mode.
 *  		When method eval() is called using this mode, the query is evaluated, the result
 *  		is written to the output stream and signOff-statements are evaluated.
 */
#define EVAL_QUERY    		0

/*! @def EVAL_QUERY_SILENT
 *  @brief Predefined variable for use as <tt>third</tt> argument of method eval() and
 *  		represents a query evaluation mode.
 *  @details Predefined variable for use as <tt>third</tt> argument of method eval() and
 *  		represents a query evaluation mode. Equal to evaluation mode EVAL_QUERY, except that
 *  		in this mode the result is not written to the output stream but passed to the calling
 *  		function instead.
 */
#define EVAL_QUERY_SILENT    1

/*! @def EVAL_SIGNOFF
 *  @brief Predefined variable for use as <tt>third</tt> argument of method eval() and
 *  		represents a query evaluation mode.
 *  @details Predefined variable for use as <tt>third</tt> argument of method eval() and
 *  		represents a query evaluation mode. In this query evaluation mode, no query evaluation
 *  		is performed, but only signOff-statements are evaluated. This mode is used to evaluate
 *     	signOff-statements in the part of if-statements that has not been been matched by the
 *  		if-condition. These signOff-statements must be evaluated, because at projection time we
 *  		do not know the exact condition and, for this reason, assign roles for both parts of the
 *  		expression.
 */
#define EVAL_SIGNOFF    	2

// forward declarations
class PathEnvironment;
class SignOffExpression;
class DependencySet;
class ParVarMap;
class OutputStream;

/*! @class Expression
 *  @brief Represents base class of all expressions.
 *  @details Represents base class of all expressions.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class Expression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for an EXP_TYPE.
     *  @param[in] _type EXP_TYPE.
     */
    Expression(EXP_TYPE _type);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ Expression();

    /*! @fn virtual void scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars)
     *  @brief Returns if all used variables were previously (through a for-clause) defined or
     *                  if a variable leaves their defined scope.
     *  @details Returns if all used variables were previously (through a for-clause) defined or
     *                  if a variable leaves their defined scope.
     *  @param[in] def_vars Vector containing all previously defined variables.
     *  @param[in] introduced_vars Vector containing all up to this expression defined variables.
     *  @param[in] violating_vars Vector containing all variables which violates the scope condition.
     *  @retval void
     */
    virtual void scopeCheck(vector < unsigned >&def_vars,
                            vector < unsigned >&introduced_vars,
                            vector < unsigned >&violating_vars);

    /*! @fn virtual void replaceVarId(unsigned old_id, unsigned new_id)
     *  @brief Replaces all matching variable IDs by a new one.
     *  @details Replaces all matching variable IDs by a new one, which is needed in case
     *                  of calling method rewriteVarsteps().
     *  @param[in] old_id Old variable ID.
     *  @param[in] new_id New variable ID, which should be introduced instead.
     *  @retval void
     */
    virtual void replaceVarId(unsigned old_id, unsigned new_id);

    /*! @fn virtual void mergeSequences()
     *  @brief Merges belonging together XQuery sequences into a single sequence.
     *  @details Merges belonging together XQuery sequences into a single sequence, which is done
     *                  because of better/nicer output look.
     *  @retval void
     */
    virtual void mergeSequences();

    /*! @fn virtual void extractFSAMap(FSAMap* fsamap, unsigned parent_var)
     *  @brief Extracts the needed first straight ancestor (FSA) variables map.
     *  @details Extracts the needed first straight ancestor (FSA) variables map, which is needed for correct
     *                  insertion of SignOffExpression.
     *  @param[in,out] fsamap Pointer to a FSAMap object.
     *  @param[in] parent_var Parent variable.
     *  @retval void
     */
    virtual void extractFSAMap(FSAMap * fsamap, unsigned parent_var);

    /*! @fn virtual void extractParVarMap(ParVarMap* parvarmap)
     *  @brief Extracts the needed parent variable (ParVar) map.
     *  @details Extracts the needed parent variable (ParVar) map, which is needed for constructing
     *                  the VariableTree.
     *  @param[in,out] parvarmap Pointer to a ParVarMap object.
     *  @retval void
     */
    virtual void extractParVarMap(ParVarMap * parvarmap);

    /*! @fn virtual void extractDependencies(vector<DependencySet*>* depset)
     *  @brief Extracts the needed dependencies of a query.
     *  @details Extracts the needed dependencies of a query, which are needed for constructing
     *                  the ProjectionTree.
     *  @param[in,out] depset Pointer to a vector containing Pointer to DependencySet objects.
     *  @retval void
     */
    virtual void extractDependencies(vector < DependencySet * >*depset);

    /*! @fn virtual Expression* placeSignOffs(vector<SignOffExpression*>& signoffs)
     *  @brief Places all signOff-statements in the corresponding query.
     *  @details Places all signOff-statements in the corresponding query, which are needed for
     *                  active garbage collection by removing Role from the BufferNode.
     *  @param[in] signoffs Pointer to a vector containing Pointer to SignOffExpression objects.
     *  @retval Expression* Pointer to a Expression object (following expression in the corresponding query).
     */
    virtual Expression *placeSignOffs(vector < SignOffExpression * >&signoffs);

    /*! @fn virtual void rewriteWhereExps()
     *  @brief Rewrites all where-clauses in the query into equivalent if-expressions.
     *  @details Rewrites all where-clauses in the query into equivalent if-expressions, i.e. rewrites
     *                  all WhereExpression     into equivalent IfExpression.
     *  @retval void
     */
    virtual void rewriteWhereExps();

    /*! @fn virtual void rewriteEmptyFuncts()
     *  @brief Rewrites all <tt>fn:empty</tt> XQuery functions into equivalent <tt>fn:not(fn:exists)</tt> XQuery functions.
     *  @details Rewrites all <tt>fn:empty</tt> XQuery functions into equivalent <tt>fn:not(fn:exists)</tt> XQuery functions.
     *  @retval void
     */
    virtual void rewriteEmptyFuncts();

    /*! @fn virtual void rewriteVarstepCondExps()
     *  @brief Rewrites all <tt>if(\$x)</tt> if-conditions into equivalent <tt>if(fn:not(fn:empty(\$x)))</tt> XQuery expressions.
     *  @details Rewrites all <tt>if(\$x)</tt> if-conditions into equivalent <tt>if(fn:not(fn:empty(\$x)))</tt> XQuery expressions.
     *  @retval void
     */
    virtual void rewriteVarstepCondExps();

    /*! @fn virtual void rewriteAggregateFuncts()
     *  @brief Rewrites all arguments of all aggregate functions into an equivalent for-clause (with return-clause).
     *  @details Rewrites all arguments of all aggregate functions into an equivalent for-clause (with return-clause).
     *  @retval void
     */
    virtual void rewriteAggregateFuncts();

    /*! @fn virtual void rewriteVarsteps()
     *  @brief Rewrites all varstep expressions, which produces output into an equivalent for-clause (with return-clause).
     *  @details Rewrites all varstep expressions, which produces output into an equivalent for-clause (with return-clause).
     *                  This method belongs to the 'earlier execution of signoff-statements' optimization. 
     *  @retval void
     */
    virtual void rewriteVarsteps();

    /*! @fn virtual void print(OutputStream& dos) const
     *  @brief Prints the expression.
     *  @details Prints the expression.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    virtual void print(OutputStream & dos) const = 0;

    /*! @fn virtual void init(BufferNode* root)
     *  @brief Initializes the query for evaluation.
     *  @details Initializes the query for evaluation. Up to now only all required
     *                  BufferIterator objects are created to speed up query evaluation.
     *  @param[in] root Pointer to a BufferNode object.
     *  @retval void
     */
    virtual void init(BufferNode * root);

    /*! @fn virtual void eval(OutputStream& eos, Environment* env, unsigned modus)
     *  @brief Evaluates the expression in the given environment.
     *  @details Evaluates the expression in the given environment. The evaluation takes
     *                  place in the environment that is passed to this method. In addition, the parameter
     *                  modus is one of the following: EVAL_QUERY, EVAL_QUERY_SILENT or EVAL_SIGNOFF
     *                  (see also the corresponding defines for more information). This method implements the
     *                  XQuery semantics for the standard expression types, but also the GCX specific
     *                  extensions like signOff-statements.
     *  @param[in] eos Pointer to a OutputStream object.
     *  @param[in] env Pointer to an Environment object.
     *  @param[in] modus The query evaluation mode.
     *  @exception RuntimeException If illegal evaluation mode.
     */
    virtual void eval(OutputStream & eos, Environment * env, unsigned modus);

    /*! @fn inline const EXP_TYPE getType()
     *  @brief Returns the EXP_TYPE of the corresponding expression.
     *  @details Returns the EXP_TYPE of the corresponding expression.
     *  @retval EXP_TYPE.
     */
    inline const EXP_TYPE getType() {
        return type;
    }
    /*! @fn inline void setType (EXP_TYPE _type)
     *  @brief Sets the EXP_TYPE of the corresponding expression.
     *  @details Sets the EXP_TYPE of the corresponding expression.
     *  @param[in] _type EXP_TYPE.
     *  @retval void
     */ inline void setType(EXP_TYPE _type) {
        type = _type;
    }

    /*! @fn virtual bool containsDirectOutput()
     *  @brief Returns if the corresponding expression produces direct output.
     *  @details Returns if the corresponding expression produces direct output.
     *                      Direct output is meant that this subexpression produces output
     *                      such as node constructs or strings.
     *  @retval bool <tt>true</tt> if the corresponding expression produces direct output,
     *                      <tt>false</tt> otherwise.
     */
    virtual bool containsDirectOutput();

  protected:
    /*! @var TagMap* tagmap
     *  @brief The map which contains mapping of numeric values of tagnames to their string value.
     *  @details The map which contains mapping of numeric values of tagnames to their string value.
     */
    TagMap * tagmap;

    /*! @var EXP_TYPE type
     *  @brief The entered EXP_TYPE.
     *  @details The entered EXP_TYPE, which is the argument of the constructor.
     */
    EXP_TYPE type;
};

#endif // EXPRESSION_H
