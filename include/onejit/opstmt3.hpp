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
  x(/**/ ADD, add)  /* x = y + z on 64-bit integer register or memory */                           \
      x(ADDS, adds) /* x = y + z on 32-bit integer register or memory */                           \
      x(AND, and)   /* x = y & z on 64-bit integer register or memory */                           \
      x(ANDS, ands) /* x = y & z on 32-bit integer register or memory */                           \
                                                                                                   \
      x(BEQ, beq)    /* jump to label x if 64 bit integer y == z */                                \
      x(BEQS, beqs)  /* jump to label x if 32 bit integer y == z */                                \
      x(FBEQ, dbeq)  /* jump to label x if float32 integer y == z */                               \
      x(DBEQ, dbeq)  /* jump to label x if float64 integer y == z */                               \
      x(LDBEQ, dbeq) /* jump to label x if float80 integer y == z */                               \
                                                                                                   \
      x(BNE, bne)    /* jump to label x if 64 bit integer y != z */                                \
      x(BNES, bnes)  /* jump to label x if 32 bit integer y != z */                                \
      x(FBNE, dbne)  /* jump to label x if float32 integer y != z */                               \
      x(DBNE, dbne)  /* jump to label x if float64 integer y != z */                               \
      x(LDBNE, dbne) /* jump to label x if float80 integer y != z */                               \
                                                                                                   \
      x(BLT, blt)     /* jump to label x if 64 bit integer y < z */                                \
      x(BLTS, blts)   /* jump to label x if 32 bit integer y < z */                                \
      x(UBLT, ublt)   /* jump to label x if 64 bit unsigned y < z */                               \
      x(UBLTS, ublts) /* jump to label x if 32 bit unsigned y < z */                               \
      x(FBLT, dblt)   /* jump to label x if float32 integer y < z */                               \
      x(DBLT, dblt)   /* jump to label x if float64 integer y < z */                               \
      x(LDBLT, dblt)  /* jump to label x if float80 integer y < z */                               \
                                                                                                   \
      x(BLE, ble)     /* jump to label x if 64 bit integer y <= z */                               \
      x(BLES, bles)   /* jump to label x if 32 bit integer y <= z */                               \
      x(UBLE, uble)   /* jump to label x if 64 bit unsigned y <= z */                              \
      x(UBLES, ubles) /* jump to label x if 32 bit unsigned y <= z */                              \
      x(FBLE, dble)   /* jump to label x if float32 integer y <= z */                              \
      x(DBLE, dble)   /* jump to label x if float64 integer y <= z */                              \
      x(LDBLE, dble)  /* jump to label x if float80 integer y <= z */                              \
                                                                                                   \
      x(BGT, bgt)     /* jump to label x if 64 bit integer y > z */                                \
      x(BGTS, bgts)   /* jump to label x if 32 bit integer y > z */                                \
      x(UBGT, ubgt)   /* jump to label x if 64 bit unsigned y > z */                               \
      x(UBGTS, ubgts) /* jump to label x if 32 bit unsigned y > z */                               \
      x(FBGT, dbgt)   /* jump to label x if float32 integer y > z */                               \
      x(DBGT, dbgt)   /* jump to label x if float64 integer y > z */                               \
      x(LDBGT, dbgt)  /* jump to label x if float80 integer y > z */                               \
                                                                                                   \
      x(BGE, bge)     /* jump to label x if 64 bit integer y >= z */                               \
      x(BGES, bges)   /* jump to label x if 32 bit integer y >= z */                               \
      x(UBGE, ubge)   /* jump to label x if 64 bit unsigned y >= z */                              \
      x(UBGES, ubges) /* jump to label x if 32 bit unsigned y >= z */                              \
      x(FBGE, dbge)   /* jump to label x if float32 integer y >= z */                              \
      x(DBGE, dbge)   /* jump to label x if float64 integer y >= z */                              \
      x(LDBGE, dbge)  /* jump to label x if float80 integer y >= z */

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

constexpr OpStmt3 operator+(OpStmt3 op, int delta) noexcept {
  return OpStmt3(int(op) + delta);
}
constexpr OpStmt3 operator-(OpStmt3 op, int delta) noexcept {
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
