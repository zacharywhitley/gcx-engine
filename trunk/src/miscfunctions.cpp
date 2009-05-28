
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
 *  @brief Implementations of header file miscfunctions.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file miscfunctions.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "miscfunctions.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

//! @namespace std
using namespace std;

long double MiscFunctions::getNumericFromString(const char *str) {
    bool ws_only = true;

    for (unsigned i = 0; i < strlen(str); i++) {
        if (!isspace(str[i])) {
            ws_only = false;
            break;
        }
    }
    if (ws_only) {
        throw
            InvalidCastException
            ("Method \"getNumericFromString()\" Cannot Convert Whitespaces To A Number",
             eid_cast_stringnumeric);
    }

    char *nstr = (char *) str;

    if (REPLACE_DECIMAL_POINT_BEFORE) {
        while (char *pComma = strchr(nstr, ',')) {
            *pComma = '.';
        }
    }

    errno = 0;
    char *pEnd;
    long double result = strtold(nstr, &pEnd);

    if (isnan(result)) {
        // throw InvalidCastException("Result From Method \"getNumericFromString()\" Is Not A Number (NaN)", eid_cast_stringnumeric);
    } else if (isinf(result)) {
        // throw InvalidCastException("Result From Method \"getNumericFromString()\" Is Infinity", eid_cast_stringnumeric);
    } else if (result == HUGE_VALL) {
        throw
            InvalidCastException
            ("Result From Method \"getNumericFromString()\" Is Too Large (HUGE_VALL)",
             eid_cast_stringnumeric);
    } else if (result == -HUGE_VALL) {
        throw
            InvalidCastException
            ("Result From Method \"getNumericFromString()\" Is Too Small (-HUGE_VALL)",
             eid_cast_stringnumeric);
    } else if (errno == ERANGE) {
        throw
            InvalidCastException
            ("Result From Method \"getNumericFromString()\" Is Either Too Large (HUGE_VALL) Or Too Small (-HUGE_VALL)",
             eid_cast_stringnumeric);
    } else if (errno) {
        throw
            InvalidCastException
            ("Result From Method \"getNumericFromString()\" Is Wrong Due To An Unknown Error",
             eid_cast_stringnumeric);
    } else if (*pEnd && !isdigit(*pEnd)) {
        while (*pEnd) {
            if (!isspace(*pEnd)) {
                throw
                    InvalidCastException
                    ("Result From Method \"getNumericFromString()\" Still Contains Not Castable Characters",
                     eid_cast_stringnumeric);
            }
            pEnd++;
        }
    }

    return result;
}

const char *MiscFunctions::getStringFromNumerical(long double num) {
    ostringstream o;

    if (!(o << num)) {
        throw
            InvalidCastException
            ("Result From Method \"getStringFromNumerical()\" Is Wrong Due To Bad Conversion",
             eid_cast_numericstring);
    }

    size_t str_len = strlen(o.str().c_str()) + 1;
    char *cpy = new char[str_len];

    strcpy(cpy, o.str().c_str());
    cpy[str_len - 1] = '\0';

    if (REPLACE_DECIMAL_POINT_AFTER) {
        while (char *pComma = strchr(cpy, '.')) {
            *pComma = ',';
        }
    }

    return cpy;
}

long double MiscFunctions::getSummationFrom(long double left, long double right) {
    errno = 0;
    long double sum = (left + right);

    if (isnan(sum)) {
        // throw InvalidCastException("Result From Method \"getSummationFrom()\" Is Not A Number (NaN)", eid_cast_summation);
    } else if (isinf(sum)) {
        // throw InvalidCastException("Result From Method \"getSummationFrom()\" Is Infinity", eid_cast_summation);
    } else if (sum == HUGE_VALL) {
        throw
            InvalidCastException
            ("Result From Method \"getSummationFrom()\" Is Too Large (HUGE_VALL)",
             eid_cast_summation);
    } else if (sum == -HUGE_VALL) {
        throw
            InvalidCastException
            ("Result From Method \"getSummationFrom()\" Is Too Small (-HUGE_VALL)",
             eid_cast_summation);
    } else if (errno == ERANGE) {
        throw
            InvalidCastException
            ("Result From Method \"getSummationFrom()\" Is Either Too Large (HUGE_VALL) Or Too Small (-HUGE_VALL)",
             eid_cast_summation);
    } else if (errno) {
        throw
            InvalidCastException
            ("Result From Method \"getSummationFrom()\" Is Wrong Due To An Unknown Error",
             eid_cast_summation);
    }

    return sum;
}

long double MiscFunctions::getSubtractionFrom(long double left,
                                              long double right) {
    return getSummationFrom(left, -right);
}

