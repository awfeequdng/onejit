/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * var.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/var.hpp>

namespace onejit {
namespace ir {

Var::Var(Func &func, Kind kind) noexcept : Var{func.new_var(kind)} {
}

Var Var::create(Code *holder, Local local) noexcept {
  const Header header{VAR, local.kind(), 0};
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

const Fmt &Var::format(const Fmt &fmt, Syntax /*syntax*/, size_t /*depth*/) const {
  return fmt << local();
}

} // namespace ir
} // namespace onejit
