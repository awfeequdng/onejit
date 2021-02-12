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
 * group.cpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/group.hpp>
#include <onestl/chars.hpp>
#include <onestl/fmt.hpp>

namespace onejit {

static const Chars gstring[] = {
    "void", "bool",          //
    "int",  "uint", "float", //
    "ptr",  "arch", "?",     //
};

const Chars &to_string(Group g) noexcept {
  enum _ { n = sizeof(gstring) / sizeof(gstring[0]) };
  uint8_t i = (uint8_t)g;
  if (i >= n) {
    i = n - 1; // "?"
  }
  return gstring[i];
}

const Fmt &operator<<(const Fmt &out, Group g) {
  return out << to_string(g);
}

} // namespace onejit
