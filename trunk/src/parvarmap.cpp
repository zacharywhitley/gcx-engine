
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
 *  @brief Implementations of header file parvarmap.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file parvarmap.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "parvarmap.h"

ParVarMap *ParVarMap::instance = NULL;

ParVarMap *ParVarMap::getInstance() {
    if (instance == NULL) {
        instance = new ParVarMap();
    }

    return instance;
}

ParVarMap::ParVarMap() {
    parvarmap[TAGID_ROOT] = pair < unsigned,
     PathExpression * >(TAGID_ROOT, NULL);

    varsq.push_back(TAGID_ROOT);
    direct_output.push_back(false);
}

ParVarMap::~ParVarMap() {
}

void ParVarMap::insertParVar(unsigned var, unsigned parvar,
                             PathExpression * path, bool _direct_output) {
    parvarmap[var] = pair < unsigned,
    PathExpression * >(parvar, path);

    varsq.push_back(var);
    direct_output.push_back(_direct_output);
}

void ParVarMap::print(OutputStream & dos) {
    VarVarPathMap::iterator it;
    PathExpression *path;

    for (it = parvarmap.begin(); it != parvarmap.end(); it++) {
        if (it == parvarmap.begin() && it->second.first == TAGID_ROOT) {
            dos << "undefined";
        } else {
            dos << "$" << VarName::getInstance()->getVarname(it->second.first);
        }
        path = it->second.second;
        dos << " => ";
        if (path) {
            dos << *(path);
        } else {
            dos << "%";
        }
        dos << NEWLINE;
    }
}

unsigned ParVarMap::getParVar(unsigned var) {
    VarVarPathMap::iterator it;

    it = parvarmap.find(var);
    return it->second.first;
}

PathExpression *ParVarMap::getPath(unsigned var) {
    VarVarPathMap::iterator it;

    it = parvarmap.find(var);
    return it->second.second;
}
