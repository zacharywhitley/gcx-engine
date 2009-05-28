
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
 *  @brief Implementations needed for header file stringdefs.h.
 *  @details Implementations needed for header file stringdefs.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include <iostream>
#include <cstdlib>

/*! @fn int main(int argc, char** argv)
 *  @brief MAIN.
 *  @details MAIN.
 *  @param[in] argc Number of entered application call parameters.
 *  @param[in] argv All entered application call parameters.
 *  @retval int <tt>EXIT_FAILURE or 1</tt> in case of an error, 
 *    <tt>EXIT_SUCCESS or 0</tt> if application terminated with success. 
 */
int main(int argc, char **argv) {

    if (argc < 2)
        return 1;

    FILE *f = fopen(argv[1], "rb");

    fseek(f, 0, SEEK_END);
    long pos = ftell(f);

    fseek(f, 0, SEEK_SET);

    char *bytes = (char *) malloc(pos);

    fread(bytes, pos, 1, f);
    fclose(f);

    char backslash = '\\';
    char n = 'n';
    char linebreak = '\n';
    char quote = '"';

    FILE *f2 = fopen(argv[2], "w");

    fwrite(&quote, 1, 1, f2);

    bool backslashed = false;

    for (int i = 0; i < pos; i++) {

        if (bytes[i] != '"' || backslashed) {
            if (bytes[i] != '\n') {
                fwrite(bytes + i, 1, 1, f2);
            } else {
                fwrite(&backslash, 1, 1, f2);
                fwrite(&n, 1, 1, f2);
                fwrite(&quote, 1, 1, f2);
                fwrite(&linebreak, 1, 1, f2);
                fwrite(&quote, 1, 1, f2);
            }
        }
        // cover single backslashing
        backslashed = bytes[i] == '\\';
    }
    fwrite(&quote, 1, 1, f2);
    fwrite(&linebreak, 1, 1, f2);
    fclose(f2);

    free(bytes);                // free allocated memory
}
