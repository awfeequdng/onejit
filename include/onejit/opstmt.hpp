/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
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
 * opstmt.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPSTMT_HPP
#define ONEJIT_OPSTMT_HPP

#include <onejit/fmt.hpp>
#include <onejit/fwd.hpp>

#include <cstdint> // uint8_t

namespace onejit {

#define ONEJIT_COMMENT(...)

enum OpStmt0 : uint16_t {
  BAD = 0,
  BREAK = 1,
  CONTINUE = 2,
  FALLTHROUGH = 3,

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

enum OpStmt1 : uint16_t {
  BAD_ST1 = 0,
  GOTO = 1,
  INC = 2, // ++
  DEC = 3, // --

// numeric values of the OpStmt1 enum constants below this line MAY CHANGE WITHOUT WARNING

#define ONEJIT_OPSTMT1_ASM(x)                                                                      \
  x(JA, ja)       /* jump if above */                                                              \
      x(JAE, jae) /* jump if above or equal */                                                     \
      x(JB, jb)   /* jump if below */                                                              \
      x(JBE, jbe) /* jump if below or equal */                                                     \
      x(JE, je)   /* jump if equal (if zero) */                                                    \
      x(JG, jg)   /* jump if greater */                                                            \
      x(JGE, jge) /* jump if greater or equal */                                                   \
      x(JL, jl)   /* jump if less */                                                               \
      x(JLE, jle) /* jump if less or equal */                                                      \
      x(JNE, jne) /* jump if not equal (if not zero) */

#define ONEJIT_OPSTMT1_X86(x)                                                                      \
  x(BSWAP, bswap)      /* x = byteswap(x) - invert endianity of 32bit or 64bit register */         \
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

#define ONEJIT_X(NAME, name) ASM_##NAME,
  ONEJIT_OPSTMT1_ASM(ONEJIT_X)
#undef ONEJIT_X

#define ONEJIT_X(NAME, name) X86_##NAME,
      ONEJIT_OPSTMT1_X86(ONEJIT_X)
#undef ONEJIT_X

          X86_CWDE = X86_CBW, /* sign-extend  %ax -> %eax */
  X86_CDQE = X86_CBW,         /* sign-extend %eax -> %rax */

};

enum OpStmt2 : uint16_t {
  BAD_ST2 = 0,
  // intentionally same values as Op2
  ADD_ASSIGN, // +=
  SUB_ASSIGN, // -=
  MUL_ASSIGN, // *=
  QUO_ASSIGN, // /=
  REM_ASSIGN, // %=
  AND_ASSIGN, // &=
  OR_ASSIGN,  // |=
  XOR_ASSIGN, // ^=
  SHL_ASSIGN, // <<=
  SHR_ASSIGN, // >>=
  ASSIGN,     // =

