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
 * var.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/node/var.hpp>

namespace onejit {

Var::Var(Func &func, Kind kind) noexcept : Var{func.new_var(kind)} {
}

Var Var::create(Code *holder, Local local) noexcept {
  const NodeHeader header{VAR, local.kind(), 0};
  if (local.is_direct()) {
    return Var{Node{header, local.direct(), nullptr}};
  }
  while (holder) {
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_item(local.indirect())) {
      return Var{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return Var{};
}

Local Var::local() const noexcept {
  if (is_direct()) {
    return Local::parse_direct(offset_or_direct());
  } else {
    return Local::parse_indirect(kind(), get(sizeof(CodeItem)));
  }
}

const Fmt &Var::format(const Fmt &out, size_t /*depth*/) const {
  return out << local();
}

} // namespace onejit
