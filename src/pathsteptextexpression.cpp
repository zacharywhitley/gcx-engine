
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
 *  @brief Implementations of header file pathsteptextexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file pathsteptextexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "pathsteptextexpression.h"

PathStepTextExpression::
PathStepTextExpression(AXIS_TYPE _axis):PathStepExpression(et_pathsteptext,
                                                           _axis, NULL) {
}

PathStepTextExpression::PathStepTextExpression(AXIS_TYPE _axis,
                                               PathStepAttribute *
                                               _attribute):PathStepExpression
    (et_pathsteptext, _axis, _attribute) {
}

PathStepTextExpression::PathStepTextExpression():
PathStepExpression(et_pathsteptext, at_child, NULL) {
}

PathStepTextExpression::~PathStepTextExpression() {
}

void PathStepTextExpression::print(OutputStream & dos) const {
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
    dos << "text()";
    if (attribute) {
        dos << *(attribute);
    }
}

PathStepTextExpression *PathStepTextExpression::clone() {
    if (attribute) {
        return new PathStepTextExpression(axis, attribute->clone());
    } else {
        return new PathStepTextExpression(axis);
    }
}

PathStepTextExpression *PathStepTextExpression::cloneWithoutAttributes() {
    return new PathStepTextExpression(axis);
}
