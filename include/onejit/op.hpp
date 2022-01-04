/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

// see OpN for + * & | ^
enum Op2 : uint16_t {
  BAD2 = 0,
  SUB, // -
  QUO, // /
  REM, // %
  SHL, // <<
  SHR, // >>

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
  BADN = 0,
  ADD, // +
  MUL, // *
  AND, // &
  OR,  // |
  XOR, // ^
  MAX,
  MIN,
  COMMA,
  CALL,
  MEM_OP,

  // numeric values of the OpN enum constants below this line MAY CHANGE WITHOUT WARNING

  MIR_MEM,
  X86_MEM,
  ARM64_MEM,
};

constexpr Op1 operator+(Op1 op, int delta) noexcept {
  return Op1(int(op) + delta);
}
constexpr Op1 operator-(Op1 op, int delta) noexcept {
  return Op1(int(op) - delta);
}

constexpr Op2 operator+(Op2 op, int delta) noexcept {
  return Op2(int(op) + delta);
}
constexpr Op2 operator-(Op2 op, int delta) noexcept {
  return Op2(int(op) - delta);
}

constexpr OpN operator+(OpN op, int delta) noexcept {
  return OpN(int(op) + delta);
}
constexpr OpN operator-(OpN op, int delta) noexcept {
  return OpN(int(op) - delta);
}

constexpr bool is_comparison(Op2 op) noexcept {
  return op >= LSS && op <= GEQ;
}
// change < to >, <= to >=, > to < and >= to <=
Op2 swap_comparison(Op2 op) noexcept;
// change < to >=, <= to >, != to ==, == to !=, > to <= and >= to <
Op2 not_comparison(Op2 op) noexcept;

constexpr bool is_arithmetic(OpN op) noexcept {
  return op >= ADD && op <= COMMA;
}
constexpr bool is_associative(OpN op) noexcept {
  return op >= ADD && op <= COMMA;
}
constexpr bool is_commutative(OpN op) noexcept {
  return op >= ADD && op <= MIN;
}
constexpr bool is_bitwise(OpN op) noexcept {
  return op >= AND && op <= XOR;
}

const Chars to_string(Op1 op) noexcept;
const Chars to_string(Op2 op) noexcept;
const Chars to_string(OpN op) noexcept;

const Fmt &operator<<(const Fmt &fmt, Op1 op);
const Fmt &operator<<(const Fmt &fmt, Op2 op);
const Fmt &operator<<(const Fmt &fmt, OpN op);

} // namespace onejit

#endif // ONEJIT_OP1_HPP
