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
 * const.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/const.hpp>

namespace onejit {

Imm Const::imm() const noexcept {
  if (is_direct()) {
    return Imm::parse_direct(offset_or_direct());
  } else {
    return Imm::parse_indirect(kind(), offset_or_direct() + sizeof(CodeItem), code());
  }
}

Node Const::create(Code *holder, const Imm &imm) noexcept {
  const NodeHeader header{CONST, imm.kind(), 0};

  if (imm.is_direct()) {
    return Node{header, imm.direct(), nullptr};
  }
  while (holder) {
    CodeItem offset = holder->length();

    if (holder->add(header) && !imm.write_indirect(holder)) {
      return Const{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return Const{};
}

std::ostream &operator<<(std::ostream &out, const Const &c) {
  return out << c.imm();
}

} // namespace onejit
