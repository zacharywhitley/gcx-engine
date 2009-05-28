
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
 *  @brief Implementations of header file projectiondfatransitions.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file projectiondfatransitions.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "projectiondfatransitions.h"
#include "projectiondfastate.h"

ProjectionDFATransitions::ProjectionDFATransitions(ProjectionDFAState * _parent):
parent(_parent), text_transition_computed(false), text_transition(NULL) {
}

ProjectionDFATransitions::~ProjectionDFATransitions() {
    delete text_transition;

    hash_map < TAG,
     ProjectionDFAState * >::iterator it = transitions.begin();

    while (it != transitions.end()) {
        delete(ProjectionDFAState *) it->second;
        it++;
    }
}

void ProjectionDFATransitions::print(OutputStream & dos, unsigned indents) {

    // if there is a text() transition, then print it
    if (text_transition) {
        dos << resetIndents() << incrementIndents(indents);
        dos << writeIndents();
        dos << "#PCDATA --> ";
        text_transition->print(dos, indents + 1, true);
    }
    // print the regular transitions
    hash_map < TAG,
    ProjectionDFAState * >::iterator it = transitions.begin();

    while (it != transitions.end()) {
        dos << resetIndents() << incrementIndents(indents);
        dos << writeIndents();
        dos << "<" << TagMap::getInstance()->getTag(it->first) << "> --> ";
        ProjectionDFAState *follow_up = it->second;

        if (follow_up != NULL) {
            follow_up->print(dos, indents + 1);
        } else {
            dos << "NULL" << NEWLINE;
        }
        it++;
    };
}

ProjectionDFAState *ProjectionDFATransitions::takeTransition(TAG t,
                                                             short &t_type) {
    hash_map < TAG,
    ProjectionDFAState * >::iterator it = transitions.find(t);

    if (it == transitions.end()) {
        t_type = TRANSITION_UNKNOWN;
        return NULL;
    } else {
        t_type = TRANSITION_REGULAR;
        return it->second;
    }
}

ProjectionDFAState *ProjectionDFATransitions::takeTextTransition(bool is_root) {
    if (!text_transition_computed) {
        computeTextTransition(is_root);
    }
    return text_transition;
}

void ProjectionDFATransitions::computeTransition(ProjectionDFAState * parent,
                                                 TAG t) {
    TokenConfiguration *new_conf = parent->getTokenConfiguration()->applyTag(t);
    PassiveProjectionTreeConfiguration *new_pconf =
        parent->getPassiveProjectionTreeConfiguration() ==
        NULL ? NULL : parent->getPassiveProjectionTreeConfiguration()->
        applyTag(t);
    if (new_conf) {
        transitions[t] = new ProjectionDFAState(parent, new_conf, new_pconf);
    } else {
        delete new_pconf;

        transitions[t] = NULL;
    }
}

void ProjectionDFATransitions::computeTextTransition(bool is_root) {
    text_transition_computed = true;
    if (is_root) {
        text_transition = NULL;
    } else {
        TokenConfiguration *new_conf =
            parent->getTokenConfiguration()->applyText();
        PassiveProjectionTreeConfiguration *new_pconf =
            parent->getPassiveProjectionTreeConfiguration() ==
            NULL ? NULL : parent->getPassiveProjectionTreeConfiguration()->
            applyText();
        if (new_conf) {
            text_transition =
                new ProjectionDFAState(parent, new_conf, new_pconf);
        } else {
            if (parent->isOutput()) {
                text_transition = new ProjectionDFAState(parent, new_pconf);
            } else {
                delete new_pconf;

                text_transition = NULL;
            }
        }
    }
}
