/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * const.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/const.hpp>

namespace onejit {
namespace ir {

Imm Const::imm() const noexcept {
  if (is_direct()) {
    return Imm::parse_direct(offset_or_direct());
  } else {
    return Imm::parse_indirect(kind(), offset_or_direct() + sizeof(CodeItem), code());
  }
}

Node Const::create(Func &func, const Imm &imm) noexcept {
  if (imm.is_valid()) {
    const Header header{CONST, imm.kind(), 0};

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
  return Const{func, Value{-1}.cast(kind)};
}

const Fmt &Const::format(const Fmt &fmt, Syntax /*syntax*/, size_t /*depth*/) const {
  return fmt << imm();
}

} // namespace ir
} // namespace onejit
