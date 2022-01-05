/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * opstmt1.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPSTMT1_HPP
#define ONEJIT_OPSTMT1_HPP

#include <onejit/fmt.hpp>
#include <onejit/fwd.hpp>

#include <cstdint> // uint16_t

namespace onejit {

enum OpStmt1 : uint16_t {
  BAD_ST1 = 0,
  GOTO = 1,
  INC = 2, // ++
  DEC = 3, // --

// numeric values of the OpStmt1 enum constants below this line MAY CHANGE WITHOUT WARNING

#define ONEJIT_OPSTMT1_MIR(x) /*                                                         */        \
  x(/**/ JMP, jmp)            /* unconditional jump to label                             */

#define ONEJIT_OPSTMT1_X86(x)                                                                      \
  x(/**/ BSWAP, bswap) /* x = byteswap(x) - invert endianity of 32bit or 64bit register  */        \
      x(CALL, call)    /* call function. argument is relative offset, register or memory */        \
      x(CBW, cbw)      /* sign-extend %al -> %ax or %ax -> %eax or  %eax -> %rax */                \
      x(DEC, dec)      /* decrement register or memory by 1 */                                     \
      x(INC, inc)      /* increment register or memory by 1 */                                     \
      x(INT, int)      /* generate a call to interrupt procedure. argument is immediate */         \
      x(JA, ja)        /* jump if above */                                                         \
      x(JAE, jae)      /* jump if above or equal (if not carry) */                                 \
      x(JB, jb)        /* jump if below (if carry) */                                              \
      x(JBE, jbe)      /* jump if below or equal */                                                \
      x(JE, je)        /* jump if equal (if zero) */                                               \
      x(JG, jg)        /* jump if greater */                                                       \
      x(JGE, jge)      /* jump if greater or equal */                                              \
      x(JL, jl)        /* jump if less */                                                          \
      x(JLE, jle)      /* jump if less or equal */                                                 \
      x(JNE, jne)      /* jump if not equal (if not zero) */                                       \
      x(JNO, jno)      /* jump if not overflow */                                                  \
      x(JNP, jnp)      /* jump if not parity (if odd) */                                           \
      x(JNS, jns)      /* jump if not sign */                                                      \
      x(JO, jo)        /* jump if overflow */                                                      \
      x(JP, jp)        /* jump if parity (if even) */                                              \
      x(JS, js)        /* jump if sign */                                                          \
      x(JMP, jmp)      /* unconditional jump. argument is relative offset, register or memory */   \
      x(NEG, neg)      /* negate (i.e. -x) register or memory */                                   \
      x(NOT, not)      /* invert (i.e. ^x) register or memory */                                   \
      x(POP, pop)      /* pop 2 or 8 bytes from stack into register or memory */                   \
      x(PUSH, push)    /* push 2 or 8 bytes to stack from register or memory */                    \
      x(RDTSC, rdtsc)  /* read timestamp counter into %rdx:%rax */                                 \
      x(SETA, seta)    /* set 1 byte register or memory if above */                                \
      x(SETAE, setae)  /* set 1 byte register or memory if above or equal (if not carry) */        \
      x(SETB, setb)    /* set 1 byte register or memory if below (if carry) */                     \
      x(SETBE, setbe)  /* set 1 byte register or memory if below or equal */                       \
      x(SETE, sete)    /* set 1 byte register or memory if equal (if zero) */                      \
      x(SETG, setg)    /* set 1 byte register or memory if greater */                              \
      x(SETGE, setge)  /* set 1 byte register or memory if greater or equal */                     \
      x(SETL, setl)    /* set 1 byte register or memory if less */                                 \
      x(SETLE, setle)  /* set 1 byte register or memory if less or equal */                        \
      x(SETNE, setne)  /* set 1 byte register or memory if not equal (if not zero) */              \
      x(SETNO, setno)  /* set 1 byte register or memory if not overflow */                         \
      x(SETNP, setnp)  /* set 1 byte register or memory if not parity (if odd) */                  \
      x(SETNS, setns)  /* set 1 byte register or memory if not sign */                             \
      x(SETO, seto)    /* set 1 byte register or memory if overflow */                             \
      x(SETP, setp)    /* set 1 byte register or memory if parity (if even) */                     \
      x(SETS, sets)    /* set 1 byte register or memory if sign */                                 \
      ONEJIT_COMMENT() /* [CPUID CLFSH] is required by the following instructions ------------- */ \
      x(CLFLUSH, clflush) /* clear all caches containing memory address. */                        \
      ONEJIT_COMMENT() /* [CPUID CLFLUSHOPT] is required by the following instructions -------- */ \
      x(CLFLUSHOPT, clflushopt) /* clear all caches containing memory address. */                  \
      ONEJIT_COMMENT()  /* [CPUID CLWB] is required by the following instructions ------------- */ \
      x(CLWB, clwb)     /* write-back cache line containing memory address. */                     \
      ONEJIT_COMMENT()  /* [CPUID RTM] is required by the following instructions -------------- */ \
      x(XABORT, xabort) /* abort TSX transaction with 1 byte immediate value */

#define ONEJIT_X(NAME, name) MIR_##NAME,
  ONEJIT_OPSTMT1_MIR(ONEJIT_X)
#undef ONEJIT_X

#define ONEJIT_X(NAME, name) X86_##NAME,
      ONEJIT_OPSTMT1_X86(ONEJIT_X)
#undef ONEJIT_X

          X86_CWDE = X86_CBW, /* sign-extend  %ax -> %eax */
  X86_CDQE = X86_CBW,         /* sign-extend %eax -> %rax */

};

constexpr OpStmt1 operator+(OpStmt1 op, int delta) noexcept {
  return OpStmt1(int(op) + delta);
}
constexpr OpStmt1 operator-(OpStmt1 op, int delta) noexcept {
  return OpStmt1(int(op) - delta);
}

const Chars to_string(OpStmt1 op) noexcept;

const Fmt &operator<<(const Fmt &fmt, OpStmt1 op);

} // namespace onejit

#endif // ONEJIT_OPSTMT_HPP
