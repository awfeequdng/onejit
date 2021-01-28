/*
 * oneasm - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * inst1.cpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */

#include <oneasm/x86/inst.hpp>
#include <onejit/opstmt.hpp>

namespace oneasm {
namespace x86 {

using namespace onejit;

static const Inst1 inst1[] = {
    Inst1{Arg1(0), B0},                                 /* bad instruction                    */
    Inst1{Arg1::Reg, B32 | B64},                        /* bswap                */
    Inst1{Arg1::Reg | Arg1::Mem | Arg1::Val, B64, B32}, /* call                 */
    Inst1{Arg1::Rax, B16 | B32 | B64},                  /* cbw                  */
    Inst1{Arg1::Val, B0, B8},                           /* int                  */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* ja                   */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* jae                  */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* jb                   */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* jbe                  */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* je                   */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* jge                  */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* jl                   */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* jle                  */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* jne                  */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* jno                  */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* jnp                  */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* jns                  */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* jo                   */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* jp                   */
    Inst1{Arg1::Val, B0, B8 | B32, EFread},             /* js                   */
    Inst1{Arg1::Reg | Arg1::Mem | Arg1::Val, B64, B8 | B32},         /* jmp     */
    Inst1{Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite}, /* neg     */
    Inst1{Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite}, /* not     */
    Inst1{Arg1::Reg | Arg1::Mem, B16 | B64},      /* B32 on 32bit       pop     */
    Inst1{Arg1::Reg | Arg1::Mem, B16 | B64},      /* B32 on 32bit       push    */
    Inst1{Arg1::Rax, B128},                       /* B64 on 32bit       rdtsc   */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       seta                 */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setae                */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setb                 */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setbe                */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       sete                 */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setg                 */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setge                */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setl                 */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setle                */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setne                */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setno                */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setnp                */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setns                */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       seto                 */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setp                 */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       sets                 */
    Inst1{Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       sets                 */
    ONEJIT_COMMENT()          /* [CPUID CLFSH] is required by the following instructions */
    Inst1{Arg1::Mem, B8},     /*                           clflush              */
    ONEJIT_COMMENT()          /* [CPUID CLFLUSHOPT] is required by the following instructions */
    Inst1{Arg1::Mem, B8},     /*                           clflushopt           */
    ONEJIT_COMMENT()          /* [CPUID CLWB] is required by the following instructions */
    Inst1{Arg1::Mem, B8},     /*                           clwb                 */
    ONEJIT_COMMENT()          /* [CPUID RTM] is required by the following instructions */
    Inst1{Arg1::Val, B0, B8}, /*                           xabort               */
};

const Inst1 &to_inst(OpStmt1 op) {
  if (op < X86_BSWAP || op > X86_XABORT) {
    op = OpStmt1(X86_BSWAP - 1);
  }
  return inst1[op - X86_BSWAP + 1];
}

} // namespace x86
} // namespace oneasm
