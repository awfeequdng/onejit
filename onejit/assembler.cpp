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
 * assembler.cpp
 *
 *  Created on Jan 30, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/assembler.hpp>
#include <onejit/error.hpp>

namespace onejit {

Assembler::~Assembler() noexcept {
}

Assembler &Assembler::add_relocation(Label l) noexcept {
  if (l && !relocation_.append(Relocation{size(), l})) {
    good_ = false;
  }
  return *this;
}

Assembler &Assembler::error(Node where, Chars msg) noexcept {
  good_ = good_ && error_.append(Error{where, msg});
  return *this;
}

Assembler &Assembler::out_of_memory(Node where) noexcept {
  // always set good_ to false
  good_ = good_ && error_.append(Error{where, "out of memory"}) && false;
  return *this;
}

} // namespace onejit
