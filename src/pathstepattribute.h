
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
 *  @brief Header file for pathstepattribute.cpp.
 *  @details Header file specifying constructors, destructor and functions for pathstepattribute.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PATHSTEPATTRIBUTE_H
#define PATHSTEPATTRIBUTE_H

#include "typeenums.h"

//! @namespace std
using namespace std;

// forward declarations
class OutputStream;

/*! @class PathStepAttribute
 *  @brief Represents base class of all attribute expressions. 
 *  @details Represents base class of all attribute expressions.
 *  @note Currently there is only support for attribute <tt>[position</tt><tt>()=n]</tt>
 *    (whereas <tt>n</tt> denotes a (numeric) position of an item in a sequence). This
 *    information is used internally to determine that only the first item in a sequence
 *    needs to be kept.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class PathStepAttribute {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a ATTRIBUTE_TYPE.
     *  @param[in] _type ATTRIBUTE_TYPE.
     */
    PathStepAttribute(ATTRIBUTE_TYPE _type);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ PathStepAttribute();

    /*! @fn virtual void print(OutputStream& dos) const
     *  @brief Prints the corresponding attribute.
     *  @details Prints the corresponding attribute.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    virtual void print(OutputStream & dos) const = 0;

    /*! @fn inline const ATTRIBUTE_TYPE getType()
     *  @brief Returns the type of the corresponding attribute.
     *  @details Returns the type of the corresponding attribute.
     *  @retval ATTRIBUTE_TYPE ATTRIBUTE_TYPE.
     */
    inline const ATTRIBUTE_TYPE getType() {
        return type;
    }
    /*! @fn virtual PathStepAttribute* clone()
     *  @brief Returns a new, independent copy of the corresponding attribute.
     *  @details Returns a new, independent copy of the corresponding attribute.
     *  @retval PathStepAttribute* Copy of the corresponding attribute or of this object respectively.
     */ virtual PathStepAttribute *clone() = 0;

  private:
    /*! @var ATTRIBUTE_TYPE type
     *  @brief The entered type.
     *  @details The entered type, which is the argument of the constructor and determines
     *    the type of the attribute.
     */
    ATTRIBUTE_TYPE type;
};

#endif // PATHSTEPATTRIBUTE_H
