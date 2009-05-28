
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
 *  @brief Implementations of header file fsamap.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file fsamap.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "fsamap.h"
#include "outputstream.h"

FSAMap *FSAMap::instance = NULL;

FSAMap *FSAMap::getInstance() {
    if (instance == NULL) {
        instance = new FSAMap();
    }

    return instance;
}

FSAMap::FSAMap() {
    fsamap[TAGID_ROOT] = TAGID_ROOT;
}

FSAMap::~FSAMap() {
}

void FSAMap::insertFSA(unsigned var, unsigned fsa) {
    fsamap[var] = fsa;
}

void FSAMap::print(OutputStream & dos) {
    VarVarMap::iterator it;

    for (it = fsamap.begin(); it != fsamap.end(); it++) {
        dos << "$" << VarName::getInstance()->getVarname(it->first) << " -> ";
        dos << "$" << VarName::getInstance()->getVarname(it->second) << NEWLINE;
    }
}

unsigned FSAMap::getFSA(unsigned var) {
    VarVarMap::iterator it;

    it = fsamap.find(var);
    return it->second;
}
