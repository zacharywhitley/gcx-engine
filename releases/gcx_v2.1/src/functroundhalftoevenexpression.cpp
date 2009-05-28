
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
 *  @brief Implementations of header file functroundhalftoevenexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file functroundhalftoevenexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "functroundhalftoevenexpression.h"

FunctRoundHalfToEvenExpression::FunctRoundHalfToEvenExpression(AggregateFunctExpression * _aggfunct):
RoundingExpression(et_roundhalftoeven, _aggfunct) {
}

FunctRoundHalfToEvenExpression::~FunctRoundHalfToEvenExpression() {
}

void FunctRoundHalfToEvenExpression::eval(OutputStream & eos,
                                          Environment * env, unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
          {
              bool non_value = true;
              long double roundhalftoeven_val =
                  calculateValue(eos, env, modus, non_value);
              if (!non_value) {
                  const char *roundhalftoeven_str =
                      MiscFunctions::
                      getStringFromNumerical(roundhalftoeven_val);
                  eos << roundhalftoeven_str;
                  delete[]roundhalftoeven_str;
              } else {
                  eos << OUTPUT_ROUNDHALFTOEVEN_ON_NON_VALUE;
              }
              break;
          }

        case EVAL_QUERY_SILENT:
          {
              bool non_value = true;
              long double roundhalftoeven_val =
                  calculateValue(eos, env, modus, non_value);
              if (!non_value) {
                  cur_val.setNumVal(roundhalftoeven_val);
              } else {
                  const char *roundhalftoeven_non_val =
                      new char[strlen(OUTPUT_ROUNDHALFTOEVEN_ON_NON_VALUE) + 1];
                  strcpy((char *) roundhalftoeven_non_val,
                         OUTPUT_ROUNDHALFTOEVEN_ON_NON_VALUE);
                  cur_val.setStrVal(roundhalftoeven_non_val);
              }
              break;
          }

        case EVAL_SIGNOFF:
            break;

        default:
            throw
                RuntimeException
                ("FunctRoundHalfToEvenExpression: Illegal Evaluation Mode",
                 eid_runtime_illegalmode);
            break;
    }
}

long double FunctRoundHalfToEvenExpression::calculateValue(OutputStream & eos,
                                                           Environment * env,
                                                           unsigned modus,
                                                           bool & non_value) {

    long double roundhalftoeven_ret = 0;

    aggfunct->prepareOperand(eos, env, modus);
    Value *agg_val = aggfunct->getNextValue();

    if (agg_val->getType() == xsd_numeric) {
        non_value = false;
        roundhalftoeven_ret =
            MiscFunctions::getRoundFrom(agg_val->getNumVal(), 0);
    }
    aggfunct->unprepareOperand();

    return roundhalftoeven_ret;
}
