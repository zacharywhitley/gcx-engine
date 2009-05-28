
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
 *  @brief Header file for tokenconfiguration.cpp.
 *  @details Header file specifying constructors, destructor and functions for tokenconfiguration.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef TOKENCONFIGURATION_H
#define TOKENCONFIGURATION_H

#include "projectiontree.h"
#include "runtimeexception.h"

/*! @class TokenConfiguration
 *  @brief Represents a token configuration, which describes a set of active
 *    positions in the projection tree during stream preprojection.        
 *  @details A token configuration touroughly describes a Projection DFA state.
 *    Each token configuration is characterized by a multiset of tokens
 *    that are places at path steps in the ProjectionTree. To this end,
 *    the projection tree is labeled (each path step in the tree gets assigned
 *    a unique label), and tokens are placed on top of these labels.
 *    We always distinguish between active and passive tokens, i.e. tokens
 *    describing labels that are currently active, and tokens that describe
 *    previously matched labels (which are important to keep track of descendant axes).
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class TokenConfiguration {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object with one button placed at the root node.
     *  @param[in] pt Pointer to a ProjectionTree object.
     */
    TokenConfiguration(ProjectionTree * pt);

    /*! @brief Constructor.
     *  @details Constructor - creating object using the parent's
     *    passive tokens as a starting point.
     *  @param[in] parent Pointer to a TokenConfiguration object.
     */
    TokenConfiguration(TokenConfiguration * parent);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ TokenConfiguration();

    /*! @fn void createRoleList(vector<Role*>& roles, vector<unsigned>& role_counts)
     *  @brief Extracts the role list described by this configuration
     *  @details The role list is computed from the roles that are
     *    associated with the projection tree labels identified by this configuration.
     *  @param[in] roles The vector to store the roles in
     *    (pass-by-reference)
     *  @param[in] role_counts The vector to store the associated counts in (pass-by-reference)
     *  @retval void
     */
    void createRoleList(vector < Role * >&roles,
                        vector < unsigned >&role_counts);

    /*! @fn TokenConfiguration* applyTag(TAG t)
     *  @brief Compute the configuration when that results when moving tokens along tag t.
     *  @details Creates a fresh TokenConfiguration object.
     *  @param[in] t The tag that is applied to the current TokenConfiguration.
     *  @retval TokenConfiguration* The resulting token configuration.
     */
    TokenConfiguration *applyTag(TAG t);

    /*! @fn TokenConfiguration* applyText()
     *  @brief Compute the configuration that results when reading PCDATA content.
     *  @details Creates a fresh TokenConfiguration object.
     *  @retval TokenConfiguration* The resulting token configuration.
     */
    TokenConfiguration *applyText();

    /*! @fn inline void addPassiveTokens(unsigned i, unsigned n)
     *  @brief Adds tokens to the passive tokens count.
     *  @details Adds n tokens to the passive tokens count at index i.
     *  @param[in] i The token index.
     *  @param[in] n Number of tokens to be added. 
     *  @retval void
     */
    inline void addPassiveTokens(unsigned i, unsigned n) {
        passive_tokens[i] += n;
    }
    /*! @fn void print(OutputStream& dos, bool is_text)
     *  @brief Debug prints the token configuration
     *  @details This message is for debugging purpose only. 
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @param[in] is_text True if the configuration is a text configuration.
     *  @retval void
     */ void print(OutputStream & dos, bool is_text = false);

    /*! @fn inline vector<unsigned> getActiveTokens()
     *  @brief Returns the active token vector.
     *  @details Returns a copy of the active_token vector member variable.
     *  @retval vector<unsigne> A copy of the active tokens vector.
     */
    inline vector < unsigned >getActiveTokens() {
        return active_tokens;
    }

    /*! @fn inline void setActiveTokens(unsigned i, unsigned n)
     *  @brief Sets the active token count.
     *  @details Sets the active token count at index i to value n.
     *  @param[in] i The token index.
     *  @param[in] n Specified number.
     *  @retval void
     */
    inline void setActiveTokens(unsigned i, unsigned n) {
        active_tokens[i] = n;
    }

    /*! @fn inline vector<unsigned> getPassiveTokens()
     *  @brief Returns the passive token vector.
     *  @details Returns a copy of the passive_token vector member variable.
     *  @retval vector<unsigne> A copy of the passive tokens vector.
     */
    inline vector < unsigned >getPassiveTokens() {
        return passive_tokens;
    }

    /*! @fn inline void setPassiveTokens(unsigned i, unsigned n)
     *  @brief Sets the passive token count.
     *  @details Sets the passive token count at index i to value n
     *  @param[in] i The token index.
     *  @param[in] n Specified number. 
     *  @retval void
     */
    inline void setPassiveTokens(unsigned i, unsigned n) {
        passive_tokens[i] = n;
    }

    /*! @fn inline unsigned getActiveTokensById(unsigned i)
     *  @brief Returns the number of active tokens at a given position.
     *  @details Returns the number of active tokens in the current configuration
     *    at a given position.
     *  @param[in] i The index position.
     *  @retval unsigned The number of active tokens placed at position i.
     */
    inline unsigned getActiveTokensById(unsigned i) {
        return active_tokens[i];
    }

    /*! @fn inline unsigned getPassiveTokensById(unsigned i)
     *  @brief Returns the number of passive tokens at a given position.
     *  @details Returns the number of passive tokens at a given position
     *    by token ID.
     *  @param[in] i The token ID.
     *  @retval unsigned The number of active tokens placed at position i.
     */
    inline unsigned getPassiveTokensById(unsigned i) {
        return passive_tokens[i];
    }

    /*! @fn unsigned getLastActiveTokenCountFor(unsigned token_id)
     *  @brief Lookup the active token count in the last config where the token appears.
     *  @details Recursively investigates the parent token configuration and returns
     *    the number of tokens with the given token_id in the last configuration
     *    that contains more than zero active tokens with the given id .
     *  @param[in] token_id The id of the token count lookup is performed for.
     *  @retval unsigned The respective number of active tokens.
     *  @exception Throws RuntimeException if the TokenConfiguration is invalid,
     *    i.e. the requested token has never been used as a active token in
     *    any ancestor configuration.
     */
    unsigned getLastActiveTokenCountFor(unsigned token_id);

    /*! @fn inline ProjectionTreeLabels* getLabels()
     *  @brief Returns a pointer to the ProjectionTreeLabels object.
     *  @details Returns the associated ProjectionTreeLabels object, which contains
     *  a list of all labels.
     *  @retval ProjectionTreeLabels* Pointer to the ProjectionTreeLabels object.
     */
    inline ProjectionTreeLabels *getLabels() {
        return labels;
    }

    /*! @fn bool isEmpty()
     *  @brief Checks for token configuration being empty.
     *  @details A token configuration is defined to be empty if there are
     *     no active and no passive tokens.
     *  @retval bool True if the current TokenConfiguration is empty, false otherwise.
     */
    bool isEmpty();

    /*! @fn bool isOutput()
     *  @brief Returns true if the configuration describes a node that will be written out.
     *  @details Returns true if the configuration describes a node that will
     *    be written out, i.e. if all descendants of the node must be kept. 
     *  @retval bool True if the TokenConfiguration is an output configuration,
     *    false otherwise.
     */
    bool isOutput();

    /*! @fn bool hasActiveToken()
     *  @brief Checks for active tokens in the current configurarion.
     *  @details Returns true if the configuration contains at least one active
     *    token, false otherwise.
     *  @retval bool True if the configuration contains at least one active token,
     *    false otherwise.
     */
    bool hasActiveToken();

    /*! @fn unsigned sumUpActiveTokenCountFor(unsigned token_id)
     *  @brief Sums up the active token count in this and ancestor configurations.
     *  @details Recursively sums up the active tokens of the current TokenConfiguration
     *    and all ancestor configurations for a given token.
     *  @param[in] token_id The id of the token the sum is to be computed for.
     *  @retval unsigned The aggregated number of active token.
     */
    unsigned sumUpActiveTokenCountFor(unsigned token_id);

    /*! @fn bool forceChildKeep()
     *  @brief Returns true if we are forced to keep all subsequent child tags
     *  @details The method implements the descendant-child axis clash and
     *    covers a special case where we are forced to keep child nodes although
     *    these child nodes are not immediately matched by the projection tree.
     *  @retval bool True if a child-descendant conflict exists and child nodes
     *    must not be discarded.
     */
    bool forceChildKeep();

    /*! @fn bool keepSubtree()
     *  @brief Returns true if the configuration forces us to keep
     *    the whole subtree beneath matching document nodes.
     *  @details This case applies if we are in one or more dos::node() states
     *    and no other states (s.t. no role information must be computed for
     *    subnodes).
     *  @retval bool True if the subtree is kept without any additional role
     *    information.
     */
    bool keepSubtree();

    /*! @fn bool skipSubtree()
     *  @brief Returns true if the configuration allows us to discard the whole subtree.
     *  @details This case applies if the current TokenConfiguration is empty.
     *  @retval bool True if the subtree can be discarded, false otherwise.
     */
    bool skipSubtree();

  private:
    /*! @var TokenConfiguration* parent
     *  @brief The parent configuration.
     *  @details Is NULL for the root configuration.
     */
    TokenConfiguration * parent;

    /*! @var ProjectionTreeLabels* labels
     *  @brief The label dictionary.
     *  @details The label for token at index i can be simply retrieved by labels->getLabelById(i).
     */
    ProjectionTreeLabels *labels;

    /*! @var vector<unsigned> active_tokens.
     *  @brief One component of a TokenConfiguration are active tokens
     *  @details The set of active tokens describes the active states in the
     *    projection tree (i.e. the ProjectionTreeLabels) currently
     *    visited. The size of this vector equals to the number of
     *    labels in the dictionary, for instance tokens=(0,2,2,0) means: 
     *    0 tokens placed at label 0, 2 tokens placed at label 1, etc.
     */
    vector < unsigned >active_tokens;

    /*! @var vector<unsigned> passive_tokens
     *  @brief The second component of a TokenConfiguration are the
     *    passive tokens.
     *  @details Passive tokens remember previously visited states, which are
     *    important when descendant axes are involved: in this case,
     *    we are interested in descendants starting from a given config
     *    in every following depth.
     */
    vector < unsigned >passive_tokens;
};

#endif // TOKENCONFIGURATION_H