long double MiscFunctions::getPowerFrom(long double base, long double exponent) {
    errno = 0;
    long double power = pow(base, exponent);

    if (isnan(power)) {
        // throw InvalidCastException("Result From Method \"getPowerFrom()\" Is Not A Number (NaN)", eid_cast_power);
    } else if (isinf(power)) {
        // throw InvalidCastException("Result From Method getPowerFrom()\" Is Infinity", eid_cast_power);
    } else if (power == HUGE_VALL) {
        throw
            InvalidCastException
            ("Result From Method \"getPowerFrom()\" Is Too Large (HUGE_VALL)",
             eid_cast_power);
    } else if (power == -HUGE_VALL) {
        throw
            InvalidCastException
            ("Result From Method \"getPowerFrom()\" Is Too Small (-HUGE_VALL)",
             eid_cast_power);
    } else if (errno == ERANGE) {
        throw
            InvalidCastException
            ("Result From Method \"getPowerFrom()\" Is Either Too Large (HUGE_VALL) Or Too Small (-HUGE_VALL)",
             eid_cast_power);
    } else if (errno == EDOM) {
        throw
            InvalidCastException
            ("Result From Method \"getPowerFrom()\" Is Wrong Due To An \"Domain Error\"",
             eid_cast_power);
    } else if (errno) {
        throw
            InvalidCastException
            ("Result From Method \"getPowerFrom()\" Is Wrong Due To An Unknown Error",
             eid_cast_power);
    }

    return power;
}

long double MiscFunctions::getRadicalFrom(long double base, long double radix) {
    if (base < (long double) 0) {
        throw
            InvalidCastException
            ("Result From Method \"getRadicalFrom()\" Is Wrong Due To Negative Base",
             eid_cast_radical);
    } else if (radix == (long double) 0) {
        throw
            InvalidCastException
            ("Result From Method \"getRadicalFrom()\" Is Wrong Due To Division By Zero",
             eid_cast_radical);
    }

    return getPowerFrom(base, (1 / radix));
}

long double MiscFunctions::getDivisionFrom(long double numerator,
                                           long double denominator) {
    if (denominator == (long double) 0) {
        throw
            InvalidCastException
            ("Result From Method \"getDivisionFrom()\" Is Wrong Due To Division By Zero",
             eid_cast_division);
    }

    errno = 0;
    long double fraction = (numerator / denominator);

    if (isnan(fraction)) {
        // throw InvalidCastException("Result From Method \"getDivisionFrom()\" Is Not A Number (NaN)", eid_cast_division);
    } else if (isinf(fraction)) {
        // throw InvalidCastException("Result From Method \"getDivisionFrom()\" Is Infinity", eid_cast_division);
    } else if (fraction == HUGE_VALL) {
        throw
            InvalidCastException
            ("Result From Method \"getDivisionFrom()\" Is Too Large (HUGE_VALL)",
             eid_cast_division);
    } else if (fraction == -HUGE_VALL) {
        throw
            InvalidCastException
            ("Result From Method \"getDivisionFrom()\" Is Too Small (-HUGE_VALL)",
             eid_cast_division);
    } else if (errno == ERANGE) {
        throw
            InvalidCastException
            ("Result From Method \"getDivisionFrom()\" Is Either Too Large (HUGE_VALL) Or Too Small (-HUGE_VALL)",
             eid_cast_division);
    } else if (errno) {
        throw
            InvalidCastException
            ("Result From Method \"getDivisionFrom()\" Is Wrong Due To An Unknown Error",
             eid_cast_division);
    }

    return fraction;
}

long double MiscFunctions::getRoundFrom(long double num, unsigned places) {
    double v[] = { 1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9,
        1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21,
        1e22
    };

    return (floor(num * v[places] + 0.5) / v[places]);
}

bool MiscFunctions::needsTrim(const char *s) {
    unsigned l = strlen(s);

    return (l > 0 && (isspace(s[0]) || isspace(s[l - 1])));
}

const char *MiscFunctions::trim(const char *s) {

    int first_nonws = 0;
    int last_nonws = strlen(s) + 1;

    // calculate offsets
    bool search_finished = false;

    for (int i = 0; i < (int) strlen(s) - 1 && !search_finished; i++) {
        if (isspace(s[i])) {
            first_nonws = i;
            search_finished = true;
        }
    }

    search_finished = false;
    for (int i = strlen(s) - 1; i >= 0 && !search_finished; i--) {
        if (isspace(s[i])) {
            last_nonws = i;
            search_finished = true;
        }
    }

    char *ret = NULL;

    if (last_nonws < first_nonws || last_nonws == (int) strlen(s) + 1) {
        ret = new char[1];      // empty string

        ret[0] = '\0';
    } else {
        ret =
            new char[(last_nonws - first_nonws) * sizeof (char) +
                     2 * sizeof (char)];
        strncpy(ret, s + first_nonws,
                (last_nonws - first_nonws + sizeof (char)));
    }

    // copy and return string
    return ret;
}
