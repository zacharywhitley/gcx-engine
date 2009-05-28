
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
 *  @brief Header file for projectiondfa.cpp.
 *  @details Header file specifying constructors, destructor and functions for projectiondfa.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PROJECTIONDFA_H
#define PROJECTIONDFA_H

#include "projectiondfastate.h"
#include "passiveprojectiontree.h"
#include "projectiontree.h"

/*! @class ProjectionDFA
 *  @brief Represents the DFA used for stream preprojection.
 *  @details The information from the statical analysis phase are compiled
 *    into this (lazily constructed) DFA. The DFA then can be used to
 *    determine whether tokens must be loaded into the buffer or can be
 *    projected away when successively processing the input stream/file.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class ProjectionDFA {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object using the
     *    ProjectionTree object that has been computed in the statical
     *    analysis phase. Note that the ProjectionDFA is computed
     *    lazily while projecting the input stream, in order to avoid
     *    unnecessary computations (the projection DFA may be unlimited
     *    in size and therefore cannot even be computed in advance)
     *  @param[in] _pt The ProjectionTree object that contains information on required
     *    paths as well as role information.
     *  @param[in] _ppt The PassiveProjectionTree object that contains required paths.
     */
    ProjectionDFA(ProjectionTree * _pt, PassiveProjectionTree * _ppt);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ ProjectionDFA();

    /*! @fn void print(OutputStream& dos)
     *  @brief Debug prints the projection DFA to.
     *  @details Note that only those parts of the projection DFA will be
     *    output that have been computed so far. This means that
     *    the shape of the ProjectionDFA does not only depend on
     *    the input query, but also on the current state in reading
     *    the input document and the input document itself.
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline ProjectionDFAState* getInitialState()
     *  @brief Returns the initial (starting) state of the projection DFA.
     *  @details Returns the initial (starting) state of the projection DFA,
     *    i.e. the state corresponfing to thevirtual XML document root.
     *  @retval ProjectioDFAState* The starting state of the projection DFA.
     */
    inline ProjectionDFAState *getInitialState() {
        return initial;
  } private:
    /*! @var ProjectionDFAState* initial
     *  @brief The initial (starting) state of the projection DFA.
     *  @details This state corresponds to the (virtual) document root of
     *   the XML document.
     */
     ProjectionDFAState * initial;
};

#endif // PROJECTIONDFA_H
