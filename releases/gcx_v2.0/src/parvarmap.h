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
 *     @brief Header file for parvarmap.cpp.
 *     @details Header file specifying constructors, destructor and functions for parvarmap.cpp.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
#ifndef PARVARMAP_H
#define PARVARMAP_H

#include <map>
#include "pathexpression.h"
#include "varname.h"

/*! @typedef map<unsigned, pair<unsigned, PathExpression*> > VarVarPathMap
 *  @brief The underlying map of this object.
 *     @details The underlying map of this object, which maps variables to their parent variable (ParVar) and path.
 */
typedef map<unsigned, pair<unsigned, PathExpression*> > VarVarPathMap;

/*! @class ParVarMap
 *     @brief Represents mapping of variables to their parent variable (ParVar) and path.
 *     @details Represents mapping of variables to their parent variable (ParVar) and path,
 *          which takes therefore two variables, e.g. <tt>ParVar(\$x) = \$y</tt>, and a
 *             PathExpression, e.g. <tt>/bib//book</tt> or <tt>/bib/book/title</tt>, as arguments.
 *  @note Variables are internally represented by numbers!
 *  @note SINGLETON PATTERN!
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
class ParVarMap {

public:
    /*! @fn static ParVarMap* getInstance()
     *  @brief Returns instance of ParVarMap.
     *  @details Returns instance of ParVarMap, i.e. of this object.
     *  @retval ParVarMap* Pointer to a ParVarMap object.
     */
    static ParVarMap* getInstance();
    
    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ParVarMap();

    /*! @fn void insertParVar(unsigned var, unsigned parvar, PathExpression* path, bool direct_output)
     *  @brief Inserts a new variable with its parent variable (ParVar) and path.
     *  @details Inserts a new variable with its parent variable (ParVar) and path.
     *  @param[in] var Variable for which parent variable (ParVar) and path holds.
     *  @param[in] parvar Parent variable (ParVar).
     *  @param[in] path Pointer to a PathExpression object.
     *  @param[in] _direct_output true if the associated for_loop contains direct output, e.g.
     *             in form of a node construct or constant string
     *  @retval void
     */
    void insertParVar(unsigned var, unsigned parvar, PathExpression* path, bool _direct_output);
    
    /*! @fn unsigned getParVar(unsigned var)
     *  @brief Returns the parent variable (ParVar) for the entered variable.
     *  @details Returns the parent variable (ParVar) for the entered variable.
     *  @param[in] var Variable for which parent variable (ParVar) should be obtained.
     *  @retval unsigned Parent variable (ParVar) for the entered variable.
     */
    unsigned getParVar(unsigned var);
    
    /*! @fn PathExpression* getPath(unsigned var)
     *  @brief Returns the path for the entered variable.
     *  @details Returns the path for the entered variable.
     *  @param[in] var Variable for which path should be obtained.
     *  @retval PathExpression* Pointer to a PathExpression object.
     */
    PathExpression* getPath(unsigned var);
    
    /*! @fn vector<unsigned>* getVarsq()
     *  @brief Returns all occurring variables (set VARS<sub>Q</sub>) of the underlying query.
     *  @details Returns all occurring variables (set VARS<sub>Q</sub>) of the underlying query.
     *  @retval vector<unsigned>* Pointer to a vector object containing all occurring variables (set VARS<sub>Q</sub>) of the underlying query.
     */
    vector<unsigned>* getVarsq() { return &varsq; }

    /*! @fn vector<bool> getDirectOutputInfo()
     *  @brief Returns the vector that stores the direct_output info (see member variable).
     */
    vector<bool>* getDirectOutputInfo() { return &direct_output; }

    /*! @fn void print()
     *  @brief Prints all variables with their parent variable (ParVar) and path.
     *  @details Prints all variables with their parent variable (ParVar) and path.
     *  @retval void
     */
    void print();

private:
    /*! @var static ParVarMap* instance
     *  @brief Instance of ParVarMap.
     *  @details Instance of ParVarMap to avoid duplicate existence of this object.
     */
    static ParVarMap* instance;
    
    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    ParVarMap();

    /*! @var VarVarPathMap parvarmap
     *  @brief The underlying map of this object.
     *  @details The underlying map of this object, which maps variables to their parent variable (ParVar) and path.
     */
    VarVarPathMap parvarmap;

    /*! @var vector<unsigned> varsq
     *  @brief All occurring variables (set VARS<sub>Q</sub>) of the underlying query.
     *  @details All occurring variables (set VARS<sub>Q</sub>) of the underlying query.
     */
    vector<unsigned> varsq;

    /*! @var vector<bool> direct_output
     *  @brief Stores, for each variable, whether the associated for-loop contains direct output or not.
     */
    vector<bool> direct_output; 
};

#endif // PARVARMAP_H
