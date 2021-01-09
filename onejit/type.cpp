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

static const Chars tstring[] = {
    "bad",     "break", "continue", "fallthrough", //
    "default", "unary", "binary",   "case",        //
    "if",      "for",   "switch",   "tuple",       "?",
};

static const uint8_t tchildren[] = {
    0, 0, 0,   0, //
    1, 1, 2,   2, //
    3, 4, 255, 255, 0,
};

const Chars &to_string(Type t) {
  const uint8_t n = sizeof(tstring) / sizeof(tstring[0]);
  uint8_t i = uint8_t(t);
  if (i >= n) {
    i = n - 1;
  }
  return tstring[i];
}

uint8_t to_children(Type t) {
  const uint8_t n = sizeof(tchildren) / sizeof(tchildren[0]);
  uint8_t i = uint8_t(t);
  if (i >= n) {
    i = n - 1;
  }
  return tchildren[i];
}

std::ostream &operator<<(std::ostream &out, Type t) {
  return out << to_string(t);
}

} // namespace onejit
