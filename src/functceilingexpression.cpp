
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
 *  @brief Implementations of header file functceilingexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file functceilingexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "functceilingexpression.h"

FunctCeilingExpression::FunctCeilingExpression(AggregateFunctExpression * _aggfunct):
RoundingExpression(et_ceiling, _aggfunct) {
}

FunctCeilingExpression::~FunctCeilingExpression() {
}

void FunctCeilingExpression::eval(OutputStream & eos, Environment * env,
                                  unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
          {
              bool non_value = true;
              long double ceiling_val =
                  calculateValue(eos, env, modus, non_value);
              if (!non_value) {
                  const char *ceiling_str =
                      MiscFunctions::getStringFromNumerical(ceiling_val);
                  eos << ceiling_str;
                  delete[]ceiling_str;
              } else {
                  eos << OUTPUT_CEILING_ON_NON_VALUE;
              }
              break;
          }

        case EVAL_QUERY_SILENT:
          {
              bool non_value = true;
              long double ceiling_val =
                  calculateValue(eos, env, modus, non_value);
              if (!non_value) {
                  cur_val.setNumVal(ceiling_val);
              } else {
                  const char *ceiling_non_val =
                      new char[strlen(OUTPUT_CEILING_ON_NON_VALUE) + 1];
                  strcpy((char *) ceiling_non_val, OUTPUT_CEILING_ON_NON_VALUE);
                  cur_val.setStrVal(ceiling_non_val);
              }
              break;
          }

        case EVAL_SIGNOFF:
            break;

        default:
            throw
                RuntimeException
                ("FunctCeilingExpression: Illegal Evaluation Mode",
                 eid_runtime_illegalmode);
            break;
    }
}

long double FunctCeilingExpression::calculateValue(OutputStream & eos,
                                                   Environment * env,
                                                   unsigned modus,
                                                   bool & non_value) {

    long double ceiling_ret = 0;

    aggfunct->prepareOperand(eos, env, modus);
    Value *agg_val = aggfunct->getNextValue();

    if (agg_val->getType() == xsd_numeric) {
        non_value = false;
        ceiling_ret = ceil(agg_val->getNumVal());
    }
    aggfunct->unprepareOperand();

    return ceiling_ret;
}
