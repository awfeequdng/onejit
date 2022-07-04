/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * unary.cpp
 *
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/unary.hpp>

namespace onejit {
namespace ir {

ONEJIT_NOINLINE Node Unary::create(Func &func, Kind kind, Op1 op, Expr child) noexcept {
  return Base::create_indirect(func,                                                     //
                               Header{UNARY, kind, uint16_t(op | child.side_effects())}, //
                               {child});
}

ONEJIT_NOINLINE Node Unary::create(Func &func, Op1 op, Expr child) noexcept {
  Kind kind;
  if (op == NOT1) {
    kind = Bool;
  } else if (op <= BITCOPY) {
    // CAST and BITCOPY kind should be specified manually
    kind = child.kind();
  } else {
    kind = Bad;
  }
  return create(func, kind, op, child);
}

const Fmt &Unary::format(const Fmt &fmt, Syntax syntax, size_t depth) const {
  Op1 op = this->op();
  fmt << '(' << op;
  if (op == CAST || op == BITCOPY) {
    fmt << ' ' << kind();
  }
  fmt << ' ';
  return x().format(fmt, syntax, depth + 1) << ')';
}

} // namespace ir
} // namespace onejit
