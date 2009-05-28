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
 *     @brief Header file for tokenconfiguration.cpp.
 *     @details Header file specifying constructors, destructor and functions
 *              for tokenconfiguration.cpp.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#ifndef TOKENCONFIGURATION_H
#define TOKENCONFIGURATION_H

#include "projectiontree.h"

/*! @class TokenConfiguration
 *     @brief Represents a token configuration, which describes a set of active
 *            positions in the projection tree during stream preprojection.
 *            
 *     @details A token configuration touroughly describes a Projection DFA state.
 *             Each token configuration is characterized by a multiset of tokens
 *             that are places at path steps in the ProjectionTree. To this end,
 *             the projection tree is labeled (each path step in the tree gets assigned
 *             a unique label), and tokens are placed on top of these labels.
 *             We always distinguish between active and passive tokens, i.e. tokens
 *             describing labels that are currently active, and tokens that describe
 *             previously matched labels (which are important to keep track of descendant axes).
 *     @author Michael Schmidt
 *     @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
class TokenConfiguration {
    
public:
    /*! @brief Constructor.
     *  @details Constructor - creates token configuration, with one button
     *           placed at the root node.
     */
    TokenConfiguration(ProjectionTree* pt);

    /*! @brief Constructor.
     *  @details Constructor - creates a configuration using the parent's
     *           passive tokens as a starting point.
     */
    TokenConfiguration(TokenConfiguration* parent);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~TokenConfiguration();

    /*! @brief Returns the ProjectionTreeLabels object, which contains
               a list of all labels
     *  @details
     *  @retval ProjectionTreeLabels*
     */
    inline ProjectionTreeLabels* getLabels() { return labels; }

    /*! @brief Extracts the role list described by this configuration
     *  @details The role list is computed from the roles that are
     *           associated with the projection tree labels identified
     *           by this configuration.
     *  @param[in] roles The vector to store the roles in
     *                   (pass-by-reference)
     *  @param[in] role_counts The vector to store the associated counts in
     *                         (pass-by-reference)
     *  @retval void
     */
    void createRoleList(vector<Role*>& roles, vector<unsigned>& role_counts);

    /*! @brief Returns true if we are forced to keep all subsequent child tags
     *  @details
     *  @retval bool
     */
    bool forceChildKeep();

    /*! @brief Returns true if the configuration forces us to keep
     *         the whole subtree beneath matching document nodes.
     *  @details
     *  @retval bool
     */
    bool keepSubtree();

    /*! @brief Returns true if the configuration describes a node that
     *         will be written out, i.e. if all descendants of the node
     *         must be kept 
     *  @details
     *  @retval bool
     */
    bool isOutput();

    /*! @brief Returns true if the configuration allows us to skip
     *         the whole subtree for matching document nodes.
     *  @details
     *  @retval bool
     */
    bool skipSubtree();

    /*! @brief Compute the configuration that results when moving
      *        tokens along tag t
      * @details
      * @retval TokenConfiguration* The resulting token configuration.
      */
    TokenConfiguration* applyTag(TAG t);

    /*! @brief Compute the configuration that results when reading
               PCDATA content.
      * @details
      * @retval TokenConfiguration* The resulting token configuration.
      */
    TokenConfiguration* applyText();

    /*! @brief Checks for token configuration being empty.
     *  @details A token configuration is said to be empty if there
     *           are no active and no passive tokens.
     *  @retval bool
     */
    bool isEmpty();

    /*! @brief Checks for the presence of active tokens in the configurarion.
     *  @details
     *  @retval bool
     */
    bool hasActiveToken();

    /*! @brief Returns the active token vector.
     *  @details
     *  @retval vector<unsigne> A copy of the active tokens vector.
     */
    inline vector<unsigned> getActiveTokens() { return active_tokens; }

    /*! @brief Returns the passive token vector.
     *  @details
     *  @retval vector<unsigne> A copy of the passive tokens vector.
     */
    inline vector<unsigned> getPassiveTokens() { return passive_tokens; }

    /*! @brief Returns the number of active tokens at a given position.
     *  @details
     *  @param[i] i The index position.
     *  @retval unsigned The number of active tokens placed at position i.
     */
    inline unsigned getActiveTokensById(unsigned i) { return active_tokens[i]; }

    /*! @brief Returns the number of passive tokens at a given position.
     *  @details
     *  @param[i] i The index position.
     *  @retval unsigned The number of active tokens placed at position i.
     */
    inline unsigned getPassiveTokensById(unsigned i) { return passive_tokens[i]; }

    /*! @brief Sets the active token count at index i to value n
     *  @details
     *  @param[in] i The token index.
     *  @param[in] n Specified number.
     *  @retval void
     */
    inline void setActiveTokens(unsigned i,unsigned n) { active_tokens[i]=n; }

    /*! @brief Sets the passive token count at index i to value n
     *  @details
     *  @param[in] i The token index.
     *  @param[in] n Specified number. 
     *  @retval void
     */
    inline void setPassiveTokens(unsigned i,unsigned n) { passive_tokens[i]=n; }

    /*! @brief Adds n tokens to the passive tokens count at index i
     *  @details
     *  @param[in] i The token index.
     *  @param[in] n Number of tokens to be added. 
     *  @retval void
     */
    inline void addPassiveTokens(unsigned i,unsigned n) { passive_tokens[i]+=n; }

    /*! @brief Debug prints the token configuration
     * @details
     * @param[in] is_text
     * @retval void
     */
    void print(bool is_text=false);

    /*! @brief Sums up the active token count in this and previous configurations.
     *  @details
     *  @param[in] The id of the token the sum is to be computed for.
     *  @retval unsigned
     */
    unsigned sumUpActiveTokenCountFor(unsigned token_id);

    /*! @brief Looks up the active token count in the last config where it is >0.
     *  @details
     *  @param[in] The id of the token count lookup is performed for.
     *  @retval unsigned
     */
    unsigned getLastActiveTokenCountFor(unsigned token_id);

private:
    /*! @var TokenConfiguration* parent
     *  @brief The parent configuration.
     *  @details Is NULL for the root configuration.
    */
    TokenConfiguration* parent;

    /*! @var ProjectionTreeLabels* labels
     *  @brief The label dictionary.
     *  @details The label for token at index i can be simply retrieved
     *           by labels->getLabelById(i).
    */
    ProjectionTreeLabels* labels;

    /*! @var vector<unsigned> active_tokens.
     *  @brief One component of a TokenConfiguration are active tokens
      *  @details The set of active tokens describes the active states in the
      *           projection tree (i.e. the ProjectionTreeLabels) currently
      *           visited. The size of this vector equals to the number of
      *           labels in the dictionary, for instance tokens=(0,2,2,0) means: 
      *           0 tokens placed at label 0, 2 tokens placed at label 1, etc.
     */
    vector<unsigned> active_tokens;

    /*! @var vector<unsigned> passive_tokens
     *  @brief The second component of a TokenConfiguration are the
     *         passive tokens.
     *  @details Passive tokens remember previously visited states, which are
     *           important when descendant axes are involved: in this case,
     *           we are interested in descendants starting from a given config
     *           in every following depth.
     */
    vector<unsigned> passive_tokens;
};

#endif // TOKENCONFIGURATION_H
