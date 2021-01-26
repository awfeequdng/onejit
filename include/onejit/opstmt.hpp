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

enum OpStmt0 : uint16_t {
  BAD = 0,
  BREAK = 1,
  CONTINUE = 2,
  FALLTHROUGH = 3,
};

enum OpStmt1 : uint16_t {
  BAD_ST1 = 0,
  GOTO = 1,
  INC = 2, // ++
  DEC = 3, // --

  X86_BSWAP = 64,     // x = byteswap(x) - invert endianity of 32bit or 64bit register
  X86_CALL,           // call function. argument is relative offset, register or memory
  X86_CBW,            // sign-extend  %al ->  %ax
  X86_CWDE = X86_CBW, // sign-extend  %ax -> %eax
  X86_CDQE = X86_CBW, // sign-extend %eax -> %rax
  X86_CLC,            // clear carry flag
  X86_CLD,            // clear direction flag
  X86_CMC,            // complement carry flag

  X86_CLFLUSH,    // clear all L1/L2... caches containing memory address. [CPUID CLFSH]
  X86_CLFLUSHOPT, // clear all L1/L2... caches containing memory address. [CPUID CLFLUSHOPT]
  X86_CLWB,       // write-back cache line containing memory address. [CPUID CLWB]

};

enum OpStmt2 : uint16_t {
  BAD_ST2 = 0,
  // intentionally same values as Op2
  ADD_ASSIGN,     // +=
  SUB_ASSIGN,     // -=
  MUL_ASSIGN,     // *=
  QUO_ASSIGN,     // /=
  REM_ASSIGN,     // %=
  AND_ASSIGN,     // &=
  OR_ASSIGN,      // |=
  XOR_ASSIGN,     // ^=
  SHL_ASSIGN,     // <<=
  SHR_ASSIGN,     // >>=
  AND_NOT_ASSIGN, // &^=
  ASSIGN,         // =

  CASE,
  DEFAULT,
  JUMP_IF,

  ASM_MOV = 32, // copy register to register
  ASM_LOAD,     // load register from memory
  ASM_STOR,     // store register to memory
  ASM_CMP,      // compare two arguments, set architectural flags

  X86_ADD = 64,   // x += y
  X86_ADC,        // x += y + carry
  X86_AND,        // x &= y
  X86_BSF,        // x = bitscan_forward(y)
  X86_BSR,        // x = bitscan_reverse(y)
  X86_BT,         // bit test
  X86_BTC,        // bit test and complement
  X86_BTR,        // bit test and reset
  X86_BTS,        // bit test and set
  X86_CMOVA,      // conditional move if above
  X86_CMOVAE,     // conditional move if above or equal (if not carry)
  X86_CMOVB,      // conditional move if below (if carry)
  X86_CMOVBE,     // conditional move if below or equal
  X86_CMOVE,      // conditional move if equal (if zero)
  X86_CMOVG,      // conditional move if greater
  X86_CMOVGE,     // conditional move if greater or equal
  X86_CMOVL,      // conditional move if less
  X86_CMOVLE,     // conditional move if less or equal
  X86_CMOVNE,     // conditional move if not equal (if not zero)
  X86_CMOVNO,     // conditional move if not overflow
  X86_CMOVNP,     // conditional move if not parity (if odd)
  X86_CMOVNS,     // conditional move if not sign
  X86_CMOVO,      // conditional move if overflow
  X86_CMOVP,      // conditional move if parity (if even)
  X86_CMOVS,      // conditional move if sign
  X86_CMP,        // compare two arguments
  X86_CMPXCHG,    // compare and exchange 1, 2, 4 or 8 bytes
  X86_CMPXCHG8B,  // compare and exchange 8 bytes
  X86_CMPXCHG16B, // compare and exchange 16 bytes

  // the following instructions require [CPUID SSE2]
  X86_CVTSD2SI, // convert double to int
  X86_CVTSD2SS, // convert double to float
  X86_CVTSI2SD, // convert int to double
  X86_CVTSI2SS, // convert int to float
  X86_CVTSS2SD, // convert double to float
  X86_CVTSS2SI, // convert float to int
  X86_DIVSD,    // divide double
  X86_DIVSS,    // divide float

  // the following instructions require [CPUID SSE4.1]
  X86_EXTRACTPS, // extract one float from packed floats

  // the following instructions require [CPUID SSE4.2]
  X86_CRC32, // accumulate CRC32c of 1, 2, 4 or 8 bytes
};

enum OpStmt3 : uint16_t {
  BAD_ST3 = 0,
  IF = 1,

  X86_DIV = 64, // unsigned divide %rax:%rdx by argument
  X86_IDIV,     // signed divide %rax:%rdx by argument
  X86_IMUL,     // signed multiply
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
};

const Chars &to_string(OpStmt0 op) noexcept;
const Chars &to_string(OpStmt1 op) noexcept;
const Chars &to_string(OpStmt2 op) noexcept;
const Chars &to_string(OpStmt3 op) noexcept;
const Chars &to_string(OpStmt4 op) noexcept;
const Chars &to_string(OpStmtN op) noexcept;

const Fmt &operator<<(const Fmt &out, OpStmt0 op);
const Fmt &operator<<(const Fmt &out, OpStmt1 op);
const Fmt &operator<<(const Fmt &out, OpStmt2 op);
const Fmt &operator<<(const Fmt &out, OpStmt3 op);
const Fmt &operator<<(const Fmt &out, OpStmt4 op);
const Fmt &operator<<(const Fmt &out, OpStmtN op);

} // namespace onejit

#endif // ONEJIT_OPSTMT_HPP
