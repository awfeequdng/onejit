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
 * inst.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEASM_X86_INST_HPP
#define ONEASM_X86_INST_HPP

namespace oneasm {
namespace x86 {

// describe x86/x86_64 EFLAGS
enum Eflags : uint16_t {
  CF = 1 << 0, // carry flag
  PF = 1 << 2, // parity flag
  // AF = 1 << 4, // auxiliary carry flag
  ZF = 1 << 6, // zero flag
  SF = 1 << 7, // sign flag
  // TF = 1 << 8, // trap flag
  // IF = 1 << 9, / interrupt enable flag
  DF = 1 << 10, // direction flag
  OF = 1 << 11, // overflow flag
};

// describe x86/x86_64 unary instruction operands
enum class Arg1 : uint8_t {
  AX = 1 << 0,  // %rax register
  R = 1 << 1,   // register
  M = 1 << 2,   // memory
  I8 = 1 << 3,  // 8-bit immediate
  I16 = 1 << 4, // 16-bit immediate
  I32 = 1 << 5, // 32-bit immediate
};

// describe x86/x86_64 binary instruction operands.
// last operand is result, as per AT&T / GNU syntax
enum class Arg2 : uint16_t {
  RtoAX = 1 << 0,
  RtoR = 1 << 1,
  RtoM = 1 << 2,

  MtoAX = 1 << 3,
  MtoR = 1 << 4,

  ItoAX = 1 << 5,
  ItoR = 1 << 6,
  ItoM = 1 << 7,

  AXtoM = 1 << 8, // currently only used by MOV
  CXtoR = 1 << 9, // currently only used by shifts and rotations
};

// describe x86/x86_64 ternary instruction operands
enum class Arg3 : uint8_t {
  R_ItoR, // register, immediate to register
  M_ItoR, // memory, immediate to register

};

// describe x86/x86_64 instruction constraints and side effects
class Inst {
public:
};

} // namespace x86
} // namespace oneasm

#endif // ONEASM_X86_INST_HPP
