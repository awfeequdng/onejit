/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * opstmt2.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPSTMT2_HPP
#define ONEJIT_OPSTMT2_HPP

#include <onejit/fmt.hpp>
#include <onejit/fwd.hpp>

#include <cstdint> // uint8_t

namespace onejit {

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

#define ONEJIT_OPSTMT2_MIR(x)                                                                      \
  x(MOV, mov)         /* x = y on integer register (any size) or memory */                         \
      x(FMOV, fmov)   /* x = y on 32-bit float register or memory */                               \
      x(DMOV, dmov)   /* x = y on 64-bit float register or memory */                               \
      x(LDMOV, ldmov) /* x = y on 80-bit float register or memory */                               \
                                                                                                   \
      x(EXT8, ext8)     /* sign-extend 8-bit integer to 64 bit */                                  \
      x(EXT16, ext16)   /* sign-extend 16-bit integer to 64 bit */                                 \
      x(EXT32, ext32)   /* sign-extend 32-bit integer to 64 bit */                                 \
      x(UEXT8, uext8)   /* zero-extend 8-bit integer to 64 bit */                                  \
      x(UEXT16, uext16) /* zero-extend 16-bit integer to 64 bit */                                 \
      x(UEXT32, uext32) /* zero-extend 32-bit integer to 64 bit */                                 \
      x(I2F, i2f)       /* convert int64 to float32 */                                             \
      x(I2D, i2d)       /* convert int64 to float64 */                                             \
      x(I2LD, i2ld)     /* convert int64 to float80 */                                             \
      x(UI2F, ui2f)     /* convert uint64 to float32 */                                            \
      x(UI2D, ui2d)     /* convert uint64 to float64 */                                            \
      x(UI2LD, ui2ld)   /* convert uint64 to float80 */                                            \
      x(F2I, f2i)       /* convert float32 to int64 */                                             \
      x(D2I, d2i)       /* convert float64 to int64 */                                             \
      x(LD2I, ld2i)     /* convert float80 to int64 */                                             \
      x(F2D, f2d)       /* convert float32 to float64 */                                           \
      x(F2LD, f2ld)     /* convert float32 to float80 */                                           \
      x(D2F, d2f)       /* convert float64 to float32 */                                           \
      x(D2LD, d2ld)     /* convert float64 to float80 */                                           \
      x(LD2F, ld2f)     /* convert float80 to float32 */                                           \
      x(LD2D, ld2d)     /* convert float80 to float64 */                                           \
      x(NEG, neg)       /* x = -y on 64-bit integer register or memory */                          \
      x(NEGS, negs)     /* x = -y on 32-bit integer register or memory */                          \
      x(FNEG, fneg)     /* x = -y on float32 (or memory?) */                                       \
      x(DNEG, dneg)     /* x = -y on float64 (or memory?) */                                       \
      x(LDNEG, ldneg)   /* x = -y on float80 (or memory?) */

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

#define ONEJIT_X(NAME, name) MIR_##NAME,
  ONEJIT_OPSTMT2_MIR(ONEJIT_X)
#undef ONEJIT_X

#define ONEJIT_X(NAME, name) X86_##NAME,
      ONEJIT_OPSTMT2_X86(ONEJIT_X)
#undef ONEJIT_X
};

constexpr OpStmt2 operator+(OpStmt2 op, int delta) noexcept {
  return OpStmt2(int(op) + delta);
}
constexpr OpStmt2 operator-(OpStmt2 op, int delta) noexcept {
  return OpStmt2(int(op) - delta);
}

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

const Chars to_string(OpStmt2 op) noexcept;

const Fmt &operator<<(const Fmt &fmt, OpStmt2 op);

} // namespace onejit

#endif // ONEJIT_OPSTMT2_HPP
