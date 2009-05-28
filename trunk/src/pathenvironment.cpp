
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
 *  @brief Implementations of header file pathenvironment.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file pathenvironment.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "pathenvironment.h"

PathEnvironment::PathEnvironment() {
    env[TAGID_ROOT] = new PathExpression();
    ParVarMap *parvarmap = ParVarMap::getInstance();
    vector < unsigned >*varsq = parvarmap->getVarsq();
    PathExpression *p;
    unsigned parvar;

    vector < PathExpression * >insert_paths;
    vector < PathStepExpression * >*insert_steps;
    for (unsigned i = 1; i < varsq->size(); i++) {      // omit root variable!
        insert_paths.clear();
        p = new PathExpression();
        if (parvarmap->getPath((*varsq)[i])) {
            insert_paths.push_back(parvarmap->getPath((*varsq)[i]));
        }

        parvar = parvarmap->getParVar((*varsq)[i]);
        while (parvar != TAGID_ROOT) {
            if (parvarmap->getPath(parvar)) {
                insert_paths.push_back(parvarmap->getPath(parvar));
            }
            parvar = parvarmap->getParVar(parvar);
        }

        for (int j = insert_paths.size() - 1; j >= 0; j--) {
            insert_steps = insert_paths[j]->getPathSteps();
            for (unsigned k = 0; k < insert_steps->size(); k++) {
                p->addPathStep((*insert_steps)[k]->clone());
            }
        }

        insertPath((*varsq)[i], p);
    }
}

PathEnvironment::~PathEnvironment() {
    PEnv::iterator it = env.begin();

    while (it != env.end()) {
        delete it->second;

        env.erase(it);
        it = env.begin();
    }
}

void PathEnvironment::insertPath(unsigned var, PathExpression * path) {
    env[var] = path;
}

void PathEnvironment::print(OutputStream & dos) {
    PEnv::iterator it;

    for (it = env.begin(); it != env.end(); it++) {
        dos << "$" << VarName::getInstance()->getVarname(it->first) << " -> ";
        if (!it->second->isEmptyPath()) {
            dos << *(it->second);
        } else {
            dos << "%";
        }
        dos << NEWLINE;
    }
}

PathExpression *PathEnvironment::getPath(unsigned var) {
    PEnv::iterator it = env.find(var);

    return it->second;
}

PathExpression *PathEnvironment::getPathBetween(unsigned end, unsigned start) {
    PathExpression *p = new PathExpression();

    PathExpression *end_path = getPath(end);

    for (unsigned i = getPath(start)->getPathSteps()->size();
         i < end_path->getPathSteps()->size(); i++) {
        p->addPathStep(end_path->getPathStepAt(i)->clone());
    }

    return p;
}
