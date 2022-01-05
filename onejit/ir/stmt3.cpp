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

ONEJIT_NOINLINE Node Stmt3::create(Func &func, OpStmt3 op, Node child0, Node child1,
                                   Node child2) noexcept {
  return Base::create_indirect(func,                               //
                               Header{STMT_3, Void, uint16_t(op)}, //
                               {child0, child1, child2});
}

ONEJIT_NOINLINE Node Stmt3::create(Func &func, OpStmt3 op, Nodes children) noexcept {
  return Base::create_indirect(func,                               //
                               Header{STMT_3, Void, uint16_t(op)}, //
                               children);
}

const Fmt &Stmt3::format(const Fmt &fmt, Syntax syntax, size_t depth) const {
  fmt << '(' << op();
  const bool newline = op() <= IF;
  ++depth;
  for (size_t i = 0; i < 3; i++) {
    if (newline && i != 0) {
      fmt << '\n' << Space{depth * 4};
    } else {
      fmt << ' ';
    }
    child(i).format(fmt, syntax, depth);
  }
  return fmt << ')';
}

} // namespace ir
} // namespace onejit
