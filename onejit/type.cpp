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

#include <onejit/type.hpp>
#include <onestl/chars.hpp>

#include <ostream>

#define N_OF(array) (sizeof(array) / sizeof(array[0]))

namespace onejit {

static const int8_t tchildren[] = {
    0, 1, 2,  3, 4,  -1, // Stmt*
    0, 0, 1,  2, -1, 0,  // Expr
    0, 0, -1,            // FuncType, Label
};

static const Chars tstring[] = {
    "stmt0", "stmt1", "stmt2", "stmt3",  "stmt4", "stmtn", // Stmt*
    "var",   "mem",   "unary", "binary", "tuple", "const", // *Expr
    "ftype", "label", "?",                                 // FuncType, Label
};

uint32_t to_children(Type t) noexcept {
  const uint8_t n = N_OF(tchildren);
  uint8_t i = uint8_t(t);
  if (i >= n) {
    i = n - 1;
  }
  return (uint32_t)tchildren[i];
}

const Chars &to_string(Type t) noexcept {
  const uint8_t n = N_OF(tstring);
  uint8_t i = uint8_t(t);
  if (i >= n) {
    i = n - 1;
  }
  return tstring[i];
}

std::ostream &operator<<(std::ostream &out, Type t) {
  return out << to_string(t);
}

} // namespace onejit
