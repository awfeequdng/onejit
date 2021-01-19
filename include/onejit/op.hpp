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
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OP_HPP
#define ONEJIT_OP_HPP

#include <onejit/fwd.hpp>

#include <cstdint> // uint16_t
#include <iosfwd>  // std::ostream

namespace onejit {

enum Op1 : uint16_t {
  BAD1 = 0,
  XOR1 = 1,
  NOT1 = 2,
  GOTO = 3,
};

// intentionally match Go go/token.Token values
enum Op2 : uint16_t {
  BAD2 = 0,
  ADD = 12, // +
  SUB,      // -
  MUL,      // *
  QUO,      // /
  REM,      // %

  AND,     // &
  OR,      // |
  XOR,     // ^
  SHL,     // <<
  SHR,     // >>
  AND_NOT, // &^

  ADD_ASSIGN, // +=
  SUB_ASSIGN, // -=
  MUL_ASSIGN, // *=
  QUO_ASSIGN, // /=
  REM_ASSIGN, // %=

  AND_ASSIGN,     // &=
  OR_ASSIGN,      // |=
  XOR_ASSIGN,     // ^=
  SHL_ASSIGN,     // <<=
  SHR_ASSIGN,     // >>=
  AND_NOT_ASSIGN, // &^=

  LAND, // &&
  LOR,  // ||

  EQL = LOR + 4, // ==
  LSS,           // <
  GTR,           // >
  ASSIGN,        // =

  NEQ = ASSIGN + 2, // !=
  LEQ,              // <=
  GEQ,              // >=
};

enum OpN : uint16_t {
  BAD_OPN = 0,
  CALL = 1,
  TUPLE_ASSIGN = 2,
  RETURN = 3,
};

const Chars &to_string(Op1 op);
const Chars &to_string(Op2 op);
const Chars &to_string(OpN op);

std::ostream &operator<<(std::ostream &out, Op1 op);
std::ostream &operator<<(std::ostream &out, Op2 op);
std::ostream &operator<<(std::ostream &out, OpN op);

} // namespace onejit

#endif // ONEJIT_OP1_HPP
