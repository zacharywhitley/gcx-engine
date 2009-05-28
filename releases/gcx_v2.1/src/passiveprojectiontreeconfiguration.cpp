
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
 *  @brief Implementations of header file passiveprojectiontreeconfiguration.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file passiveprojectiontreeconfiguration.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "passiveprojectiontreeconfiguration.h"

PassiveProjectionTreeConfiguration::
PassiveProjectionTreeConfiguration(PassiveProjectionTree * ppt) {
    active_nodes.push_back(ppt->getRootNode());
}


PassiveProjectionTreeConfiguration::PassiveProjectionTreeConfiguration(vector < PassiveProjectionTreeNode * >_passive_nodes):
passive_nodes(_passive_nodes) {
}


PassiveProjectionTreeConfiguration::~PassiveProjectionTreeConfiguration() {
}

PassiveProjectionTreeConfiguration *PassiveProjectionTreeConfiguration::
applyTag(TAG t) {

    PassiveProjectionTreeConfiguration *start = new PassiveProjectionTreeConfiguration(passive_nodes);  // this will never change again 

    for (unsigned i = 0; i < active_nodes.size(); i++) {
        PassiveProjectionTreeNode *pptn = active_nodes[i];

        vector < PassiveProjectionTreeNode * >*children = pptn->getChildren();
        bool has_descendantaxis_child = false;

        for (unsigned j = 0; j < children->size(); j++) {
            PassiveProjectionTreeNode *child = (*children)[j];
            PathStepExpression *ps = child->getLabel();

            has_descendantaxis_child |= ps->getAxisType() == at_descendant;
            has_descendantaxis_child |= ps->getAxisType() == at_dos;
            if (ps->isMatchingTag(t)) {
                start->addActiveNode((*children)[j]);
            }
        }
        if (has_descendantaxis_child) {
            start->addPassiveNode(pptn);
        }
    }

    for (unsigned i = 0; i < passive_nodes.size(); i++) {
        PassiveProjectionTreeNode *pptn = passive_nodes[i];

        vector < PassiveProjectionTreeNode * >*children = pptn->getChildren();
        for (unsigned j = 0; j < children->size(); j++) {
            PassiveProjectionTreeNode *child = (*children)[j];
            PathStepExpression *ps = child->getLabel();

            if (ps->getAxisType() == at_descendant
                || ps->getAxisType() == at_dos) {
                if (ps->isMatchingTag(t)) {
                    start->addActiveNode((*children)[j]);
                }
            }
        }
    }

    return start;
}

PassiveProjectionTreeConfiguration *PassiveProjectionTreeConfiguration::
applyText() {
    return NULL;                // will be kept anyway
}
