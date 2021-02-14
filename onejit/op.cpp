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
 * op.cpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/fmt.hpp>
#include <onejit/op.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// ============================  Op1  ==========================================

static const char op1string[] = "\1?\0\0\0\1^\0\0\0\1!\0\0\0\1-\0\0\0\4cast\7bitcopy";

const Chars to_string(Op1 op) noexcept {
  size_t i = 0; // "?"
  if (op <= BITCOPY) {
    i = op;
  }
  const char *addr = op1string + i * 5;
  return Chars{addr + 1, uint8_t(addr[0])};
}

const Fmt &operator<<(const Fmt &out, Op1 op) {
  return out << to_string(op);
}

// ============================  Op2  ==========================================

bool is_associative(Op2 op) noexcept {
  switch (op) {
  case ADD:
  case MUL:
  case AND:
  case OR:
  case XOR:
    return true;
  default:
    return false;
  }
}

bool is_commutative(Op2 op) noexcept {
  switch (op) {
  case ADD:
  case MUL:
  case AND:
  case OR:
  case XOR:
  case NEQ:
  case EQL:
    return true;
  default:
    return false;
  }
}

// change < to >, <= to >=, > to < and >= to <=
Op2 swap_comparison(Op2 op) noexcept {
  switch (op) {
  case LSS:
    return GTR;
  case LEQ:
    return GEQ;
  case GTR:
    return LSS;
  case GEQ:
    return LEQ;
  default:
    return op;
  }
}

Op2 not_comparison(Op2 op) noexcept {
  switch (op) {
  case LSS:
    return GEQ;
  case LEQ:
    return GTR;
  case NEQ:
    return EQL;
  case EQL:
    return NEQ;
  case GTR:
    return LEQ;
  case GEQ:
    return LSS;
  default:
    return op;
  }
}

static const char op2string[] =
    "\1?\0\1+\0\1-\0\1*\0\1/\0\1%\0\1&\0\1|\0\1^\0\2<<\2>>\2&&\2||\1<\0\2<=\2!=\2==\1>\0\2>=";

const Chars to_string(Op2 op) noexcept {
  size_t i = 0; // "?"
  if (op <= GEQ) {
    i = op;
  }
  const char *addr = op2string + i * 3;
  return Chars{addr + 1, uint8_t(addr[0])};
}

const Fmt &operator<<(const Fmt &out, Op2 op) {
  return out << to_string(op);
}

// ============================  OpN  ==========================================

static const Chars opnstring[] = {"?", "call", "mem", "comma", "x86_mem", "arm64_mem"};

const Chars to_string(OpN op) noexcept {
  size_t i = 0; // "?"
  if (op < ONEJIT_N_OF(opnstring)) {
    i = op;
  }
  return opnstring[i];
}

const Fmt &operator<<(const Fmt &out, OpN op) {
  return out << to_string(op);
}

} // namespace onejit
