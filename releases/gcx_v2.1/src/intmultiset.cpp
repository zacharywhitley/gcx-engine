
/*
 | Author: Michael Schmidt
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
 *  @brief Implementations of header file intmultiset.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file intmultiset.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "intmultiset.h"
#include "outputstream.h"

IntMultiSet::IntMultiSet() {
    max = RoleList::getInstance()->getMaxRoleId();
    elems = new unsigned[max + 1];

    for (unsigned i = 0; (int) i <= max; i++) {
        elems[i] = 0;
    }
}

IntMultiSet::IntMultiSet(vector < unsigned >*_elems) {
    max = RoleList::getInstance()->getMaxRoleId();
    elems = new unsigned[max + 1];

    for (unsigned i = 0; (int) i <= max; i++) {
        elems[i] = 0;
    }
    for (unsigned i = 0; i < _elems->size(); i++) {
        elems[(*_elems)[i]]++;
    }
}

IntMultiSet::~IntMultiSet() {
    delete[]elems;
}

void IntMultiSet::addElems(IntMultiSet & set) {
    vector < unsigned >_elems = set.getElemVec();

    for (unsigned i = 0; i < _elems.size(); i++) {
        addElem(_elems[i]);
    }
}

void IntMultiSet::addElems(vector < unsigned >*_elems) {
    for (unsigned i = 0; i < _elems->size(); i++) {
        addElem((*_elems)[i]);
    }
}

void IntMultiSet::print(OutputStream & dos) {
    bool printed = false;

    for (unsigned i = 0; (int) i <= max; i++) {
        if (elems[i] > 0) {
            if (printed)
                dos << ",";
            dos << elems[i] << "xr" << i;
            printed = true;
        }
    }
}

vector < unsigned >IntMultiSet::getElemVec() {
    vector < unsigned >retvec;

    for (unsigned i = 0; (int) i <= max; i++) {
        for (unsigned j = 0; j < elems[i]; j++) {
            retvec.push_back(i);
        }
    }

    return retvec;
}

bool IntMultiSet::isEmpty() {
    for (unsigned i = 0; (int) i <= max; i++) {
        if (elems[i])
            return false;
    }

    return true;
}

bool IntMultiSet::removeElem(unsigned i) {
    if (elems[i] == 0)
        return false;

    elems[i]--;
    return true;
}
