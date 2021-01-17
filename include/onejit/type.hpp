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
 * type.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_TYPE_HPP
#define ONEJIT_TYPE_HPP

#include <cstdint> // uint8_t
#include <iosfwd>  // std::ostream

namespace onejit {

enum Type : uint8_t {
  BAD = 0,
  // Stmt0
  BREAK = 1,
  CONTINUE = 2,
  FALLTHROUGH = 3,

  // Stmt[1+]
  STMT_2 = 4, // CASE, DEFAULT: latter has child(0) = Const(Void,0)
  STMT_3 = 5, // IF
  STMT_4 = 6, // FOR
  STMT_N = 7, // BLOCK, COND, SWITCH: child(0) is OpN

  // Expr
  VAR = 8,
  UNARY = 9,
  BINARY = 10,
  TUPLE = 11, // CALL, RET, TUPLE_ASSIGN: child(0) is OpN
  MEM = 12,
  CONST = 13,
};

class Chars;
const Chars &to_string(Type t);

std::ostream &operator<<(std::ostream &out, Type t);

} // namespace onejit

#endif // ONEJIT_TYPE_HPP
