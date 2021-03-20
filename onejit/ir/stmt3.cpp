/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * stmt3.cpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/stmt3.hpp>
#include <onejit/space.hpp>
#include <onestl/chars.hpp>

namespace onejit {
namespace ir {

// ============================  Stmt3  ========================================

ONEJIT_NOINLINE Node Stmt3::create(Func &func, Nodes children, OpStmt3 op) noexcept {
  return Base::create_indirect(func,                               //
                               Header{STMT_3, Void, uint16_t(op)}, //
                               children);
}

const Fmt &Stmt3::format(const Fmt &fmt, Syntax syntax, size_t depth) const {
  ++depth;
  fmt << '(' << op() << ' ';
  child(0).format(fmt, syntax, depth) << '\n' << Space{depth * 4};
  child(1).format(fmt, syntax, depth) << '\n' << Space{depth * 4};
  child(2).format(fmt, syntax, depth) << ')';
  return fmt;
}

// ============================  If  =======================================

Node If::create(Func &func, const Expr &test, const Node &then, const Node &else_) noexcept {
  const Node buf[] = {test, then, else_};
  return Stmt3::create(func, Nodes{buf, 3}, IF);
}

} // namespace ir
} // namespace onejit
