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
 * mem.cpp
 *
 *  Created on Jan 20, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/mem.hpp>

namespace onejit {

Node Mem::create(Func &func, Kind kind, OpN op, Exprs args) noexcept {
  const size_t n = args.size();
  Code *holder = func.code();

  while (holder && n == uint32_t(n)) {
    const NodeHeader header{MEM, kind, op};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(n) && holder->add(args, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Node{};
}

} // namespace onejit
