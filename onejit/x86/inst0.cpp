/*
 * onejit - in-memory assembler
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
 * inst0.cpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/opstmt.hpp>
#include <onejit/x86/inst.hpp>
#include <onestl/buffer.hpp>

namespace onejit {
namespace x86 {

using namespace onejit;

static const Inst0 inst0_vec[] = {
    Inst0{"\xf8", EFwrite}, /* clc      clear carry flag                      */
    Inst0{"\xfc", EFwrite}, /* cld      clear direction flag                  */
    Inst0{"\xf5", EFrw},    /* cmc      complement carry flag                 */
    Inst0{"\xcc"},          /* int3     generate a breakpoint trap            */
    Inst0{"\xf0"},          /* lock     lock prefix for following instruction */
    Inst0{"\x90"},          /* nop      no operation                          */
    Inst0{"\xf3\x90"},      /* pause    spin loop hint                        */
    Inst0{"\x9d", EFwrite}, /* popf     pop 8 bytes (4 on 32bit) from stack into EFLAGS */
    Inst0{"\x9c", EFread},  /* pushf    push 8 bytes (4 on 32bit) to stack from EFLAGS */
    Inst0{"\xf3"},          /* rep      repeat prefix for following instruction */
    Inst0{"\xf2"},          /* repne    repeat prefix for following instruction */
    Inst0{"\xc3"},          /* ret      return from function call             */
    Inst0{"\xf9", EFwrite}, /* stc      set carry flag                        */
    Inst0{"\xfd", EFwrite}, /* std      set direction flag                    */
    Inst0{"\x0f\x05"},      /* syscall  fast system call                      */
    Inst0{"\x0f\x34"},      /* sysenter fast system call                      */
    Inst0{"\x0f\x35"},      /* sysexit  fast system call return               */
    Inst0{"\x0f\x07"},      /* sysret   fast system call return               */
    Inst0{"\x0f\x0b"},      /* ud2      undefined instruction, causes SIGILL  */
    Inst0{"\xf2"},          /* xacquire lock elision hint                     */
    Inst0{"\xf3"},          /* xrelease lock elision hint                     */
    ONEJIT_COMMENT()        /* [CPUID SSE2] is required by the following instructions */
    Inst0{"\x0f\xae\xe8"},  /* lfence   serialize memory load operations      */
    Inst0{"\x0f\xae\xf0"},  /* mfence   serialize all memory operations       */
    Inst0{"\x0f\xae\xf8"},  /* sfence   serialize memory store operations     */
    /**/                    /* [CPUID RTM] is required by the following instructions  */
    Inst0{"\x0f\x01\xd5"},  /* xend     end TSX transaction                   */
    /* */                   /* xtest    test if TSX transaction or lock elision is running */
    Inst0{"\x0f\x01\xd6", EFwrite}, /*                                        */
};

const Inst0 &Inst0::find(OpStmt0 op) noexcept {
  if (op < X86_CLC || op > X86_XTEST) {
    op = X86_UD2;
  }
  return inst0_vec[op - X86_CLC];
}

onestl::ByteBuf &Inst0::emit(onestl::ByteBuf &dst) const noexcept {
  return dst.append(bytes());
}

} // namespace x86
} // namespace onejit
