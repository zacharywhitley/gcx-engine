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
 *     @brief Header file for forexpression.cpp.
 *     @details Header file specifying constructors, destructor and functions for forexpression.cpp.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#ifndef FOREXPRESSION_H
#define FOREXPRESSION_H

#include "expression.h"
#include "varstepexpression.h"
#include "bufferiterator.h"
#include "sequenceexpression.h"

/*! @class ForExpression
 *     @brief Represents an XQuery <tt>for-loop</tt>.
 *     @details Represents an XQuery <tt>for-loop</tt>, which takes a VarExpression, a VarExpression or a
 *             VarStepExpression and a Expression, e.g. <tt>for \$x in /bib//book return \$x/title</tt> or
 *             <tt>for \$y in \$x return \$y//book</tt>, as arguments and introduces a new variable or
 *             returns the next new variable binding of the entered VarExpression or VarStepExpression respectively.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
class ForExpression : public Expression {

public:
    /*! @brief Constructor.
     *     @details Constructor - creating object for two VarExpression and a Expression.
     *     @param[in] _var Pointer to a VarExpression object.
     *     @param[in] _varexp Pointer to a VarExpression object.
     *     @param[in] _exp Pointer to a Expression object.
     */
    ForExpression(VarExpression* _var, VarExpression* _varexp, Expression* _exp);
    
    /*! @brief Constructor.
     *     @details Constructor - creating object for a VarExpression, a VarStepExpression an a Expression.
     *     @param[in] _var Pointer to a VarExpression object.
     *     @param[in] _varstep Pointer to a VarStepExpression object.
     *     @param[in] _exp Pointer to a Expression object.
     */
    ForExpression(VarExpression* _var, VarStepExpression* _varstep, Expression* _exp);
    
    /*! @brief Destructor.
     *     @details Destructor.
     */
    virtual ~ForExpression();

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
     *     @copydoc Expression::mergeSequences()
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
     *     @copydoc Expression::eval(Environment* env, unsigned modus)
     */
    virtual void* eval(Environment* env, unsigned modus);

    /*! @fn virtual BufferNode* evalUpdateForBinding(Environment* env, BufferNode* cur)
     *  @brief Binds the for-loop iteration variable to cur (in a new environment)
     *         and executes the signOffs in this environment.
     */
    virtual void evalSignOffForBinding(Environment* env, BufferNode* cur); 

    /*! @fn containsDirectOutput()
     *     @copydoc Expression::containsDirectOutput()
     */
    virtual bool containsDirectOutput();

private:
    /*! @var VarExpression* var
     *     @brief The entered VarExpression.
     *     @details The entered VarExpression, which is the first argument of the constructor and
     *             defines a new introduced variable of a <tt>for-loop</tt>.
     */
    VarExpression* var;
    
    /*! @var VarExpression* varexp
     *     @brief The entered VarExpression.
     *     @details The entered VarExpression, which is the second argument of the constructor and
     *             defines the iteration base, i.e. the sequence of nodes for iteration, of a <tt>for-loop</tt>.
     *             This member variable is <tt>NULL</tt> if the constructor was called with a VarStepExpression.
     */
    VarExpression* varexp;
    
    /*! @var VarStepExpression* varstep
     *     @brief The entered VarStepExpression.
     *     @details The entered VarStepExpression, which is the second argument of the constructor and
     *             defines the iteration base, i.e. the sequence of nodes for iteration, of a <tt>for-loop</tt>.
     *             This member variable is <tt>NULL</tt> if the constructor was called with a VarExpression.
     */
    VarStepExpression* varstep;
    
    /*! @var Expression* exp
     *     @brief The entered Expression.
     *     @details The entered Expression, which is the third argument of the constructor and
     *             defines the <tt>return-clause</tt> of a <tt>for-loop</tt>.
     */
    Expression* exp;
};

#endif // FOREXPRESSION_H
