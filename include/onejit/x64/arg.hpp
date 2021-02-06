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
 * arg.hpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_ARG_HPP
#define ONEJIT_X64_ARG_HPP

#include <cstdint> // uint*_t

namespace onejit {
namespace x64 {

// describe x86/x64 EFLAGS
enum EflagsMask : uint16_t {
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

// instruction effect on eflags: none, read, write, both
enum Eflags : uint8_t {
  EFnone = 0,
  EFread = 1,
  EFwrite = 2,
  EFrw = 3,
};

// describe supported width of registers, memory access or immediate constants
// in x86/x64 instructions
enum BitSize : uint8_t {
  B0 = 0,        // zero
  B8 = 1 << 0,   // 8 bit
  B16 = 1 << 1,  // 16 bit
  B32 = 1 << 2,  // 32 bit
  B64 = 1 << 3,  // 64 bit
  B128 = 1 << 4, // 128 bit. for Rax means %rdx:%rax
};

// describe x86/x64 unary instruction operands
enum class Arg1 : uint8_t {
  None = 0,
  Rax = 1 << 0,               // %rax register
  Rcx = 1 << 1,               // %rcx register. only used to compute Arg2, Arg3
  Reg = (1 << 2) | Rax | Rcx, // general register
  Xmm = 1 << 3,               // %xmm register. only used to compute Arg2, Arg3
  Mem = 1 << 4,               // memory
  Val = 1 << 5,               // immediate or label
};

// describe x86/x64 binary instruction operands.
// result is first operand, as per Intel syntax
// (instead in AT&T / GNU syntax, result is last operand)
enum class Arg2 : uint16_t {
  None = 0,
  Rax_Reg = 1 << 0, // %rax OP= register
  Rax_Mem = 1 << 1, // %rax OP= memory
  Rax_Val = 1 << 2, // %rax OP= immediate

  Reg_Rcx = 1 << 3, // register OP= %rcx. currently only used by shifts and rotations
  Reg_Reg = (1 << 4) | Rax_Reg | Reg_Rcx,
  Reg_Xmm = 1 << 5, // register OP= %xmm
  Reg_Mem = (1 << 6) | Rax_Mem,
  Reg_Val = (1 << 7) | Rax_Val,

  Xmm_Reg = 1 << 8,  // %xmm OP= register
  Xmm_Xmm = 1 << 9,  // %xmm OP= %xmm
  Xmm_Mem = 1 << 10, // %xmm OP= memory

  Mem_Rax = 1 << 11, // memory OP= %rax. currently only used by MOV
  Mem_Reg = (1 << 12) | Mem_Rax,
  Mem_Xmm = 1 << 13, // memory OP= %xmm
  Mem_Val = 1 << 14,
};

// describe x86/x64 ternary instruction operands
enum class Arg3 : uint8_t {
  None = 0,
  Reg_Reg_Val = 1 << 0, // register = register OP immediate
  Reg_Mem_Val = 1 << 1, // register = memory OP immediate
};

////////////////////////////////////////////////////////////////////////////////
constexpr inline EflagsMask operator&(EflagsMask a, EflagsMask b) noexcept {
  return EflagsMask(int(a) & int(b));
}
constexpr inline EflagsMask operator|(EflagsMask a, EflagsMask b) noexcept {
  return EflagsMask(int(a) | int(b));
}

constexpr inline Eflags operator&(Eflags a, Eflags b) noexcept {
  return Eflags(int(a) & int(b));
}
constexpr inline Eflags operator|(Eflags a, Eflags b) noexcept {
  return Eflags(int(a) | int(b));
}

constexpr inline BitSize operator&(BitSize a, BitSize b) noexcept {
  return BitSize(int(a) & int(b));
}
constexpr inline BitSize operator|(BitSize a, BitSize b) noexcept {
  return BitSize(int(a) | int(b));
}

constexpr inline Arg1 operator&(Arg1 a, Arg1 b) noexcept {
  return Arg1(int(a) & int(b));
}
constexpr inline Arg1 operator|(Arg1 a, Arg1 b) noexcept {
  return Arg1(int(a) | int(b));
}

constexpr inline Arg2 operator&(Arg2 a, Arg2 b) noexcept {
  return Arg2(int(a) & int(b));
}
constexpr inline Arg2 operator|(Arg2 a, Arg2 b) noexcept {
  return Arg2(int(a) | int(b));
}

constexpr inline Arg3 operator&(Arg3 a, Arg3 b) noexcept {
  return Arg3(int(a) & int(b));
}
constexpr inline Arg3 operator|(Arg3 a, Arg3 b) noexcept {
  return Arg3(int(a) | int(b));
}

////////////////////////////////////////////////////////////////////////////////
BitSize to_bitsize(Bits bits) noexcept;
Arg1 to_arg(const Node &node1) noexcept;
Arg2 to_arg(const Node &node1, const Node &node2) noexcept;
Arg3 to_arg(const Node &node1, const Node &node2, const Node &node3) noexcept;

bool is_compatible(Bits bits, BitSize bitsize) noexcept;
bool is_compatible(const Node &node1, Arg1 arg) noexcept;
bool is_compatible(const Node &node1, const Node &node2, Arg2 arg) noexcept;
bool is_compatible(const Node &node1, const Node &node2, const Node &node3, Arg3 arg) noexcept;

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_ARG_HPP
