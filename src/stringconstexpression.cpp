
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
 *  @brief Implementations of header file stringconstexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file stringconstexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "stringconstexpression.h"

StringConstExpression::
StringConstExpression(const char *_str):ConstExpression(et_stringconst) {
    // we replace special signs, namely: < -> &lt and > -> &gt;
    ostringstream s;

    for (unsigned i = 0; i < strlen(_str); i++) {
        if (_str[i] == '<') {
            s << "&lt;";
        } else if (_str[i] == '>') {
            s << "&gt;";
        } else {
            s << _str[i];
        }
    }
    str = strdup(s.str().c_str());
    delete[]_str;
}

StringConstExpression::~StringConstExpression() {
    free((char *) str);
}

void StringConstExpression::print(OutputStream & dos) const {
    dos << "\"" << str << "\"";
}

void StringConstExpression::eval(OutputStream & eos, Environment * env,
                                 unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
            eos << str;
            break;

        case EVAL_SIGNOFF:
            break;

        default:
            throw
                RuntimeException
                ("StringConstExpression: Illegal Evaluation Mode",
                 eid_runtime_illegalmode);
            break;
    }
}

Value *StringConstExpression::getNextValue() {
    if (initial) {
        initial = false;
        cur_val.setStrVal(strdup(str));
        return &cur_val;
    } else {
        initial = true;
        return NULL;
    }
}
