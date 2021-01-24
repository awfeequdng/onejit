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
 * varexpr.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/varexpr.hpp>

namespace onejit {

Variable Var::var() const noexcept {
  if (is_direct()) {
    return Variable::parse_direct(offset_or_direct());
  } else {
    return Variable::parse_indirect(kind(), get(sizeof(CodeItem)));
  }
}

Var Var::create(Variable var, Code *holder) noexcept {
  const NodeHeader header{VAR, var.kind(), 0};
  if (var.is_direct()) {
    return Var{Node{header, var.direct(), nullptr}};
  }
  while (holder) {
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_item(var.indirect())) {
      return Var{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return Var{};
}

std::ostream &operator<<(std::ostream &out, const Var &ve) {
  return out << ve.var();
}

} // namespace onejit
