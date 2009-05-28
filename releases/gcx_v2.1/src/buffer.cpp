
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
 *  @brief Implementations of header file buffer.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file buffer.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "buffer.h"

Buffer::Buffer(ProjectionDFA * _pdfa) {
    root = new BufferNode((TAG) TAGID_ROOT, NULL);
    cur = root;

    ProjectionDFAState *is = _pdfa->getInitialState();

    appendRoles(is->getCumulativeRoles(), is->getNonCumulativeRoles());
}

Buffer::~Buffer() {
    delete root;
}

void Buffer::appendTag(TAG tag) {
    BufferNode *t = new BufferNode(tag, cur);

    cur->addChild(t);
    cur = t;
}

void Buffer::appendTag(TAG tag, vector < unsigned >*_cumulative_roles,
                       vector < unsigned >*_non_cumulative_roles) {
    BufferNode *t =
        new BufferNode(tag, cur, _cumulative_roles, _non_cumulative_roles);
    cur->addChild(t);
    cur = t;
}

void Buffer::appendPCData(const char *data) {
    BufferNode *t = new BufferNode(data, cur);

    cur->addChild(t);
}

void Buffer::appendPCData(const char *data,
                          vector < unsigned >*_cumulative_roles,
                          vector < unsigned >*_non_cumulative_roles) {
    BufferNode *t =
        new BufferNode(data, cur, _cumulative_roles, _non_cumulative_roles);
    cur->addChild(t);
}

void Buffer::appendRoles(vector < unsigned >*_cumulative_roles,
                         vector < unsigned >*_non_cumulative_roles) {
    cur->appendRoles(_cumulative_roles, _non_cumulative_roles);
}

void Buffer::print(OutputStream & dos) {
    root->print(dos);
}

void Buffer::printCurrent(OutputStream & dos) {
    cur->print(dos);
}

void Buffer::debugPrint(OutputStream & dos) {
    root->debugPrint(dos);
}

void Buffer::closeTag() {
    cur->close();
    cur = cur->parent;
}
