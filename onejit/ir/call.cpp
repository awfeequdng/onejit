/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * call.cpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/call.hpp>
#include <onejit/ir/functype.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/op.hpp>
#include <onestl/buffer.hpp>

namespace onejit {
namespace ir {

// ============================  Call  ====================================

// shortcut for child(i + 2).is<Expr>()
Expr Call::arg(uint32_t i) const noexcept {
  return child(add_uint32(2, i)).is<Expr>();
}

Node Call::create(Func &caller, const FuncType &ftype, const Expr &address, Exprs args) noexcept {
  const size_t n = args.size();
  Code *holder = caller.code();
  while (holder && n == uint32_t(n)) {
    const Header header{TUPLE, ftype.param_n() == 0 ? Void : ftype.param(0), CALL};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(add_uint32(2, n)) && //
        holder->add(ftype, offset) && holder->add(address, offset) &&  //
        holder->add(args, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Node{};
}

} // namespace ir
} // namespace onejit
