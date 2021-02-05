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
 * scale.cpp
 *
 *  Created on Feb 03, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/x86/scale.hpp>

namespace onejit {
namespace x86 {

static const char sstring[6] = {'0', '1', '2', '4', '8', '?'};

const Chars Scale::string() const noexcept {
  enum _ { n = ONEJIT_N_OF(sstring) };
  size_t i = val_;
  if (i >= n) {
    i = n - 1; // "?"
  }
  return Chars{sstring + i, 1};
}

ONEJIT_NOINLINE const Fmt &operator<<(const Fmt &out, Scale scale) {
  return out << scale.string();
}

const Fmt &operator<<(const Fmt &out, eScale escale) {
  return out << Scale{escale};
}

} // namespace x86
} // namespace onejit
