
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
 *  @brief Implementations of header file dependencytuple.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file dependencytuple.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "dependencytuple.h"

DependencyTuple::DependencyTuple(PathExpression * _path, bool _cond):
path(_path), cond(_cond), role(NULL) {
}

DependencyTuple::~DependencyTuple() {
    delete path;
}

void DependencyTuple::print(OutputStream & dos) {
    dos << "<";
    if (path) {
        dos << *(path);
    }
    if (role) {
        dos << ",r" << role->getId();
    }
    dos << ">";
}

bool DependencyTuple::isPosTuple() {
    if (path && path->getTailPathStep()->hasAttribute()) {
        return path->getTailPathStep()->getAttribute()->getType() ==
            at_position;
    }

    return false;
}

bool DependencyTuple::isDosTuple() {
    if (path) {
        return path->getTailPathStep()->isDosNodeStep();
    }

    return false;
}

bool DependencyTuple::isSyntacticallyEqualTo(DependencyTuple * tuple) {
    if (path == NULL) {
        return tuple->getPath() == NULL;
    } else {
        if ((path == NULL && tuple->getPath())
            || (path && tuple->getPath() == NULL)) {
            return false;
        } else {
            return (!(isPosTuple() ^ tuple->isPosTuple())
                    && !(isDosTuple() ^ tuple->isDosTuple())
                    && path->isSyntacticallyEqualTo(tuple->getPath()));
        }
    }
}

bool DependencyTuple::isSemanticallyContainedIn(DependencyTuple * tuple) {
    if (path == NULL) {
        return tuple->getPath() == NULL;
    } else {
        return !path->mightHasChildDescendantConflict(tuple->getPath()) &&
            path->isSemanticallyContainedIn(tuple->getPath());
    }
}

void DependencyTuple::registerToPassiveProjectionTree(unsigned var,
                                                      PassiveProjectionTree *
                                                      ppt) {
    PathExpression *rpath = ppt->getPathEnv()->getPath(var)->clone();

    if (path) {
        for (unsigned i = 0; i < path->getPathSize(); i++) {
            rpath->addPathStep(path->getPathStepAt(i)->clone());
        }
    }
    ppt->registerPath(rpath);
    delete rpath;
}
