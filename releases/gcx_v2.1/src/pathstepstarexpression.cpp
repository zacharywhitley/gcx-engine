
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
 *  @brief Implementations of header file pathstepstarexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file pathstepstarexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "pathstepstarexpression.h"

PathStepStarExpression::
PathStepStarExpression(AXIS_TYPE _axis):PathStepExpression(et_pathstepstar,
                                                           _axis, NULL) {
}

PathStepStarExpression::PathStepStarExpression(AXIS_TYPE _axis,
                                               PathStepAttribute *
                                               _attribute):PathStepExpression
    (et_pathstepstar, _axis, _attribute) {
}

PathStepStarExpression::PathStepStarExpression():
PathStepExpression(et_pathstepstar, at_child, NULL) {
}

PathStepStarExpression::~PathStepStarExpression() {
}

void PathStepStarExpression::print(OutputStream & dos) const {
    switch (axis) {
        case at_child:
            dos << "/";
            break;
        case at_descendant:
            dos << "//";
            break;
        case at_dos:
            dos << "/dos::";
            break;
    }
    dos << "*";
    if (attribute) {
        dos << *(attribute);
    }
}

PathStepStarExpression *PathStepStarExpression::clone() {
    if (attribute) {
        return new PathStepStarExpression(axis, attribute->clone());
    } else {
        return new PathStepStarExpression(axis);
    }
}

PathStepStarExpression *PathStepStarExpression::cloneWithoutAttributes() {
    return new PathStepStarExpression(axis);
}
