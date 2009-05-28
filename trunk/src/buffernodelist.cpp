
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
 *  @brief Implementations of header file buffernodelist.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file buffernodelist.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "buffernodelist.h"
#include "outputstream.h"
#include "buffernode.h"

BufferNodeList::BufferNodeList():
front(NULL), back(NULL) {
}

BufferNodeList::~BufferNodeList() {
    while (front) {
        BufferNode *next = front->r_sibling;
        delete front;

        front = next;
    }
}

void BufferNodeList::push_back(BufferNode * node) {
    if (!front) {
        front = node;
        back = node;
    } else {
        back->r_sibling = node;
        back = node;
    }
    back->r_sibling = NULL;
}

BufferNode *BufferNodeList::operator[] (unsigned index) {
    unsigned _index = 0;
    BufferNode *it = NULL;

    for (it = front; _index != index; it = it->r_sibling) {
        _index++;
    }
    return it;
}

void BufferNodeList::print(OutputStream & dos) {
    for (BufferNode * it = front; it != NULL; it = it->r_sibling) {
        it->debugPrint(dos);
    }
}

void BufferNodeList::removeElem(BufferNode * to_remove, BufferNode * prev) {
    // case 1: deleting first element
    if (prev == NULL) {
        if (back == front) {    // list contains 1 element
            front = NULL;
            back = NULL;
        } else {                // list contains more than 1 element
            front = front->r_sibling;
        }

        // case 2: deleting other than first element
    } else {
        prev->r_sibling = to_remove->r_sibling; // adjust list pointer
        // front remains unchanges
        if (to_remove == back)
            back = prev;        // is we remove the last element, back is changed
    }

    delete to_remove;
}
