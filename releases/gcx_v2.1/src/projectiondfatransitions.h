
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
 *  @brief Header file for projectiondfatransitions.cpp.
 *  @details Header file specifying constructors, destructor and functions for projectiondfatransitions.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PROJECTIONDFATRANSITIONS_H
#define PROJECTIONDFATRANSITIONS_H

#include "tagmap.h"
#include "tokenconfiguration.h"

#ifdef __GNUC__
#include <ext/hash_map>
#else
#include <hash_map>
#endif

/*! @def TRANSITION_UNKNOWN 
 *  @brief Definie for transitions of unknown type.
 *  @details This transition type should only be used for initialization purposes and has no meaning by itself.
 *         
 */
#define TRANSITION_UNKNOWN            0

/*! @def TRANSITION_REGULAR
 *  @brief Define for regular transitions in a new state.
 *  @details More about 'non-regular' transitions can be found in the description of the
 *    defines for TRANSITION_KEEP_SUBTREE and TRANSITION_SKIP_SUBTREE.
 */
#define TRANSITION_REGULAR            1

/*! @def TRANSITION_SKIP_SUBTREE
 *  @brief Define for skip-subtree transitions.
 *  @details Transitions of this type specify that input document nodes associated with the
 *    state reached through the transition can be skipped, including their subtrees.
 */
#define TRANSITION_SKIP_SUBTREE        2

/*! @def TRANSITION_KEEP_SUBTREE
 *  @brief Define for keep-subtree transitions.
 *  @details Transitions of this type specify that input document nodes associated with the
 *    state reached through the transition must be appended to the buffer, including
 *    all their subtrees (furthermore, nodes in the subtree will never be assigned any roles).
 */
#define TRANSITION_KEEP_SUBTREE        3

using __gnu_cxx::hash_map;

// forward declarations
class ProjectionDFAState;

/*! @class ProjectionDFATransitions
 *  @brief Represents a set of transition out of a projection DFA state.
 *  @details Besides the transition itself, the class contains methods
 *    for computing transitions and transition lookup.
 *  @note the class is initially empty; the caller must assert that computeTransitions
 *    is called if a call to takeTransitions is not successful (i.e. return TRANSITION_UNKNWON)
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class ProjectionDFATransitions {

  public:
    /*! @brief Constructor.
     *  @details Consructor - creating object for a ProjectionDFATransitions object
     *    associated with the passed ProjectionDFAState object.
     *  @param[in] _parent The ProjectionDFAState the transition is linked to.
     */
    ProjectionDFATransitions(ProjectionDFAState * _parent);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ ProjectionDFATransitions();

    /*! @fn void print(OutputStream& dos, unsigned indents)
     *  @brief Prints the object to the given output stream.
     *  @details This is a pretty print method that outputs all
     *    defined transitions (in particular those that have been computed so far).
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @param[in] indents Defines whitespace to be printed (pretty printing).
     *  @retval void
     */
    void print(OutputStream & dos, unsigned indents);

    /*! @fn ProjectionDFAState* takeTransition(TAG t, short& t_type)
     *  @brief Returns the state reached through the transition for tag t.
     *  @details May return NULL is the transition is not defined. The second
     *    parameter t_type is also set by the method, see the parameter
     *    description for more information.
     *  @param[in] t The tag for which the transition is searched for.
     *  @param[in] t_type The transition argument must be provided and is set
     *    by this method. It will take one of the followin
     *    values: TRANSITION_KEEP_SUBTREE means that, for tag t,
     *    the whole subtree must be remembered;
     *    TRANSITION_SKIP_SUBTREE means that, for tag t, the
     *    whole substree must be skipped; TRANSITION_REGULAR
     *    means that, for tag t, there is a regular transition
     *    into the state that is passed as return value).
     *  @retval ProjectionDFAState* The state reached under tag t.
     */
    ProjectionDFAState *takeTransition(TAG t, short &t_type);

    /*! @fn ProjectionDFAState* takeTextTransition(bool is_root)
     *  @brief Returns the projection DFA state reached through the text transition.
     *  @details The text transition is the transition that is taken when PCDATA is
     *    encountered in the ProjectionDFAState associated to this transition (i.e. the parent member variable)
     *  @param[in] is_root True if the state for which transition is computed is the root state. 
     *  @retval ProjectionDFAState* State reached through the text transition.
     */
    ProjectionDFAState *takeTextTransition(bool is_root);

    /*! @fn void computeTransition(ProjectionDFAState* parent, TAG t)
     *  @brief Computes the transition under tag t.
     *  @details Computes the transition out of ProjectionDFAState* parent under tag t. 
     *  @param[in] parent The associated ProjectionDFAState* object.
     *  @param[in] t The tag for which the transition is to be computed.
     *  @retval void
     */
    void computeTransition(ProjectionDFAState * parent, TAG t);

  private:
    /*! @fn virtual void computeTextTransition(bool is_root)
     *  @brief Computes the text transition.
     *  @details The text transition is stored in the text_transition variable.
     *    It may become NULL if no text transition exists.
     *  @param[in] is_root True if the state for which transition is computed is the root state. 
     *  @retval void
     */
    virtual void computeTextTransition(bool is_root);

    /*! @var ProjectionDFAState* parent
     *  @brief Pointer to thei associated ProjectionDFAState object.
     *  @details Pointer to thei associated ProjectionDFAState object for which this
     *    object stores the transitions.
     */
    ProjectionDFAState *parent;

    /*! @var hash_map<TAG,ProjectionDFAState*> transitions
     *  @brief Hash map storing the transitions for different tags.
     *  @details The map is computed on demand, i.e. its shape depend on the structure of the input
     *    document and the current state in input document processing.
     */
    hash_map < TAG,
    ProjectionDFAState * >transitions;

    /*! @var bool text_transition_computed 
     *  @brief Stores if the text_transition has already been computed, false otherwise.
     *  @details Used to avoid unnecessary (re)computation of this information.
     */
    bool text_transition_computed;

    /*! @var ProjectionDFAState* text_transition
     *  @brief The ProjectionDFAState reached through the text transition
     *  @details May be NULL if no text transition exists.
     */
    ProjectionDFAState *text_transition;
};

#endif // PROJECTIONDFATRANSITIONS_H
