
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
 *  @brief Implementations of header file aggregatefunctminexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file aggregatefunctminexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "aggregatefunctminexpression.h"

AggregateFunctMinExpression::AggregateFunctMinExpression(VarExpression * _var):
AggregateFunctExpression(et_aggregatefunctmin, _var) {
}

AggregateFunctMinExpression::AggregateFunctMinExpression(VarStepExpression * _varstep):
AggregateFunctExpression(et_aggregatefunctmin, _varstep) {
}

AggregateFunctMinExpression::~AggregateFunctMinExpression() {
}

void AggregateFunctMinExpression::eval(OutputStream & eos, Environment * env,
                                       unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
          {
              bool empty_sequence = true;
              long double min_val =
                  calculateValue(eos, env, modus, empty_sequence);
              if (!empty_sequence) {
                  const char *min_str =
                      MiscFunctions::getStringFromNumerical(min_val);
                  eos << min_str;
                  delete[]min_str;
              } else {
                  eos << OUTPUT_MIN_ON_EMPTY_SEQUENCE;
              }
              break;
          }

        case EVAL_QUERY_SILENT:
          {
              bool empty_sequence = true;
              long double min_val =
                  calculateValue(eos, env, modus, empty_sequence);
              if (!empty_sequence) {
                  cur_val.setNumVal(min_val);
              } else {
                  const char *min_empty_val =
                      new char[strlen(OUTPUT_MIN_ON_EMPTY_SEQUENCE) + 1];
                  strcpy((char *) min_empty_val, OUTPUT_MIN_ON_EMPTY_SEQUENCE);
                  cur_val.setStrVal(min_empty_val);
              }
              break;
          }

        case EVAL_SIGNOFF:
            forexp->eval(eos, env, EVAL_SIGNOFF);
            break;

        default:
            throw
                RuntimeException
                ("AggregateFunctMinExpression: Illegal Evaluation Mode",
                 eid_runtime_illegalmode);
            break;
    }
}

long double AggregateFunctMinExpression::calculateValue(OutputStream & eos,
                                                        Environment * env,
                                                        unsigned modus,
                                                        bool & empty_sequence) {

    var ? bit->init(env->getNodeBinding(var->getId())) :
        bit->init(env->getNodeBinding(varstep->getId()));

    long double min_ret = 0;
    long double count = 0;
    long double result = 0;
    const char *pcd = NULL;

    while (BufferNode * cur =
           bit->getNext(READ_UP_TO_CLOSE_CONTEXT, LOCK_CONTEXT_ALWAYS_CLEAR)) {

        empty_sequence = false;
        count++;

        pcd = cur->getPCDataRepresentation();

        if (count > 1) {
            result = min(result, MiscFunctions::getNumericFromString(pcd));
        } else {
            result = MiscFunctions::getNumericFromString(pcd);
        }

        free((char *) pcd);

        forexp->evalSignOffForBinding(eos, env, cur);
    }
    bit->clear();

    if (!empty_sequence) {
        min_ret = MiscFunctions::getRoundFrom(result);
    }

    return min_ret;
}
