/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * stmt1.cpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/stmt1.hpp>

namespace onejit {
namespace ir {

// ============================  Stmt1  ========================================

ONEJIT_NOINLINE Node Stmt1::create(Func &func, Expr arg, OpStmt1 op) noexcept {
  return Base::create_indirect(func,                               //
                               Header{STMT_1, Void, uint16_t(op)}, //
                               Nodes{&arg, 1});
}

const Fmt &Stmt1::format(const Fmt &fmt, Syntax syntax, size_t depth) const {
  fmt << '(' << op() << ' ';
  return child(0).format(fmt, syntax, depth + 1) << ')';
}

// ============================  Goto  ==================================

// ============================  Inc  ===================================

// ============================  Dec  ===================================

} // namespace ir
} // namespace onejit
