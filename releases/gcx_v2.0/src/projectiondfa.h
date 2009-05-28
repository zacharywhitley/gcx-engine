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
 *     @brief Header file for projectiondfa.cpp.
 *     @details Header file specifying constructors, destructor and functions for projectiondfa.cpp.
 *     @author Michael Schmidt
 *     @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#ifndef PROJECTIONDFA_H
#define PROJECTIONDFA_H

#include "projectiondfastate.h"
#include "passiveprojectiontree.h"
#include "projectiontree.h"

/*! @class ProjectionDFA
 *  @brief Represents the DFA used for stream preprojection.
 *  @details The information from the statical analysis phase are compiled
 *           into this (lazily constructed) DFA. The DFA then can be used to
 *           determine whether tokens must be loaded into the buffer or can be
 *           projected away when successively processing the input stream/file.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class ProjectionDFA {

public:
    /*! @brief Constructor.
     *  @details Constructor - initializes the projection DFA using the
     *           ProjectionTree object that has been computed in the statical
     *           analysis phase. Note that the ProjectionDFA is computed
     *           lazily while projecting the input stream, in order to avoid
     *           unnecessary computations (the projection DFA may be unlimited
     *           in size and therefore cannot even be computed in advance)
     *  @param[in] _pt The ProjectionTree object that contains information on required
     *                 paths as well as role information.
     *  @param[in] _ppt The PassiveProjectionTree object that contains required paths.
     */
    ProjectionDFA(ProjectionTree* _pt,PassiveProjectionTree* _ppt);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ProjectionDFA();

    /*! @brief Debug prints the projection DFA to STDOUT.
     *  @details Note that only those parts of the projection DFA will be
     *           output that have been computed so far. This means that
     *           the shape of the ProjectionDFA does not only depend on
     *           the input query, but also on the current state in reading
     *           the input document and the input document itself.
     *  @retval void
     */
    void print();

    /*! @brief Returns the initial (starting) state of the projection DFA.
     *  @details
     *  @retval ProjectioDFAState* The starting state of the projection DFA.
     */
    inline ProjectionDFAState* getInitialState() { return initial; }

private:
    /*! @var ProjectionDFAState* initial
     *  @brief The initial (=starting) state of the projection DFA.
     */
    ProjectionDFAState* initial;
};

#endif // PROJECTIONDFA_H
