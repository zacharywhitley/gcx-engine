/*
 | Author: Michael Schmidt;
 |         Gunnar Jehl (multi-step paths/aggregate functions/optimizations) 
 ===========================================================================
 ===========================================================================
 | Software License Agreement (BSD License)
 | 
 | Copyright (c) 2006-2007, Saarland University Database Group 
 | All rights reserved.
 |
 | Redistribution and use of this software in source and binary forms,
 | with or without modification, are permitted provided that the following
 | conditions are met:
 | 
 | * Redistributions of source code must retain the above
 |   copyright notice, this list of conditions and the
 |   following disclaimer.
 |
 | * Redistributions in binary form must reproduce the above
 |   copyright notice, this list of conditions and the
 |   following disclaimer in the documentation and/or other
 |   materials provided with the distribution.
 |
 | * Neither the name the of Saarland University Database Group nor the names 
 |   of its contributors may be used to endorse or promote products derived 
 |   from this software without specific prior written permission of the
 |   Saarland University Database Group.
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
*/
/*! @file
 *     @brief Header file for sequenceexpression.cpp.
 *     @details Header file specifying constructors, destructor and functions for sequenceexpression.cpp.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
#ifndef SEQUENCEEXPRESSION_H
#define SEQUENCEEXPRESSION_H

#include "expression.h"
#include "varstepexpression.h"
#include "forexpression.h"
#include "signoffexpression.h"

/*! @class SequenceExpression
 *     @brief Represents a sequence of XQuery expressions.
 *     @details Represents a sequence of XQuery expressions, which takes therefore two Expression,
 *             e.g. <tt>\$x, \$y</tt> or <tt>//book, fn:count(\$x)</tt>, as arguments and returns the strictly
 *             sequentially evaluated results of the two Expression, i.e. outputs or returns the corresponding Expression values
 *          according to their (input) order.
 *  @note A (entered) Expression can also be a SequenceExpression themselves!
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class SequenceExpression : public Expression {

public:
    /*! @brief Constructor.
     *     @details Constructor - creating object for two Expression.
     *     @param[in] _left Pointer to a Expression object.
     *     @param[in] _right Pointer to a Expression object.
     */
    SequenceExpression(Expression* _left, Expression* _right);
    
    /*! @brief Destructor.
     *     @details Destructor.
     */
    virtual ~SequenceExpression();

    /*! @fn virtual bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
     *  @copydoc Expression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
     */
    virtual bool scopeCheck(vector<unsigned>& def_vars,
                                vector<unsigned>& introduced_vars);
    
    /*! @fn virtual void replaceVarId(unsigned old_id, unsigned new_id)
     *     @copydoc Expression::replaceVarId(unsigned old_id, unsigned new_id)
     */
    virtual void replaceVarId(unsigned old_id, unsigned new_id);
    
    /*! @fn virtual void mergeSequences()
     *  @copydoc Expression::mergeSequences()
     */
    virtual void mergeSequences();
    
    /*! @fn virtual void rewriteVarsteps()
     *     @copydoc Expression::rewriteVarsteps()
     */
    virtual void rewriteVarsteps();

    /*! @fn virtual void extractFSAMap(FSAMap* fsamap, unsigned parent_var)
     *     @copydoc Expression::extractFSAMap(FSAMap* fsamap, unsigned parent_var)
     */
    virtual void extractFSAMap(FSAMap* fsamap, unsigned parent_var);
    
    /*! @fn virtual void extractParVarMap(ParVarMap* parvarmap)
     *     @copydoc Expression::extractParVarMap(ParVarMap* parvarmap)
     */
    virtual void extractParVarMap(ParVarMap* parvarmap);
    
    /*! @fn virtual void extractDependencies(vector<DependencySet*>* depset)
     *     @copydoc Expression::extractDependencies(vector<DependencySet*>* depset)
     */
    virtual void extractDependencies(vector<DependencySet*>* depset);
    
    /*! @fn virtual Expression* placeSignOffs(vector<SignOffExpression*>& signoffs)
     *     @copydoc Expression::placeSignOffs(vector<SignOffExpression*>& signoffs)
     */
    virtual Expression* placeSignOffs(vector<SignOffExpression*>& signoffs);
    
    /*! @fn virtual void rewriteAggregateFuncts()
     *     @copydoc Expression::rewriteAggregateFuncts()
     */
    virtual void rewriteAggregateFuncts();
    
    /*! @fn virtual void print(ostream& o) const
     *     @copydoc Expression::print(ostream& o) const
     */
    virtual void print(ostream& o) const;

    /*! @fn virtual void* eval(Environment* env, unsigned modus)
     *  @copydoc Expression::eval(Environment* env, unsigned modus)
     */
    virtual void* eval(Environment* env, unsigned modus);

    /*! @fn void unsetRecursiveDelete()
     *  @brief Specifies that, on delete of the object, the subexpressions will not
     *         be deleted recursively.
     */
    inline void unsetRecursiveDelete() { delete_recursively=false; }

    /*! @fn containsDirectOutput()
     *     @copydoc Expression::containsDirectOutput()
     */
    virtual bool containsDirectOutput();

private:
    /*! @fn inline vector<Expression*>* getExps()
     *  @brief Returns the sequence of Expression. 
     *  @details Returns the sequence of Expression, i.e. returns the member variable.
     *  @retval vector<Expression*>* Pointer to a vector object containing Pointer to Expression objects.
     */
    inline vector<Expression*>* getExps() { return &exps; }
    
    /*! @var vector<Expression*> exps
     *  @brief The entered (sequence of) Expression objects.
     *  @details The entered (sequence of) Expression objects, which are the arguments of the constructor.
     */
    vector<Expression*> exps;

    /*! @var bool delete_recursively
     *  @brief Switch that can be used to change the behavior when the constructor is called.
     *  @details If this variable is set to true, then the sequence will delete itself recursively,
     *           otherwise no recursive deletion takes place.
     */
    bool delete_recursively;
};

#endif // SEQUENCEEXPRESSION_H
