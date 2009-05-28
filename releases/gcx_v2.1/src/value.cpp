
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
 *  @brief Implementations of header file value.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file value.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "value.h"

Value::Value():str_val(NULL), num_val(0), type(xsd_unknown) {
}

Value::Value(const char *_str_val):str_val(_str_val),
num_val(0),
type(xsd_string) {
}

Value::Value(long double _num_val):str_val(NULL),
num_val(_num_val),
type(xsd_numeric) {
}

Value::~Value() {
    reset();
}

void Value::setStrVal(const char *_str_val) {
    free((void *) str_val);
    str_val = _str_val;
    num_val = 0;
    type = xsd_string;
}

void Value::setNumVal(long double _num_val) {
    delete[]str_val;
    str_val = NULL;
    num_val = _num_val;
    type = xsd_numeric;
}

void Value::reset() {
    free((void *) str_val);
    num_val = 0;
    type = xsd_unknown;
}