  CASE,
  DEFAULT,
  JUMP_IF,

// numeric values of the OpStmt2 enum constants below this line MAY CHANGE WITHOUT WARNING

#define ONEJIT_OPSTMT2_ASM(x) x(CMP, cmp) /* compare arguments, set architectural flags */

#define ONEJIT_OPSTMT2_X86(x)                                                                      \
  x(/**/ ADD, add)              /* x += y on register or memory */                                 \
      x(ADC, adc)               /* x += y + carry on register or memory */                         \
      x(AND, and)               /* bitwise AND (i.e. x&y) register or memory */                    \
      x(BSF, bsf)               /* x = bitscan_forward(y) */                                       \
      x(BSR, bsr)               /* x = bitscan_reverse(y) */                                       \
      x(BT, bt)                 /* bit test */                                                     \
      x(BTC, btc)               /* bit test and complement */                                      \
      x(BTR, btr)               /* bit test and reset */                                           \
      x(BTS, bts)               /* bit test and set */                                             \
      x(CMOVA, cmova)           /* conditional move if above */                                    \
      x(CMOVAE, cmovae)         /* conditional move if above or equal (if not carry) */            \
      x(CMOVB, cmovb)           /* conditional move if below (if carry) */                         \
      x(CMOVBE, cmovbe)         /* conditional move if below or equal */                           \
      x(CMOVE, cmove)           /* conditional move if equal (if zero) */                          \
      x(CMOVG, cmovg)           /* conditional move if greater */                                  \
      x(CMOVGE, cmovge)         /* conditional move if greater or equal */                         \
      x(CMOVL, cmovl)           /* conditional move if less */                                     \
      x(CMOVLE, cmovle)         /* conditional move if less or equal */                            \
      x(CMOVNE, cmovne)         /* conditional move if not equal (if not zero) */                  \
      x(CMOVNO, cmovno)         /* conditional move if not overflow */                             \
      x(CMOVNP, cmovnp)         /* conditional move if not parity (if odd) */                      \
      x(CMOVNS, cmovns)         /* conditional move if not sign */                                 \
      x(CMOVO, cmovo)           /* conditional move if overflow */                                 \
      x(CMOVP, cmovp)           /* conditional move if parity (if even) */                         \
      x(CMOVS, cmovs)           /* conditional move if sign */                                     \
      x(CMP, cmp)               /* compare two arguments, set EFLAGS */                            \
      x(CMPXCHG, cmpxchg)       /* compare and exchange 1, 2, 4 or 8 bytes */                      \
      x(CMPXCHG8B, cmpxchg8b)   /* compare and exchange 8 bytes */                                 \
      x(CMPXCHG16B, cmpxchg16b) /* compare and exchange 16 bytes */                                \
      x(DIV, div)               /* unsigned divide %rdx:%rax by argument */                        \
      x(IDIV, idiv)             /* signed divide %rdx:%rax by argument */                          \
      x(IMUL, imul)             /* signed multiply %rdx:%rax by argument */                        \
      x(LEA, lea)               /* load effective address */                                       \
      x(LODS, lods)             /* load string from %rsi into %al/%ax/%eax/%rax */                 \
      x(MOV, mov)               /* general purpose move register, memory or immediate */           \
      x(MOVNTI, movnti) /* store 4 or 8 bytes register to memory, with non-temporal hint */        \
      x(MOVS, movs)   /* move 1,2,4 or 8 bytes from mem at %rsi to mem at %rdi, and update both */ \
      x(MOVSX, movsx) /* sign-extend register or memory */                                         \
      x(MOVZX, movzx) /* zero-extend register or memory */                                         \
      x(MUL, mul)     /* unsigned multiply %rdx:%rax by argument */                                \
      x(OR, or)       /* bitwise OR (i.e. x|y) register or memory */                               \
      x(RCL, rcl)     /* rotate left 1,2,4 or 8 bytes + carry by specified # bits */               \
      x(RCR, rcr)     /* rotate right 1,2,4 or 8 bytes + carry by specified # bits */              \
      x(ROL, rol)     /* rotate left 1,2,4 or 8 bytes by specified # bits */                       \
      x(ROR, ror)     /* rotate right 1,2,4 or 8 bytes by specified # bits */                      \
      x(SAR, sar)     /* right shift register or memory by specified # bits, sign fill */          \
      x(SHL, shl)     /* left shift register or memory by specified # bits */                      \
      x(SHR, shr)     /* right shift register or memory by specified # bits, zero fill */          \
      x(SBB, sbb)     /* subtract with borrow, register or memory */                               \
      x(SUB, sub)     /* subtract register or memory */                                            \
      x(TEST, test)   /* bitwise AND register or memory, discard result, set EFLAGS */             \
      x(XADD, xadd)   /* exchange and add register or memory */                                    \
      x(XCHG, xchg)   /* exchange register or memory */                                            \
      x(XOR, xor)     /* bitwise XOR (i.e. x^y) register or memory */                              \
      ONEJIT_COMMENT()    /* ------------------------------------------------------------------ */ \
      ONEJIT_COMMENT()    /* [CPUID SSE] is required by the following instructions ------------ */ \
      ONEJIT_COMMENT()    /* ------------------------------------------------------------------ */ \
      x(MOVHLPD, movhlpd) /* move high 8 bytes to low 8 bytes of packed float */                   \
      x(MOVHPD, movhpd)   /* move high 8 bytes to high 8 bytes of packed double */                 \
      x(MOVLHPD, movlhpd) /* move low 8 bytes to high 8 bytes of packed double */                  \
      x(MOVLPD, movlpd)   /* move low 8 bytes to low 8 bytes of packed double */                   \
      x(MOVHLPS, movhlps) /* move high 8 bytes to low 8 bytes of packed float */                   \
      x(MOVHPS, movhps)   /* move high 8 bytes to high 8 bytes of packed float */                  \
      x(MOVLHPS, movlhps) /* move low 8 bytes to high 8 bytes of packed float */                   \
      x(MOVLPS, movlps)   /* move low 8 bytes to low 8 bytes of packed float */                    \
      ONEJIT_COMMENT()    /* ------------------------------------------------------------------ */ \
      ONEJIT_COMMENT()    /* [CPUID SSE2] is required by the following instructions ----------- */ \
      ONEJIT_COMMENT()    /* ------------------------------------------------------------------ */ \
      x(CVTSD2SI, cvtsd2si) /* convert double to int */                                            \
      x(CVTSD2SS, cvtsd2ss) /* convert double to float */                                          \
      x(CVTSI2SD, cvtsi2sd) /* convert int to double */                                            \
      x(CVTSI2SS, cvtsi2ss) /* convert int to float */                                             \
      x(CVTSS2SD, cvtss2sd) /* convert double to float */                                          \
      x(CVTSS2SI, cvtss2si) /* convert float to int */                                             \
      x(DIVSD, divsd)       /* divide double */                                                    \
      x(DIVSS, divss)       /* divide float */                                                     \
      x(MAXPD, maxpd)       /* maximum of two %xmm packed double */                                \
      x(MAXPS, maxps)       /* maximum of two %xmm packed float */                                 \
      x(MAXSD, maxsd)       /* maximum double */                                                   \
      x(MAXSS, maxss)       /* maximum float */                                                    \
      x(MINPD, minpd)       /* minimum of two %xmm packed double */                                \
      x(MINPS, minps)       /* minimum of two %xmm packed float */                                 \
      x(MINSD, minsd)       /* minimum double */                                                   \
      x(MINSS, minss)       /* minimum float */                                                    \
      x(MOVAPD, movapd)     /* move packed double from %xmm or aligned memory to %xmm */           \
      x(MOVAPS, movaps)     /* move packed float from %xmm or aligned memory to %xmm */            \
      x(MOVD, movd)         /* move 4 bytes between %xmm, register or memory */                    \
      x(MOVDQA, movdqa)     /* move packed int between %xmm and %xmm or aligned memory */          \
      x(MOVDQU, movdqu)     /* move packed int between %xmm and %xmm or unaligned memory */        \
      x(MOVMKSPD, movmkspd) /* extract the two sign bits from packed double */                     \
      x(MOVMKSPS, movmksps) /* extract the four sign bits from packed float */                     \
      x(MOVNTDQ, movntdq)   /* store packed int to aligned memory, with non-temporal hint */       \
      x(MOVNTPD, movntpd)   /* store packed double to aligned memory, with non-temporal hint */    \
      x(MOVNTPS, movntps)   /* store packed float to aligned memory, with non-temporal hint */     \
      x(MOVQ, movq)         /* move 8 bytes between between %xmm, register or memory */            \
      x(MOVSD, movsd)       /* move low double from %xmm to %xmm or memory */                      \
      x(MOVSS, movss)       /* move low float from %xmm to %xmm or memory */                       \
      x(MOVUPD, movupd)     /* move packed double from %xmm to %xmm or unaligned memory */         \
      x(MOVUPS, movups)     /* move packed float from %xmm to %xmm or unaligned memory */          \
      x(MULPD, mulpd)       /* multiply packed double from %xmm to %xmm or memory */               \
      x(MULPS, mulps)       /* multiply packed float from %xmm to %xmm or memory */                \
      x(PAND, pand)         /* bitwise AND of %xmm and %xmm or memory */                           \
      x(PANDN, pandn)       /* bitwise AND-NOT of %xmm and %xmm or memory */                       \
      x(POR, por)           /* bitwise OR of %xmm and %xmm or memory */                            \
      x(PXOR, pxor)         /* bitwise XOR of %xmm and %xmm or memory */                           \
      ONEJIT_COMMENT() /* --------------------------------------------------------------------- */ \
      ONEJIT_COMMENT() /* [CPUID SSE3] is required by the following instructions -------------- */ \
      x(LDDQU, lddqu)  /* load unaligned 128 bits into %xmm */                                     \
      ONEJIT_COMMENT() /* --------------------------------------------------------------------- */ \
      ONEJIT_COMMENT() /* [CPUID SSE4.1] is required by the following instructions ------------ */ \
      x(MOVNTDQA, movntdqa) /* load packed double from aligned memory, with non-temporal hint */   \
      ONEJIT_COMMENT() /* --------------------------------------------------------------------- */ \
      ONEJIT_COMMENT() /* [CPUID SSE4.2] is required by the following instructions ------------ */ \
      x(CRC32, crc32)  /* accumulate CRC32c of 1, 2, 4 or 8 bytes into register */                 \
      ONEJIT_COMMENT() /* --------------------------------------------------------------------- */ \
      ONEJIT_COMMENT() /* [CPUID LZCNT] is required by the following instructions ------------- */ \
      x(LZCNT, lzcnt)  /* count most significant zero bits */                                      \
      ONEJIT_COMMENT() /* --------------------------------------------------------------------- */ \
      ONEJIT_COMMENT() /* [CPUID MOVBE] is required by the following instructions ------------- */ \
      x(MOVBE, movbe)  /* move and byte-swap register */                                           \
      ONEJIT_COMMENT() /* --------------------------------------------------------------------- */ \
      ONEJIT_COMMENT() /* [CPUID POPCNT] is required by the following instructions ------------ */ \
      x(POPCNT, popcnt) /* count bits = 1 */                                                       \
      ONEJIT_COMMENT()  /* [CPUID RTM] is required by the following instructions ------------ */   \
      x(XBEGIN, xbegin) /* start TSX transaction. arg is displacement of code to run on abort.     \
                           writes %rax */

#define ONEJIT_X(NAME, name) ASM_##NAME,
  ONEJIT_OPSTMT2_ASM(ONEJIT_X)
#undef ONEJIT_X

#define ONEJIT_X(NAME, name) X86_##NAME,
      ONEJIT_OPSTMT2_X86(ONEJIT_X)
#undef ONEJIT_X
};

enum OpStmt3 : uint16_t {
  BAD_ST3 = 0,
  IF = 1,

// numeric values of the OpStmt3 enum constants below this line MAY CHANGE WITHOUT WARNING

#define ONEJIT_OPSTMT3_X86(x)                                                                      \
  x(/**/ IMUL3, imul3)  /* signed multiply by constant */                                          \
      x(SHLD, shld)     /* left shift register or memory, fill with another register */            \
      x(SHRD, shrd)     /* right shift register or memory, fill with another register */           \
      ONEJIT_COMMENT()  /* [CPUID SSE2] is required by the following instructions ------------- */ \
      x(PEXTRW, pextrw) /* extract 4 bytes from %xmm to register */                                \
      ONEJIT_COMMENT()  /* [CPUID SSE4.1] is required by the following instructions ----------- */ \
      x(EXTRACTPS, extractps) /* extract one float from packed floats */                           \
      x(INSERTPS, insertps)   /* insert one float into packed floats */                            \
      x(PINSR, pinsr)         /* insert 1,2,4 or 8 bytes from register or memory to %xmm */

#define ONEJIT_X(NAME, name) X86_##NAME,
  ONEJIT_OPSTMT3_X86(ONEJIT_X)
#undef ONEJIT_X
};

enum OpStmt4 : uint16_t {
  BAD_ST4 = 0,
  FOR = 1,
};

enum OpStmtN : uint16_t {
  BAD_STN = 0,
  ASSIGN_CALL = 1,
  BLOCK = 2,
  COND = 3, // n-ary IF ... [ELSEIF* ... [ELSE ...]]
  RETURN = 4,
  SWITCH = 5,

// numeric values of the OpStmtN enum constants below this line MAY CHANGE WITHOUT WARNING

#define ONEJIT_OPSTMTN_X86(x) /*                                                                */ \
  x(RET, ret)                 /* return from function call */

#define ONEJIT_X(NAME, name) X86_##NAME,
  ONEJIT_OPSTMTN_X86(ONEJIT_X)
#undef ONEJIT_X
};

constexpr OpStmt0 operator+(OpStmt0 op, int delta) noexcept {
  return OpStmt0(int(op) + delta);
}
constexpr OpStmt0 operator-(OpStmt0 op, int delta) noexcept {
  return OpStmt0(int(op) - delta);
}

constexpr OpStmt1 operator+(OpStmt1 op, int delta) noexcept {
  return OpStmt1(int(op) + delta);
}
constexpr OpStmt1 operator-(OpStmt1 op, int delta) noexcept {
  return OpStmt1(int(op) - delta);
}

constexpr OpStmt2 operator+(OpStmt2 op, int delta) noexcept {
  return OpStmt2(int(op) + delta);
}
constexpr OpStmt2 operator-(OpStmt2 op, int delta) noexcept {
  return OpStmt2(int(op) - delta);
}

constexpr OpStmt3 operator+(OpStmt3 op, int delta) noexcept {
  return OpStmt3(int(op) + delta);
}
constexpr OpStmt3 operator-(OpStmt3 op, int delta) noexcept {
  return OpStmt3(int(op) - delta);
}

constexpr OpStmt4 operator+(OpStmt4 op, int delta) noexcept {
  return OpStmt4(int(op) + delta);
}
constexpr OpStmt4 operator-(OpStmt4 op, int delta) noexcept {
  return OpStmt4(int(op) - delta);
}

constexpr OpStmtN operator+(OpStmtN op, int delta) noexcept {
  return OpStmtN(int(op) + delta);
}
constexpr OpStmtN operator-(OpStmtN op, int delta) noexcept {
  return OpStmtN(int(op) - delta);
}

/*
 * if op is a conditional jump, return its negation, i.e.:
 * convert ASM_JA  -> ASM_JBE
 * convert ASM_JAE -> ASM_JB
 * convert ASM_JB  -> ASM_JAE
 * convert ASM_JBE -> ASM_JA
 *
 * convert ASM_JE  -> ASM_JNE
 * convert ASM_JNE -> ASM_JE
 *
 * convert ASM_JG  -> ASM_JLE
 * convert ASM_JGE -> ASM_JL
 * convert ASM_JL  -> ASM_JGE
 * convert ASM_JLE -> ASM_JG
 *
 * other values are returned unchanged
 */
OpStmt1 negate_condjump(OpStmt1 op) noexcept;

constexpr bool is_assign(OpStmt2 op) noexcept {
  return op >= ADD_ASSIGN && op <= ASSIGN;
}

// if OpStmt2 is *_ASSIGN, return the corresponding Op2
Op2 to_op2(OpStmt2 op) noexcept;
// if OpStmt2 is *_ASSIGN, return the corresponding OpN
OpN to_opn(OpStmt2 op) noexcept;

// return the *_ASSIGN corresponding to Op2
OpStmt2 to_assign_op(Op2 op) noexcept;
// return the *_ASSIGN corresponding to OpN
OpStmt2 to_assign_op(OpN op) noexcept;

const Chars to_string(OpStmt0 op) noexcept;
const Chars to_string(OpStmt1 op) noexcept;
const Chars to_string(OpStmt2 op) noexcept;
const Chars to_string(OpStmt3 op) noexcept;
const Chars to_string(OpStmt4 op) noexcept;
const Chars to_string(OpStmtN op) noexcept;

const Fmt &operator<<(const Fmt &out, OpStmt0 op);
const Fmt &operator<<(const Fmt &out, OpStmt1 op);
const Fmt &operator<<(const Fmt &out, OpStmt2 op);
const Fmt &operator<<(const Fmt &out, OpStmt3 op);
const Fmt &operator<<(const Fmt &out, OpStmt4 op);
const Fmt &operator<<(const Fmt &out, OpStmtN op);

} // namespace onejit

#endif // ONEJIT_OPSTMT_HPP
