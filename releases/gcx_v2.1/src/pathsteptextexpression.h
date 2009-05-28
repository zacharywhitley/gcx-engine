
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
 *  @brief Header file for pathsteptextexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for pathsteptextexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PATHSTEPTEXTEXPRESSION_H
#define PATHSTEPTEXTEXPRESSION_H

#include "pathstepexpression.h"
#include "outputstream.h"

/*! @def WEIGHT_NODETEST_TEXT
 *  @brief Weight definition for node test <tt>text()</tt> used to determine the evaluation order
 *    of SignOffExpression if more than one appear in a series.
 *  @details Weight definition for node test <tt>text()</tt> used to determine the evaluation order
 *    of SignOffExpression if more than one appear in a series.
 */
#define WEIGHT_NODETEST_TEXT	4

/*! @class PathStepTextExpression
 *  @brief Represents a single XPath path step of the form <tt>/text()</tt> or <tt>//text()</tt>.
 *  @details Represents a single XPath path step of the form <tt>/text()</tt> or <tt>//text()</tt>,
 *    which takes an AXIS_TYPE (default: child-axis), e.g. <tt>/text()</tt> or <tt>//text()</tt>,
 *    and a PathStepAttribute (if required), e.g. <tt>/text()[position</tt><tt>()=1]</tt> or <tt>//text()[position</tt><tt>()=1]</tt>,
 *    as argument(s).
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class PathStepTextExpression:public PathStepExpression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for an AXIS_TYPE.
     *  @param[in] _axis AXIS_TYPE.
     */
    PathStepTextExpression(AXIS_TYPE _axis);

    /*! @brief Constructor.
     *  @details Constructor - creating object for an AXIS_TYPE and a PathStepAttribute.
     *  @param[in] _axis AXIS_TYPE.
     *  @param[in] _attribute Pointer to a PathStepAttribute object.
     */
     PathStepTextExpression(AXIS_TYPE _axis, PathStepAttribute * _attribute);

    /*! @brief Constructor.
     *  @details Constructor - creating object for (default) AXIS_TYPE <tt>child</tt>.
     */
     PathStepTextExpression();

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ PathStepTextExpression();

    /*! @fn virtual void print(OutputStream& dos) const
     *  @copydoc PathStepExpression::print(OutputStream& dos) const
     */
    virtual void print(OutputStream & dos) const;

    /*! @fn inline bool isMatchingTag(TAG tag)
     *  @copydoc PathStepExpression::isMatchingTag(TAG tag)
     */
    inline bool isMatchingTag(TAG tag) {
        return false;
    }
    /*! @fn PathStepTextExpression* clone()
     *  @copydoc PathStepExpression::clone()
     */ PathStepTextExpression *clone();

    /*! @fn PathStepTextExpression* cloneWithoutAttributes()
     *  @copydoc PathStepExpression::cloneWithoutAttributes()
     */
    PathStepTextExpression *cloneWithoutAttributes();
};

#endif // PATHSTEPTEXTEXPRESSION_H
