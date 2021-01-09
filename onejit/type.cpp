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

#include "onejit/type.hpp"
#include "onejit/chars.hpp"

#include <ostream>

namespace onejit {

static const Chars typestring[] = {
    "bad",     "unary",       "binary", "tuple", "break",  "case", "continue",
    "default", "fallthrough", "for",    "if",    "switch", "?",
};

const Chars &to_string(Type op) {
  uint8_t i = sizeof(typestring) / sizeof(typestring[0]) - 1; // "?"
  switch (op) {
  case BAD:
    i = 0;
    break;
  case UNARY:
    i = 1;
    break;
  case BINARY:
    i = 2;
    break;
  case TUPLE:
    i = 3;
    break;
  case BREAK:
    i = 4;
    break;
  case CASE:
    i = 5;
    break;
  case CONTINUE:
    i = 6;
    break;
  case DEFAULT:
    i = 7;
    break;
  case FALLTHROUGH:
    i = 8;
    break;
  case FOR:
    i = 9;
    break;
  case IF:
    i = 10;
    break;
  case SWITCH:
    i = 11;
    break;
  }
  return typestring[i];
}

std::ostream &operator<<(std::ostream &out, Type op) {
  return out << to_string(op);
}

} // namespace onejit
