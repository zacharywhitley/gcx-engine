
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
 *  @brief Header file for projectiontreelabels.cpp.
 *  @details Header file specifying constructors, destructor and functions for projectiontreelabels.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PROJECTIONTREELABELS_H
#define PROJECTIONTREELABELS_H

#include <vector>
#include "projectiontreelabel.h"

/*! @class ProjectionTreeLabels
 *  @brief Calculates and stores the set of all projection tree labels.
 *  @details Represents a dictionary that contains all ProjectionTreeLabel
 *    objects that have been extracted from the ProjectionTree.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class ProjectionTreeLabels {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a ProjectionTreeNode.
     *  @param[in] root Pointer to a ProjectionTreeNode object.
     */
    ProjectionTreeLabels(ProjectionTreeNode * root);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ ProjectionTreeLabels();

    /*! @fn void updateParentPredecessorInformation()
     *  @brief Brings the parent and predecessor links of the labels up to date.
     *  @details To be called once after extraction.
     *  @retval void
     */
    void updateParentPredecessorInformation();

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints all projection tree labels.
     *  @details Used for debugging purpose only.
     *  @param[in] dos Reference to the (debug) OutputStream. 
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn void getAllRecursiveDosNodeSuccessors(unsigned id, vector<unsigned>& succ)
     *  @brief Returns the ids of all dos::node() successors recursively.
     *  @details Ignores all successors without dos::node() label; for suchlike
     *    successors, no further recursion is performed.
     *  @param[in] id The id of the token we search the dos::node() successors for.
     *  @param[in] succ Collects the ids of all dos::node() successors.
     *  @retval void
     */
    void getAllRecursiveDosNodeSuccessors(unsigned id,
                                          vector < unsigned >&succ);

    /*! @fn inline ProjectionTreeLabel* getLabelById(unsigned id) 
     *  @brief Returns the label by ID.
     *  @details Implements a straightforward dictionary lookup. 
     *  @param[in] id The ID of the label that is requested.
     *  @retval ProjectionTreeLabel* Pointer to the ProjectionTreeLabel with
     *    the specified ID.
     */
    inline ProjectionTreeLabel *getLabelById(unsigned id) {
        return labels[id];
    }
    /*! @fn inline unsigned nrOfLabels()
     *  @brief Returns the number of all projection tree labels.
     *  @details Returns the number of all projection tree labels.
     *  @retval unsigned The number of projection tree labels.
     */ inline unsigned nrOfLabels() {
        return labels.size();
    }

  private:
    /*! @fn ProjectionTreeLabel* computeProjectionTreeLabels(ProjectionTreeNode* n, unsigned& id, unsigned cur_position)
     *  @brief Help function for projection tree label computation.
     *  @details Recursively calls itself.
     *  @param[in] n The parent node.
     *  @param[in] id The id of the parent node.
     *  @param[in] cur_position The current path step position in the current path.
     *  @retval ProjectionTreeLabel* The label that has been set up.
     */
    ProjectionTreeLabel * computeProjectionTreeLabels(ProjectionTreeNode * n,
                                                      unsigned &id,
                                                      unsigned cur_position);

    /*! @var vector<ProjectionTreeLabel*> labels
     *  @brief Stores the list of all projection tree labels.
     *  @details Stores the list of all projection tree labels.
     */
    vector < ProjectionTreeLabel * >labels;
};

#endif // PROJECTIONTREELABLES_H
