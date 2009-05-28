
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
 *  @brief Header file for pathsteptagexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for pathsteptagexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PATHSTEPTAGEXPRESSION_H
#define PATHSTEPTAGEXPRESSION_H

#include "pathstepexpression.h"
#include "outputstream.h"

/*! @def WEIGHT_NODETEST_TAG
 *  @brief Weight definition for node test <tt>tag</tt> used to determine the evaluation order
 *    of SignOffExpression if more than one appear in a series.
 *  @details Weight definition for node test <tt>tag</tt> used to determine the evaluation order
 *    of SignOffExpression if more than one appear in a series.
 */
#define WEIGHT_NODETEST_TAG	2

/*! @class PathStepTagExpression
 *  @brief Represents a single XPath path step of the form <tt>/tag</tt> or <tt>//tag</tt>.
 *  @details Represents a single XPath path step of the form <tt>/tag</tt> or <tt>//tag</tt> (whereas <tt>tag</tt> denotes a tagname),
 *    which takes an AXIS_TYPE (default: child-axis), a string as tagname, e.g. <tt>/bib</tt> or <tt>//book</tt>,
 *    and a PathStepAttribute (if required), e.g. <tt>/tag[position</tt><tt>()=1]</tt> or <tt>//tag[position</tt><tt>()=1]</tt>,
 *    as arguments.
 *  @note Tagnames are internally represented by numbers!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class PathStepTagExpression:public PathStepExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for an AXIS_TYPE and a string (as tagname).
     *  @param[in] _axis AXIS_TYPE.
     *  @param[in] _tag Pointer to a char object.
     */
    PathStepTagExpression(AXIS_TYPE _axis, const char *_tag);

    /*! @brief Constructor.
     *  @details Constructor - creating object for (default) AXIS_TYPE <tt>child</tt> and a string (as tagname).
     *  @param[in] _tag Pointer to a char object.
     */
     PathStepTagExpression(const char *_tag);

    /*! @brief Constructor.
     *  @details Constructor - creating object for an AXIS_TYPE, a TAG, which is the (numeric value of a)
     *    tagname and a PathStepAttribute.
     *  @param[in] _axis AXIS_TYPE.
     *  @param[in] _tag TAG.
     *  @param[in] _attribute Pointer to a PathStepAttribute object.
     */
     PathStepTagExpression(AXIS_TYPE _axis, TAG _tag,
                           PathStepAttribute * _attribute);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ PathStepTagExpression();

    /*! @fn virtual void print(OutputStream& dos) const
     *  @copydoc PathStepExpression::print(OutputStream& dos) const
     */
    virtual void print(OutputStream & dos) const;

    /*! @fn inline TAG getNodeTest()
     *  @brief Returns the node test or the (numeric) value of the tagname respectively.
     *  @details Returns the node test or the (numeric) value of the tagname respectively, which
     *    has been returned after insertion into TagMap.
     *  @retval TAG Numeric value of the tagname.
     */
    inline TAG getNodeTest() {
        return nt;
    }
    /*! @fn inline bool isMatchingTag(TAG tag)
     *  @copydoc PathStepExpression::isMatchingTag(TAG tag)
     */ inline bool isMatchingTag(TAG tag) {
        return nt == tag;
    }

    /*! @fn PathStepTagExpression* clone()
     *  @copydoc PathStepExpression::clone()
     */
    PathStepTagExpression *clone();

    /*! @fn PathStepTagExpression* cloneWithoutAttributes()
     *  @copydoc PathStepExpression::cloneWithoutAttributes()
     */
    PathStepTagExpression *cloneWithoutAttributes();

  private:
    /*! @var TAG nt
     *  @brief The (numeric) value of the entered string of a tagname.
     *  @details The (numeric) value of the entered string of a tagname, which
     *    has been returned after insertion into TagMap.
     */
    TAG nt;
};

#endif // PATHSTEPTAGEXPRESSION_H
