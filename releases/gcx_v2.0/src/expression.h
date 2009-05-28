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
 *  @brief Header file for expression.cpp.
 *  @details Header file specifying constructors, destructor and functions for expression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include "environment.h"
#include "typeenums.h"
#include "prettyprint.h"
#include "fsamap.h"
#include "error.h"

/*! @def EVAL_QUERY
 *  @brief A query evaluation mode.
 *  @details This is the standard evaluation mode. When method eval() is called
 *           using this mode, the query is evaluated, result is written to STDOUT
 *           and signOffs are evaluated.
 */
#define EVAL_QUERY    		0

/*! @def EVAL_QUERY_SILENT
 *  @brief A query evaluation mode.
 *  @details Equal to evaluation mode EVAL_QUERY, except that in this mode the result
 *           is not written to STDOUT but passed to the calling function instead.
 */
#define EVAL_QUERY_SILENT    1

/*! @def EVAL_SIGNODD
 *  @brief A query evaluation mode.
 *  @details In this query evaluation mode, no query evaluation is performed, but
 *           only signOff expressions are evaluated. This mode is used to evaluate
 *           signOff expressions in the part of if-statements that has not been
 *           been matched by the if-condition. These signOffs must be evaluated,
 *           because at projection time we do not know the exact condition and, for
 *           this reason, assign roles for both parts of the expression.
 */
#define EVAL_SIGNOFF    	2

// forward declarations
class PathEnvironment;
class SignOffExpression;
class DependencySet;
class ParVarMap;

/*! @class Expression
 *  @brief Represents the base class of all expressions
 *  @details Represents the base class of all expressions. In this function, the class
 *           is abstract and defines a common set of methods and member variables (such
 *           a type variable, an evaluation template method etc.). Some of these methods
 *           have standard implementations, while others are purely abstract. 
 *  @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class Expression {

public:
    /*! @brief Constructor.
     * 	@details Constructor - creating object for an EXP_TYPE.
     * 	@param[in] _type EXP_TYPE.
     */
    Expression(EXP_TYPE _type);
    
    /*! @brief Destructor.
     * 	@details Destructor.
     */
    virtual ~Expression();    
    
    /*! @fn virtual bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
     *     @brief
     *     @details
     *     @param[in] def_vars Reference to a vector<unsigned> object.
     *     @param[in] introduced_vars Reference to a vector<unsigned> object.
     *     @retval bool
     */
    virtual bool scopeCheck(vector<unsigned>& def_vars,
                            vector<unsigned>& introduced_vars);
    
    /*! @fn virtual void replaceVarId(unsigned old_id, unsigned new_id)
     *  @brief
     *  @details
     *  @param[in] old_id
     *  @param[in] new_id
     *  @retval void
     */
    virtual void replaceVarId(unsigned old_id, unsigned new_id);
    
    /*! @fn virtual void mergeSequences()
     *  @brief
     *  @details
     *  @retval void
     */
    virtual void mergeSequences();
    
    /*! @fn virtual void rewriteVarsteps()
     *  @brief
     *  @details
     *  @retval void
     */
    virtual void rewriteVarsteps();
    
    /*! @fn virtual void extractFSAMap(FSAMap* fsamap, unsigned parent_var)
     *  @brief
     *  @details
     *  @param[in,out] fsamap Pointer to a FSAMap object.
     *  @param[in] parent_var
     *  @retval void
     */
    virtual void extractFSAMap(FSAMap* fsamap, unsigned parent_var);
    
    /*! @fn virtual void extractParVarMap(ParVarMap* parvarmap)
     *  @brief
     *  @details
     *  @param[in,out] parvarmap Pointer to a ParVarMap object.
     *  @retval void
     */
    virtual void extractParVarMap(ParVarMap* parvarmap);
    
    /*! @fn virtual void extractDependencies(vector<DependencySet*>* depset)
     *  @brief
     *  @details
     *  @param[in,out] depset Pointer to a vector<DependencySet*> object.
     *  @retval void
     */
    virtual void extractDependencies(vector<DependencySet*>* depset);

    /*! @fn virtual Expression* placeSignOffs(vector<SignOffExpression*>& signoffs)
     *  @brief
     *  @details
     *  @param[in] signoffs Reference to a vector<SignOffExpression*> object.
     *  @retval Expression*
     */
    virtual Expression* placeSignOffs(vector<SignOffExpression*>& signoffs);
    
    /*! @fn virtual void rewriteAggregateFuncts()
     *  @brief
     *  @details
     *  @retval void
     */
    virtual void rewriteAggregateFuncts();
    
    /*! @fn virtual void print(ostream& o) const
     *  @brief
     *  @details
     *  @param[in] o
     *  @retval void
     */
    virtual void print(ostream& o) const=0;
    
    /*! @fn virtual void* eval(Environment* env, unsigned modus)
     *  @brief Evaluates the current expression object in the given environment. 
     *  @details The evaluation takes place in the environment env that is passed to
     *           the function. In addition, the parameter modus is one of the following:
     *           EVAL_QUERY, EVAL_QUERY_SILENT, EVAL_SIGNOFF (see the corresponding
     *           defines for more information). The eval-method implements the XQuery
     *           semantics for the standard expression types, but also the GCX-specific
     *           extensions like signOff-statetements. The return value must be casted
     *           and depends on the specific expression type.
     *  @param[in] env Pointer to an Environment object.
     *  @param[in] modus The query evaluation mode.
     */
    virtual void* eval(Environment* env, unsigned modus);
    
    /*! @fn inline const EXP_TYPE getType()
     *     @brief
     *  @details
     *     @retval EXP_TYPE 
     */
    inline const EXP_TYPE getType() { return type; }

    /*! @fn bool containsDirectOutput()
     *  @brief Returns true if the expression contains a direct
     *         output-generating subexpressions (such as node construct or string)
     */
    virtual bool containsDirectOutput();

protected:
    PrettyPrint* pp;
    TagMap* tagmap;
    EXP_TYPE type;
};

/*!
 * operator << overloading
 */
inline ostream& operator<<(ostream& o, const Expression& exp) {
    exp.print(o);
    return o;
}

#endif // EXPRESSION_H
