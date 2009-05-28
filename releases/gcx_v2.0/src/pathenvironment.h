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
 *     @brief Header file for pathenvironment.cpp.
 *     @details Header file specifying constructors, destructor and functions for pathenvironment.cpp.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#ifndef PATHENVIRONMENT_H
#define PATHENVIRONMENT_H

#include <map>
#include "pathexpression.h"
#include "parvarmap.h"
#include "varname.h"

/*! @typedef map<unsigned,PathExpression*> PEnv
 *  @brief
 *     @details
 */
typedef map<unsigned, PathExpression*> PEnv;

/*! @class PathEnvironment
 *     @brief Represents a path environment for variables.
 *     @details
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
class PathEnvironment {

public:
    /*! @brief Constructor.
     *  @details Constructor - creates the path environment, which binds
     *           all variables in the query to their associated paths.
     *           ATTENTION: call AFTER the ParVarMap has been initialized!
     */
    PathEnvironment();

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~PathEnvironment();

    /*! @brief Inserts a new variable-path pair into the environment
     *  @details
     *  @param[in] var The variable.
     *  @param[in] path The path step expression.
     *  @retval void
     */
    void insertPath(unsigned var, PathExpression* path);

    /*! @brief Returns the passed variable is bound to.
     *  @details
     *  @param[in] var The variable the path is looked up for.
     *  @retval PathExpression* The path expression the variable is bound to.
     */
    PathExpression* getPath(unsigned var);

    /*! @brief Computes the path spanned between two variables.
     *  @details The variables must be connected accordingly.
     *  @param[in] end ID of the end variable.
     *  @param[in] end ID of the start variable.
     *  @retval PathExpression* The path inbetween the two variables.
     */
    PathExpression* getPathBetween(unsigned end, unsigned start);

    /*! @brief Debug prints the PathEnvironment object to STDOUT.
     *  @details
     *  @retval void
     */
    void print();

private:
    /*! @var PEnv env
     *  @brief Stores the path environment.
     */
    PEnv env;
};

#endif // PATHENVIRONMENT_H
