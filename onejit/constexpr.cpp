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
 * constexpr.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/constexpr.hpp>

namespace onejit {

Const ConstExpr::constant() const noexcept {
  if (is_direct()) {
    return Const::parse_direct(offset_or_direct());
  } else {
    return Const::parse_indirect(kind(), offset_or_direct() + sizeof(CodeItem), code());
  }
}

ConstExpr ConstExpr::create(const Const &c, Code *holder) noexcept {
  const NodeHeader header{CONST, c.kind(), 0};

  if (c.is_direct()) {
    return ConstExpr{Node{header, c.direct(), nullptr}};
  }
  while (holder) {
    CodeItem offset = holder->length();

    if (holder->add(header) && !c.write_indirect(holder)) {
      return ConstExpr{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return ConstExpr{};
}

std::ostream &operator<<(std::ostream &out, const ConstExpr &ce) {
  return out << ce.constant();
}

} // namespace onejit
