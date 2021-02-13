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
#include <onejit/func.hpp>
#include <onejit/node/const.hpp>

namespace onejit {

Imm Const::imm() const noexcept {
  if (is_direct()) {
    return Imm::parse_direct(offset_or_direct());
  } else {
    return Imm::parse_indirect(kind(), offset_or_direct() + sizeof(CodeItem), code());
  }
}

Node Const::create(Func &func, const Imm &imm) noexcept {
  if (imm.is_valid()) {
    const NodeHeader header{CONST, imm.kind(), 0};

    if (imm.is_direct()) {
      return Node{header, imm.direct(), nullptr};
    }
    if (Code *holder = func.code()) {
      CodeItem offset = holder->length();

      if (holder->add(header) && imm.write_indirect(holder)) {
        return Node{header, offset, holder};
      }
      holder->truncate(offset);
    }
  }
  return Node{};
}

Const One(Func &func, Kind kind) noexcept {
  switch (kind.val()) {
  case eFloat32:
    return Const{func, float(1)};
  case eFloat64:
    return Const{func, double(1)};
  default:
    return Const{kind, uint16_t(1)};
  }
}

Const Two(Func &func, Kind kind) noexcept {
  switch (kind.val()) {
  case eFloat32:
    return Const{func, float(2)};
  case eFloat64:
    return Const{func, double(2)};
  default:
    return Const{kind, uint16_t(2)};
  }
}

Const MinusOne(Func &func, Kind kind) noexcept {
  return Const{func, Value{int64_t(-1)}.cast(kind)};
}

const Fmt &Const::format(const Fmt &out, size_t /*depth*/) const {
  return out << imm();
}

} // namespace onejit
