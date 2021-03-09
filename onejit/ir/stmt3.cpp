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
