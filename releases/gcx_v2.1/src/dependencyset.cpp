
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
 *  @brief Implementations of header file dependencyset.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file dependencyset.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "dependencyset.h"

DependencySet::DependencySet(unsigned _var):var(_var) {
}

DependencySet::~DependencySet() {
    for (unsigned i = 0; i < tuples.size(); i++) {
        delete tuples[i];
    }
}

void DependencySet::insertTuple(PathExpression * path, bool pos, bool dos,
                                bool cond) {
    PathExpression *path_copy = path ? path->clone() : new PathExpression();

    if (pos && !path_copy->isEmptyPath()) {
        path_copy->getTailPathStep()->
            setAttribute(new PathStepAttributePosition(1));
    }
    if (dos) {
        path_copy->addPathStep(new PathStepNodeExpression(at_dos));
    }
    DependencyTuple *tuple = new DependencyTuple(path_copy, cond);

    tuples.push_back(tuple);
}

void DependencySet::print(OutputStream & dos) {
    for (unsigned i = 0; i < tuples.size(); i++) {
        tuples[i]->print(dos);
        if (i != tuples.size() - 1) {
            dos << ", " << NEWLINE << "           ";
        }
    }
}

void DependencySet::removeSyntacticallyEqualTuple(PassiveProjectionTree * ppt) {
    for (unsigned i = 0; i < tuples.size() - 1; i++) {
        for (unsigned j = i + 1; j < tuples.size(); j++) {
            if (tuples[i]->isSyntacticallyEqualTo(tuples[j])) {
                tuples[j]->registerToPassiveProjectionTree(var, ppt);
                delete tuples[j];

                tuples.erase(tuples.begin() + j);
                j--;
            }
        }
    }
}

void DependencySet::removeSemanticallyContainedTuple(PassiveProjectionTree *
                                                     ppt) {
    for (unsigned i = 0; i < tuples.size(); i++) {
        for (unsigned j = 0; j < tuples.size(); j++) {
            if (j != i) {
                if (tuples[j]->isSemanticallyContainedIn(tuples[i])) {
                    tuples[j]->registerToPassiveProjectionTree(var, ppt);
                    delete tuples[j];

                    tuples.erase(tuples.begin() + j);
                    if (j >= i) {
                        j--;
                    } else {
                        i--;
                        if (i < tuples.size()) {
                            j--;
                        } else {
                            j = tuples.size();
                        }
                    }
                }
            }
        }
    }
}
