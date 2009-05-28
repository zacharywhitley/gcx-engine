
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
 *  @brief Implementations of header file pathstepnodeexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file pathstepnodeexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "pathstepnodeexpression.h"

PathStepNodeExpression::
PathStepNodeExpression(AXIS_TYPE _axis):PathStepExpression(et_pathstepnode,
                                                           _axis, NULL) {
}

PathStepNodeExpression::PathStepNodeExpression(AXIS_TYPE _axis,
                                               PathStepAttribute *
                                               _attribute):PathStepExpression
    (et_pathstepnode, _axis, _attribute) {
}

PathStepNodeExpression::PathStepNodeExpression():
PathStepExpression(et_pathstepnode, at_child, NULL) {
}

PathStepNodeExpression::~PathStepNodeExpression() {
}

void PathStepNodeExpression::print(OutputStream & dos) const {
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
    dos << "node()";
    if (attribute) {
        dos << *(attribute);
    }
}

PathStepNodeExpression *PathStepNodeExpression::clone() {
    if (attribute) {
        return new PathStepNodeExpression(axis, attribute->clone());
    } else {
        return new PathStepNodeExpression(axis);
    }
}

PathStepNodeExpression *PathStepNodeExpression::cloneWithoutAttributes() {
    return new PathStepNodeExpression(axis);
}
