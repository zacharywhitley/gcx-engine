
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
 *  @brief Implementations of header file aggregatefunctmedianexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file aggregatefunctmedianexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "aggregatefunctmedianexpression.h"

AggregateFunctMedianExpression::AggregateFunctMedianExpression(VarExpression * _var):
AggregateFunctExpression(et_aggregatefunctmedian, _var) {
}

AggregateFunctMedianExpression::AggregateFunctMedianExpression(VarStepExpression * _varstep):
AggregateFunctExpression(et_aggregatefunctmedian, _varstep) {
}

AggregateFunctMedianExpression::~AggregateFunctMedianExpression() {
}

void AggregateFunctMedianExpression::eval(OutputStream & eos,
                                          Environment * env, unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
          {
              bool empty_sequence = true;
              long double median_val =
                  calculateValue(eos, env, modus, empty_sequence);
              if (!empty_sequence) {
                  const char *median_str =
                      MiscFunctions::getStringFromNumerical(median_val);
                  eos << median_str;
                  delete[]median_str;
              } else {
                  eos << OUTPUT_MEDIAN_ON_EMPTY_SEQUENCE;
              }
              break;
          }

        case EVAL_QUERY_SILENT:
          {
              bool empty_sequence = true;
              long double median_val =
                  calculateValue(eos, env, modus, empty_sequence);
              if (!empty_sequence) {
                  cur_val.setNumVal(median_val);
              } else {
                  const char *median_empty_val =
                      new char[strlen(OUTPUT_MEDIAN_ON_EMPTY_SEQUENCE) + 1];
                  strcpy((char *) median_empty_val,
                         OUTPUT_MEDIAN_ON_EMPTY_SEQUENCE);
                  cur_val.setStrVal(median_empty_val);
              }
              break;
          }

        case EVAL_SIGNOFF:
            forexp->eval(eos, env, EVAL_SIGNOFF);
            break;

        default:
            throw
                RuntimeException
                ("AggregateFunctMedianExpression: Illegal Evaluation Mode",
                 eid_runtime_illegalmode);
            break;
    }
}

long double AggregateFunctMedianExpression::calculateValue(OutputStream & eos,
                                                           Environment * env,
                                                           unsigned modus,
                                                           bool &
                                                           empty_sequence) {

    var ? bit->init(env->getNodeBinding(var->getId())) :
        bit->init(env->getNodeBinding(varstep->getId()));

    long double median_ret = 0;
    vector < long double >*pcds = new vector < long double >;
    const char *pcd = NULL;

    while (BufferNode * cur =
           bit->getNext(READ_UP_TO_CLOSE_CONTEXT, LOCK_CONTEXT_ALWAYS_CLEAR)) {

        empty_sequence = false;

        pcd = cur->getPCDataRepresentation();
        long double tmp = MiscFunctions::getNumericFromString(pcd);

        pcds->push_back(tmp);

        free((char *) pcd);

        forexp->evalSignOffForBinding(eos, env, cur);
    }
    bit->clear();

    if (!empty_sequence) {
        sort(pcds->begin(), pcds->end());
        if (pcds->size() % 2 == 0) {
            long double tmp =
                MiscFunctions::getSummationFrom((*pcds)[(pcds->size() / 2) - 1],
                                                (*pcds)[(pcds->size() / 2)]);

            median_ret =
                MiscFunctions::getRoundFrom(MiscFunctions::
                                            getDivisionFrom(tmp, 2));
        } else {
            median_ret =
                MiscFunctions::getRoundFrom((*pcds)[(int) (pcds->size() / 2)]);
        }
    }
    delete pcds;

    return median_ret;
}
