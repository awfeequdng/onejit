/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * stmt4.cpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/stmt4.hpp>
#include <onejit/space.hpp>
#include <onestl/chars.hpp>

namespace onejit {
namespace ir {

// ============================  Stmt4  ========================================

ONEJIT_NOINLINE Node Stmt4::create(Func &func, OpStmt4 op, const Node &child0, const Node &child1,
                                   const Node &child2, const Node &child3) noexcept {
  return Base::create_indirect(func,                               //
                               Header{STMT_4, Void, uint16_t(op)}, //
                               {child0, child1, child2, child3});
}

const Fmt &Stmt4::format(const Fmt &fmt, Syntax syntax, size_t depth) const {
  ++depth;
  fmt << '(' << op() << ' ';
  child(0).format(fmt, syntax, depth) << ' ';
  child(1).format(fmt, syntax, depth) << ' ';
  child(2).format(fmt, syntax, depth);
  if (op() == FOR) {
    fmt << '\n' << Space{depth * 4};
  } else {
    fmt << ' ';
  }
  child(3).format(fmt, syntax, depth) << ')';
  return fmt;
}

// ============================  For  =======================================

} // namespace ir
} // namespace onejit
