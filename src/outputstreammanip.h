
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
 *  @brief Header file for outputstreammanip.cpp.
 *  @details Header file specifying constructors, destructor and functions for outputstreammanip.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef OUTPUTSTREAMMANIP_H
#define OUTPUTSTREAMMANIP_H

#include "outputstream.h"

/*! @class DecIndents
 *  @brief Represents helper class for manipulating (decrementing) indents in all output streams.
 *  @details Represents helper class for manipulating (decrementing) indents in all output streams.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class DecIndents {
  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a number of indents to be decremented.
     *  @param[in] _n Number of indents to be decremented.
     */
  DecIndents(int _n = INDENT_SINGLE):n(_n) {
    }
    /*! @fn void operator()(OutputStream& out) const
     *  @brief Operator <tt>()</tt> overloading.
     *  @details Operator <tt>()</tt> overloading.
     *  @param[in] out Pointer to a OutputStream object.
     *  @retval void
     */ void operator     () (OutputStream & out) const {
        out.decrementIndents(n);
  } private:
    /*! @var int n
     *  @brief The entered number of indents to be decremented.
     *  @details The entered number of indents to be decremented,
     *                      which is the argument of the constructor.
     */
    int n;
};

/*! @fn inline DecIndents decrementIndents(int n)
 *  @brief Decrements the number of indents in all output streams.
 *  @details Decrements the number of indents in all output streams.
 *  @param[in] n Number of indents to be decremented.
 *  @retval DecIndents DecIndents object.
 */
inline DecIndents decrementIndents(int n = INDENT_SINGLE) {
    return DecIndents(n);
}

/*! @fn inline OutputStream& operator<<(OutputStream& out, const DecIndents& dec)
 *  @brief Operator <tt>\<\<</tt> overloading for decrementing indents.
 *  @details Operator <tt>\<\<</tt> overloading for decrementing indents.
 *  @param[in] out Pointer to a OutputStream object.
 *  @param[in] dec Pointer to a DecIndents object.
 *  @retval OutputStream Pointer to a OutputStream object.
 */
inline OutputStream & operator<<(OutputStream & out, const DecIndents & dec) {
    dec(out);
    return (out);
}

/*! @class IncIndents
 *  @brief Represents helper class for manipulating (incrementing) indents in all output streams.
 *  @details Represents helper class for manipulating (incrementing) indents in all output streams.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class IncIndents {
  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a number of indents to be incremented.
     *  @param[in] _n Number of indents to be incremented.
     */
  IncIndents(int _n = INDENT_SINGLE):n(_n) {
    }
    /*! @fn void operator()(OutputStream& out) const
     *  @brief Operator <tt>()</tt> overloading.
     *  @details Operator <tt>()</tt> overloading.
     *  @param[in] out Pointer to a OutputStream object.
     *  @retval void
     */ void operator     () (OutputStream & out) const {
        out.incrementIndents(n);
  } private:
    /*! @var int n
     *  @brief The entered number of indents to be incremented.
     *  @details The entered number of indents to be incremented,
     *                      which is the argument of the constructor.
     */
    int n;
};

/*! @fn inline IncIndents incrementIndents(int n)
 *  @brief Increments the number of indents in all output streams.
 *  @details Increments the number of indents in all output streams.
 *  @param[in] n Number of indents to be incremented.
 *  @retval IncIndents IncIndents object.
 */
inline IncIndents incrementIndents(int n = INDENT_SINGLE) {
    return IncIndents(n);
}

/*! @fn inline OutputStream & operator<<(OutputStream& out, const IncIndents& inc)
 *  @brief Operator <tt>\<\<</tt> overloading for incrementing indents.
 *  @details Operator <tt>\<\<</tt> overloading for incrementing indents.
 *  @param[in] out Pointer to a OutputStream object.
 *  @param[in] inc Pointer to a IncIndents object.
 *  @retval OutputStream Pointer to a OutputStream object.
 */
inline OutputStream & operator<<(OutputStream & out, const IncIndents & inc) {
    inc(out);
    return (out);
}

/*! @class ResIndents
 *  @brief Represents helper class for manipulating (resetting) indents in all output streams.
 *  @details Represents helper class for manipulating (resetting) indents in all output streams.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class ResIndents {
  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    ResIndents() {
    }
    /*! @fn void operator()(OutputStream& out) const
     *  @brief Operator <tt>()</tt> overloading.
     *  @details Operator <tt>()</tt> overloading.
     *  @param[in] out Pointer to a OutputStream object.
     *  @retval void
     */ void operator     () (OutputStream & out) const {
        out.resetIndents();
}};

/*! @fn inline ResIndents resetIndents()
 *  @brief Resets the number of indents in all output streams.
 *  @details Resets the number of indents in all output streams.
 *  @retval ResIndents ResIndents object.
 */
inline ResIndents resetIndents() {
    return ResIndents();
}

/*! @fn inline OutputStream& operator<<(OutputStream& out, const ResIndents& res)
 *  @brief Operator <tt>\<\<</tt> overloading for resetting indents.
 *  @details Operator <tt>\<\<</tt> overloading for resetting indents.
 *  @param[in] out Pointer to a OutputStream object.
 *  @param[in] res Pointer to a ResIndents object.
 *  @retval OutputStream Pointer to a OutputStream object.
 */
inline OutputStream & operator<<(OutputStream & out, const ResIndents & res) {
    res(out);
    return (out);
}

/*! @class WriIndents
 *  @brief Represents helper class for writing indents in all output streams.
 *  @details Represents helper class for writing indents in all output streams.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class WriIndents {
  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    WriIndents() {
    }
    /*! @fn void operator()(OutputStream& out) const
     *  @brief Operator <tt>()</tt> overloading.
     *  @details Operator <tt>()</tt> overloading.
     *  @param[in] out Pointer to a OutputStream object.
     *  @retval void
     */ void operator     () (OutputStream & out) const {
        out.writeIndents();
}};

/*! @fn inline WriIndents writeIndents()
 *  @brief Writes indents in all output streams.
 *  @details Writes indents in all output streams.
 *  @retval WriIndents WriIndents object.
 */
inline WriIndents writeIndents() {
    return WriIndents();
}

/*! @fn inline OutputStream& operator<<(OutputStream& out, const WriIndents& write)
 *  @brief Operator <tt>\<\<</tt> overloading for writing indents.
 *  @details Operator <tt>\<\<</tt> overloading for writing indents.
 *  @param[in] out Pointer to a OutputStream object.
 *  @param[in] write Pointer to a WriIndents object.
 *  @retval OutputStream Pointer to a OutputStream object.
 */
inline OutputStream & operator<<(OutputStream & out, const WriIndents & write) {
    write(out);
    return (out);
}

#endif // OUTPUTSTREAMMANIP_H
