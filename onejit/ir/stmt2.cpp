/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * stmt2.cpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/expr.hpp>
#include <onejit/ir/stmt2.hpp>

namespace onejit {
namespace ir {

// ============================  Stmt2  ========================================

ONEJIT_NOINLINE Node Stmt2::create(Func &func, Node child0, Node child1, OpStmt2 op) noexcept {
  return Base::create_indirect(func,                               //
                               Header{STMT_2, Void, uint16_t(op)}, //
                               {child0, child1});
}

const Fmt &Stmt2::format(const Fmt &fmt, Syntax syntax, size_t depth) const {
  fmt << '(' << op() << ' ';
  if (op() != DEFAULT) {
    child(0).format(fmt, syntax, depth + 1) << ' ';
  }
  return child(1).format(fmt, syntax, depth + 1) << ')';
}

// ============================  Assign  ===================================

// ============================  Case  =====================================

// ============================  Default  ==================================

// ============================  JumpIf  ==================================

} // namespace ir
} // namespace onejit
