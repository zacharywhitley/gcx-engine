
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
 *  @brief Header file for pathexpressionadornment.cpp.
 *  @details Header file specifying constructors, destructor and functions for pathexpressionadornment.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PATHEXPRESSSIONADORNMENT_H
#define PATHEXPRESSSIONADORNMENT_H

#include "pathexpression.h"
#include "pathstepstarexpression.h"

/*! @def TAG_SHADOW_FRONT
 *  @details Label name for the shadow front tag used in the path containment
 *    check algorithm.
 *  @details For more details, see the work from Miklau and Suciu cited
 *    in the class description.
 */
#define TAG_SHADOW_FRONT ".shadow_front"

/*! @def TAG_SHADOW_TAIL
 *  @details Label name for the shadow tail tag used in the path containment check algorithm.
 *  @details For details, see the work from Miklau and Suciu cited
 *    in the class description.
 */
#define TAG_SHADOW_TAIL ".shadow_tail"

/*! @class PathExpressionAdornment
 *  @brief An adorned path expression.
 *  @details More information on XPath containment and path expression adornments
 *    can be found in G. Miklau, D. Suciu: Containment and Equivalence of a Fragment
 *    of XPath, in Journal of the ACM, vol. 51, no. 1, 2004. Note that the fragment
 *    covered here slightly differs from the fragment covered in the before-mentioned
 *    publication, so some adjustments and special treatment was necessary (e.g. for
 *    node() and text() node tests). In summary, we implement a sound (but not necessarily)
 *    complete algorithm for checking XPath containment. The XPath containment check is
 *     used for optimization purpose only, so the incompleteness does not harm the
 *     correctness of the implementation.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class PathExpressionAdornment {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a given PathExpression object.
     *  @param[in] _path Pointer to the basing PathExpression object.
     */
    PathExpressionAdornment(PathExpression * _path);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ PathExpressionAdornment();

    /*! @fn void print(OutputStream& dos) const
     *  @brief Prints the object.
     *  @details Used for debugging purpose only.
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @retval void
     */
    void print(OutputStream & dos) const;

    /*! @fn inline PathExpression* getAdornedPath()
     *  @brief Returns the adorned path.
     *  @details The adorned path is set up in the constructor, computed from the
     *   basing path.
     *  @retval PathExpression* The adorned PathExpression represented by this object.
     */
    inline PathExpression *getAdornedPath() {
        return adorned_path;
    }
    /*! @fn inline PathExpression* getRewrittenPath()
     *  @brief Returns the rewritten path.
     *  @details The rewritten path is an intermediate data structure, which represents
     *    a rewritten version of the basng path where, for instance, special path steps
     *    such as dos::node() are transformed or eliminated.
     *  @retval PathExpression* The rewritten path.
     */ inline PathExpression *getRewrittenPath() {
        return rewritten_path;
    }

    /*! @fn inline vector<unsigned>* getPathAdornments()
     *  @brief Returns a pointer to the path adornments associated with
     *    member adorned_path.
     *  @details See the work of Miklau and Suciu cited in the class description
     *    for details on path adornments.
     *  @retval vector<unsigned>* Pointer to the path adornments vector.
     */
    inline vector < unsigned >*getPathAdornments() {
        return &path_adornments;
    }

    /*! @fn inline bool isChildNodePath()
     *  @brief Checks if the original path ends with a child::node() path step.
     *  @details Accounts for special features not covered in the original
     *    work of Miklau and Suciu cited in the class description.
     *  @retval bool True if the original path ends with a child::node()
     *    path step, false otherwise.
     */
    inline bool isChildNodePath() {
        return is_child_node_path;
    }

    /*! @fn inline bool isDescendantNodePath()
     *  @brief Checks if the original path ends with a descendant::node() path step.
     *  @details Accounts for special features not covered in the original
     *    work by Miklau and Suciu cited in the class description.
     *  @retval bool True if the original path ends with a descendant::node()
     *    path step, false otherwise.
     */
    inline bool isDescendantNodePath() {
        return is_descendant_node_path;
    }

    /*! @fn inline bool isDosNodePath()
     *  @brief Checks if the original path ends with a dos::node() path step.
     *  @details Accounts for special features not covered in the original
     *    work by Miklau and Suciu cited in the class description.
     *  @retval bool True if the original path ends with a dos::node()
     *    path step, false otherwise.
     */
    inline bool isDosNodePath() {
        return is_dos_node_path;
    }

    /*! @fn inline bool isChildTextPath()
     *  @brief Checks if the original path ends with a child::text() path step.
     *  @details Accounts for special features not covered in the original
     *    work by Miklau and Suciu cited in the class description.
     *  @retval bool True if the original path ends with a child::text()
     *    path step, false otherwise.
     */
    inline bool isChildTextPath() {
        return is_child_text_path;
    }

    /*! @fn inline bool isDescendantTextPath()
     *  @brief Checks if the original path ends with a descendant::text() path step.
     *  @details Accounts for special features not covered in the original
     *    work by Miklau and Suciu cited in the class description.
     *  @retval bool True if the original path ends with a descendant::text()
     *    path step, false otherwise.
     */
    inline bool isDescendantTextPath() {
        return is_descendant_text_path;
    }

  private:
    /*! @var PathExpression* adorned_path
     *  @brief The associated adorned path.
     *  @details This member is calculated from the original path in the
     *    class constructor. The associated adornment information is stored
     *    in member path_adornments.
     */
    PathExpression * adorned_path;

    /*! @var PathExpression* rewritten_path
     *  @brief An intermediate data structure that accounts for special path
     *    steps not covered in the original work by Miklau and Suciu cited in
     *    the class description.
     *  @details The rewritten path is an intermediate data structure, which represents
     *    a rewritten version of the basng path where, for instance, special path steps
     *    such as dos::node() are transformed or eliminated.
     */
    PathExpression *rewritten_path;

    /*! @var vector<unsigned>path_adornments
     *  @brief The path adornments associated with member adorned_path.
     *  @details See the work of Miklau and Suciu cited in the class description
     *    for details on path adornments.
     */
    vector < unsigned >path_adornments;

    /*! @var bool is_child_node_path
     *  @brief True if the original path ends with child::node path step.
     *  @details Accounts for special features not covered in the original
     *    work by Miklau and Suciu cited in the class description.
     */
    bool is_child_node_path;

    /*! @var bool is_descendant_node_path
     *  @brief True if the original path ends with a descendant::node() path step.
     *  @details Accounts for special features not covered in the original
     *    work by Miklau and Suciu cited in the path description.
     */
    bool is_descendant_node_path;

    /*! @var bool is_dos_node_path
     *  @brief True if the original path ends with a dos::node() path step.
     *  @details Accounts for special features not covered in the original
     *    work by Miklau and Suciu cited in the class description.
     */
    bool is_dos_node_path;

    /*! @var bool is_child_text_path
     *  @brief True if the original path ends with a child::text() path step.
     *  @details Accounts for special features not covered in the original
     *    work by Miklau and Suciu cited in the class description.
     */
    bool is_child_text_path;

    /*! @var bool is_descendant_text_path
     *  @brief True if the original path ends with a descendant::text() path step.
     *  @details Accounts for special features not covered in the original
     *    work by Miklau and Suciu cited in the class description.
     */
    bool is_descendant_text_path;
};

#endif // PATHEXPRESSSIONADORNMENT_H
