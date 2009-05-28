/*
 | Author: Michael Schmidt
 ===========================================================================
 ===========================================================================
 | Software License Agreement (BSD License)
 | 
 | Copyright (c) 2006-2007, Saarland University Database Group 
 | All rights reserved.
 |
 | Redistribution and use of this software in source and binary forms,
 | with or without modification, are permitted provided that the following
 | conditions are met:
 | 
 | * Redistributions of source code must retain the above
 |   copyright notice, this list of conditions and the
 |  following disclaimer.
 |
 | * Redistributions in binary form must reproduce the above
 |   copyright notice, this list of conditions and the
 |   following disclaimer in the documentation and/or other
 |   materials provided with the distribution.
 |
 | * Neither the name the of Saarland University Database Group nor the names 
 |   of its contributors may be used to endorse or promote products derived 
 |   from this software without specific prior written permission of the
 |   Saarland University Database Group.
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
*/
#ifndef EXPENUMS_H
#define EXPENUMS_H


#ifdef USE_CLOBS
// TODO: Fix this when reusing CLOBS
// typedef enum {e_aux, e_clob, e_dom, e_out } NODE_TYPE;
#else
typedef enum {e_aux, e_dom, e_out } NODE_TYPE;
#endif // ifdef USE_CLOBS

typedef enum {e_child, e_descendant, e_self, e_dos, e_undefined } AXIS_TYPE;

typedef enum {e_lt,e_leq,e_eq,e_gt,e_geq,e_neq} COMP_TYPE;

typedef enum {e_ctypeand, e_ctypeor, e_ctypenot, e_ctypeexists,
                e_ctypentest, e_ctypenntest, e_ctypenctest, 
                e_ctypetrue, e_typeempty, e_typeconst, e_typefor, 
                e_typeif, e_typenodeconstr, e_typepath, 
                e_typepathstep, e_typepathsteptext, e_typepathstepnode,
				e_typesequence, e_typevar, e_typevarstep, e_typeupdate,
				e_typesome } EXP_TYPE;




#define ROOTVAR ".root"

#endif // EXPENUMS_H
