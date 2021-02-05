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
 * op.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OP_HPP
#define ONEJIT_OP_HPP

#include <onejit/fmt.hpp>
#include <onejit/fwd.hpp>

#include <cstdint> // uint16_t

namespace onejit {

enum Op1 : uint16_t {
  BAD1 = 0,
  XOR1 = 1,    // invert all bits
  NOT1 = 2,    // boolean negation
  NEG1 = 3,    // arithmetic negative i.e. -x
  CAST = 4,    // truncate, zero-extend, sign-extend, float2int, int2float
  BITCOPY = 5, // copy float bits to integer or viceversa
};

enum Op2 : uint16_t {
  BAD2 = 0,
  ADD,     // +
  SUB,     // -
  MUL,     // *
  QUO,     // /
  REM,     // %
  AND,     // &
  OR,      // |
  XOR,     // ^
  SHL,     // <<
  SHR,     // >>
  AND_NOT, // &^

  LAND, // &&
  LOR,  // ||
  LSS,  // <
  LEQ,  // <=
  NEQ,  // <> !=
  EQL,  // ==
  GTR,  // >
  GEQ,  // >=
};

enum OpN : uint16_t {
  BAD_OPN = 0,
  CALL = 1,
  MEM_OP = 2,

  // numeric values of the OpN enum constants below this line MAY CHANGE WITHOUT WARNING

  X86_MEM,
  ARM64_MEM,
};

const Chars to_string(Op1 op) noexcept;
const Chars to_string(Op2 op) noexcept;
const Chars to_string(OpN op) noexcept;

const Fmt &operator<<(const Fmt &out, Op1 op);
const Fmt &operator<<(const Fmt &out, Op2 op);
const Fmt &operator<<(const Fmt &out, OpN op);

} // namespace onejit

#endif // ONEJIT_OP1_HPP
