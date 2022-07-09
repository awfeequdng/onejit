/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * opstmt3.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPSTMT3_HPP
#define ONEJIT_OPSTMT3_HPP

#include <onejit/fmt.hpp>
#include <onejit/fwd.hpp>

#include <cstdint> // uint8_t

namespace onejit {

enum OpStmt3 : uint16_t {
  BAD_ST3 = 0,
  IF = 1,

// numeric values of the OpStmt3 enum constants below this line MAY CHANGE WITHOUT WARNING

#define ONEJIT_OPSTMT3_ASM(x)                                                                      \
  x(/**/ JA, ja)  /* jump if above */                                                              \
      x(JAE, jae) /* jump if above or equal */                                                     \
      x(JB, jb)   /* jump if below */                                                              \
      x(JBE, jbe) /* jump if below or equal */                                                     \
      x(JE, je)   /* jump if equal (if zero) */                                                    \
      x(JG, jg)   /* jump if greater */                                                            \
      x(JGE, jge) /* jump if greater or equal */                                                   \
      x(JL, jl)   /* jump if less */                                                               \
      x(JLE, jle) /* jump if less or equal */                                                      \
      x(JNE, jne) /* jump if not equal (if not zero) */

#define ONEJIT_OPSTMT3_MIR(x)                                                                      \
  x(/**/ ADD, add)    /* x = y + z on [u]int64 */                                                  \
      x(ADDS, adds)   /* x = y + z on [u]int32 */                                                  \
      x(FADD, fadd)   /* x = y + z on float32  */                                                  \
      x(DADD, dadd)   /* x = y + z on float64  */                                                  \
      x(LDADD, ldadd) /* x = y + z on float128  */                                                 \
                                                                                                   \
      x(SUB, sub)     /* x = y - z on [u]int64 */                                                  \
      x(SUBS, subs)   /* x = y - z on [u]int32 */                                                  \
      x(FSUB, fsub)   /* x = y - z on float32  */                                                  \
      x(DSUB, dsub)   /* x = y - z on float64  */                                                  \
      x(LDSUB, ldsub) /* x = y - z on float128  */                                                 \
                                                                                                   \
      x(MUL, mul)     /* x = y * z on [u]int64 */                                                  \
      x(MULS, muls)   /* x = y * z on [u]int32 */                                                  \
      x(FMUL, fmul)   /* x = y * z on float32  */                                                  \
      x(DMUL, dmul)   /* x = y * z on float64  */                                                  \
      x(LDMUL, ldmul) /* x = y * z on float128  */                                                 \
                                                                                                   \
      x(DIV, div)     /* x = y / z on int64    */                                                  \
      x(DIVS, divs)   /* x = y / z on int32    */                                                  \
      x(UDIV, udiv)   /* x = y / z on uint64   */                                                  \
      x(UDIVS, udivs) /* x = y / z on uint32   */                                                  \
      x(FDIV, fdiv)   /* x = y / z on float32  */                                                  \
      x(DDIV, ddiv)   /* x = y / z on float64  */                                                  \
      x(LDDIV, lddiv) /* x = y / z on float128  */                                                 \
                                                                                                   \
      x(MOD, mod)     /* x = y % z on int64    */                                                  \
      x(MODS, mods)   /* x = y % z on int32    */                                                  \
      x(UMOD, umod)   /* x = y % z on uint64   */                                                  \
      x(UMODS, umods) /* x = y % z on uint32   */                                                  \
                                                                                                   \
      x(AND, and)     /* x = y & z on [u]int64 */                                                  \
      x(ANDS, ands)   /* x = y & z on [u]int32 */                                                  \
      x(OR, or)       /* x = y | z on [u]int64 */                                                  \
      x(ORS, ors)     /* x = y | z on [u]int32 */                                                  \
      x(XOR, xor)     /* x = y ^ z on [u]int64 */                                                  \
      x(XORS, xors)   /* x = y ^ z on [u]int32 */                                                  \
      x(LSH, lsh)     /* x = y << z on [u]int64 */                                                 \
      x(LSHS, lshs)   /* x = y << z on [u]int32 */                                                 \
      x(RSH, rsh)     /* x = y >> z on int64 */                                                    \
      x(RSHS, rshs)   /* x = y >> z on int32 */                                                    \
      x(URSH, ursh)   /* x = y >> z on uint64 */                                                   \
      x(URSHS, urshs) /* x = y >> z on uint32 */                                                   \
                                                                                                   \
      x(EQ, eq)     /* set x = y == z on [u]int64 */                                               \
      x(EQS, eqs)   /* set x = y == z on [u]int32 */                                               \
      x(FEQ, feq)   /* set x = y == z on float32  */                                               \
      x(DEQ, deq)   /* set x = y == z on float64  */                                               \
      x(LDEQ, ldeq) /* set x = y == z on float128  */                                              \
      x(NE, ne)     /* set x = y != z on [u]int64 */                                               \
      x(NES, nes)   /* set x = y != z on [u]int32 */                                               \
      x(FNE, fne)   /* set x = y != z on float32  */                                               \
      x(DNE, dne)   /* set x = y != z on float64  */                                               \
      x(LDNE, ldne) /* set x = y != z on float128  */                                              \
      x(LT, lt)     /* set x = y <  z on int64   */                                                \
      x(LTS, lts)   /* set x = y <  z on int32   */                                                \
      x(ULT, ult)   /* set x = y <  z on uint64  */                                                \
      x(ULTS, ults) /* set x = y <  z on uint32  */                                                \
      x(FLT, flt)   /* set x = y <  z on float32 */                                                \
      x(DLT, dlt)   /* set x = y <  z on float64 */                                                \
      x(LDLT, ldlt) /* set x = y <  z on float128 */                                               \
      x(LE, le)     /* set x = y <= z on int64   */                                                \
      x(LES, les)   /* set x = y <= z on int32   */                                                \
      x(ULE, ule)   /* set x = y <= z on uint64  */                                                \
      x(ULES, ules) /* set x = y <= z on uint32  */                                                \
      x(FLE, fle)   /* set x = y <= z on float32 */                                                \
      x(DLE, dle)   /* set x = y <= z on float64 */                                                \
      x(LDLE, ldle) /* set x = y <= z on float128 */                                               \
      x(GT, gt)     /* set x = y >  z on int64   */                                                \
      x(GTS, gts)   /* set x = y >  z on int32   */                                                \
      x(UGT, ugt)   /* set x = y >  z on uint64  */                                                \
      x(UGTS, ugts) /* set x = y >  z on uint32  */                                                \
      x(FGT, fgt)   /* set x = y >  z on float32 */                                                \
      x(DGT, dgt)   /* set x = y >  z on float64 */                                                \
      x(LDGT, ldgt) /* set x = y >  z on float128 */                                               \
      x(GE, ge)     /* set x = y >= z on int64   */                                                \
      x(GES, ges)   /* set x = y >= z on int32   */                                                \
      x(UGE, uge)   /* set x = y >= z on uint64  */                                                \
      x(UGES, uges) /* set x = y >= z on uint32  */                                                \
      x(FGE, fge)   /* set x = y >= z on float32 */                                                \
      x(DGE, dge)   /* set x = y >= z on float64 */                                                \
      x(LDGE, ldge) /* set x = y >= z on float128 */                                               \
                                                                                                   \
      x(BEQ, beq)     /* jump to label x if y == z on [u]int64 */                                  \
      x(BEQS, beqs)   /* jump to label x if y == z on [u]int32 */                                  \
      x(FBEQ, fbeq)   /* jump to label x if y == z on float32  */                                  \
      x(DBEQ, dbeq)   /* jump to label x if y == z on float64  */                                  \
      x(LDBEQ, ldbeq) /* jump to label x if y == z on float128  */                                 \
      x(BNE, bne)     /* jump to label x if y != z on [u]int64 */                                  \
      x(BNES, bnes)   /* jump to label x if y != z on [u]int32 */                                  \
      x(FBNE, dbne)   /* jump to label x if y != z on float32  */                                  \
      x(DBNE, dbne)   /* jump to label x if y != z on float64  */                                  \
      x(LDBNE, dbne)  /* jump to label x if y != z on float128  */                                 \
      x(BLT, blt)     /* jump to label x if y <  z on int64   */                                   \
      x(BLTS, blts)   /* jump to label x if y <  z on int32   */                                   \
      x(UBLT, ublt)   /* jump to label x if y <  z on uint64  */                                   \
      x(UBLTS, ublts) /* jump to label x if y <  z on uint32  */                                   \
      x(FBLT, dblt)   /* jump to label x if y <  z on float32 */                                   \
      x(DBLT, dblt)   /* jump to label x if y <  z on float64 */                                   \
      x(LDBLT, dblt)  /* jump to label x if y <  z on float128 */                                  \
      x(BLE, ble)     /* jump to label x if y <= z on int64   */                                   \
      x(BLES, bles)   /* jump to label x if y <= z on int32   */                                   \
      x(UBLE, uble)   /* jump to label x if y <= z on uint64  */                                   \
      x(UBLES, ubles) /* jump to label x if y <= z on uint32  */                                   \
      x(FBLE, dble)   /* jump to label x if y <= z on float32 */                                   \
      x(DBLE, dble)   /* jump to label x if y <= z on float64 */                                   \
      x(LDBLE, dble)  /* jump to label x if y <= z on float128 */                                  \
      x(BGT, bgt)     /* jump to label x if y >  z on int64   */                                   \
      x(BGTS, bgts)   /* jump to label x if y >  z on int32   */                                   \
      x(UBGT, ubgt)   /* jump to label x if y >  z on uint64  */                                   \
      x(UBGTS, ubgts) /* jump to label x if y >  z on uint32  */                                   \
      x(FBGT, dbgt)   /* jump to label x if y >  z on float32 */                                   \
      x(DBGT, dbgt)   /* jump to label x if y >  z on float64 */                                   \
      x(LDBGT, dbgt)  /* jump to label x if y >  z on float128 */                                  \
      x(BGE, bge)     /* jump to label x if y >= z on int64   */                                   \
      x(BGES, bges)   /* jump to label x if y >= z on int32   */                                   \
      x(UBGE, ubge)   /* jump to label x if y >= z on uint64  */                                   \
      x(UBGES, ubges) /* jump to label x if y >= z on uint32  */                                   \
      x(FBGE, dbge)   /* jump to label x if y >= z on float32 */                                   \
      x(DBGE, dbge)   /* jump to label x if y >= z on float64 */                                   \
      x(LDBGE, dbge)  /* jump to label x if y >= z on float128 */

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

#define ONEJIT_X(NAME, name) ASM_##NAME,
  ONEJIT_OPSTMT3_ASM(ONEJIT_X)
#undef ONEJIT_X

#define ONEJIT_X(NAME, name) MIR_##NAME,
      ONEJIT_OPSTMT3_MIR(ONEJIT_X)
#undef ONEJIT_X

#define ONEJIT_X(NAME, name) X86_##NAME,
          ONEJIT_OPSTMT3_X86(ONEJIT_X)
#undef ONEJIT_X
};

constexpr inline OpStmt3 operator+(OpStmt3 op, int delta) noexcept {
  return OpStmt3(int(op) + delta);
}
constexpr inline OpStmt3 operator-(OpStmt3 op, int delta) noexcept {
  return OpStmt3(int(op) - delta);
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
OpStmt3 negate_condjump(OpStmt3 op) noexcept;

const Chars to_string(OpStmt3 op) noexcept;

const Fmt &operator<<(const Fmt &fmt, OpStmt3 op);

} // namespace onejit

#endif // ONEJIT_OPSTMT3_HPP
