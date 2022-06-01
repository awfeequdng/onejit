/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * opstmt0.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPSTMT0_HPP
#define ONEJIT_OPSTMT0_HPP

#include <onejit/fmt.hpp>
#include <onejit/fwd.hpp>

#include <cstdint> // uint16_t

namespace onejit {

enum OpStmt0 : uint16_t {
  BAD = 0,
  BREAK = 1,       // only allowed in input IR - never used in compiled code
  CONTINUE = 2,    // idem
  FALLTHROUGH = 3, // idem

// numeric values of the OpStmt0 enum constants below this line MAY CHANGE WITHOUT WARNING

#define ONEJIT_OPSTMT0_X86(x)                                                                      \
  x(CLC, clc)               /* clear carry flag           */                                       \
      x(CLD, cld)           /* clear direction flag       */                                       \
      x(CMC, cmc)           /* complement carry flag      */                                       \
      x(INT3, int3)         /* generate a breakpoint trap */                                       \
      x(LOCK, lock)         /* lock prefix for following instruction */                            \
      x(NOP, nop)           /* no operation               */                                       \
      x(PAUSE, pause)       /* spin loop hint             */                                       \
      x(POPF, popf)         /* pop 8 bytes (4 on 32bit) from stack into EFLAGS */                  \
      x(PUSHF, pushf)       /* push 8 bytes (4 on 32bit) to stack from EFLAGS */                   \
      x(REP, rep)           /* repeat prefix for following instruction */                          \
      x(REPNE, repne)       /* repeat prefix for following instruction */                          \
      /*x(RET, ret)*/       /* return from function call. not here, see ONEJIT_OPSTMTN_X86 */      \
      x(STC, stc)           /* set carry flag             */                                       \
      x(STD, std)           /* set direction flag         */                                       \
      x(SYSCALL, syscall)   /* fast system call           */                                       \
      x(SYSENTER, sysenter) /* fast system call           */                                       \
      x(SYSEXIT, sysexit)   /* fast system call return    */                                       \
      x(SYSRET, sysret)     /* fast system call return    */                                       \
      x(UD2, ud2)           /* undefined instruction, intentionally causes SIGILL */               \
      x(XACQUIRE, xacquire) /* lock elision hint          */                                       \
      x(XRELEASE, xrelease) /* lock elision hint          */                                       \
      ONEJIT_COMMENT() /* --------------------------------------------------------------------- */ \
      ONEJIT_COMMENT() /* [CPUID SSE2] is required by the following instructions -------------- */ \
      ONEJIT_COMMENT() /* --------------------------------------------------------------------- */ \
      x(LFENCE, lfence) /* serialize memory load operations  */                                    \
      x(MFENCE, mfence) /* serialize all memory operations   */                                    \
      x(SFENCE, sfence) /* serialize memory store operations */                                    \
      ONEJIT_COMMENT() /* --------------------------------------------------------------------- */ \
      ONEJIT_COMMENT() /* [CPUID RTM] is required by the following instructions --------------- */ \
      ONEJIT_COMMENT() /* --------------------------------------------------------------------- */ \
      x(XEND, xend)    /* end TSX transaction */                                                   \
      x(XTEST, xtest)  /* if TSX transaction or lock elision is in progress, clear ZF flag.        \
                          otherwise set ZF */

#define ONEJIT_X(NAME, name) X86_##NAME,
  ONEJIT_OPSTMT0_X86(ONEJIT_X)
#undef ONEJIT_X
};

constexpr inline OpStmt0 operator+(OpStmt0 op, int delta) noexcept {
  return OpStmt0(int(op) + delta);
}
constexpr inline OpStmt0 operator-(OpStmt0 op, int delta) noexcept {
  return OpStmt0(int(op) - delta);
}

const Chars to_string(OpStmt0 op) noexcept;

const Fmt &operator<<(const Fmt &fmt, OpStmt0 op);

} // namespace onejit

#endif // ONEJIT_OPSTMT0_HPP
