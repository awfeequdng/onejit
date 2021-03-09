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

ONEJIT_NOINLINE Node Stmt4::create(Func &func, const Node &child0, const Node &child1,
                                   const Node &child2, const Node &child3, OpStmt4 op) noexcept {
  return Base::create_indirect(func,                               //
                               Header{STMT_4, Void, uint16_t(op)}, //
                               {child0, child1, child2, child3});
}

const Fmt &Stmt4::format(const Fmt &fmt, Syntax syntax, size_t depth) const {
  ++depth;
  fmt << '(' << op() << ' ';
  child(0).format(fmt, syntax, depth) << ' ';
  child(1).format(fmt, syntax, depth) << ' ';
  child(2).format(fmt, syntax, depth) << '\n' << Space{depth * 4};
  child(3).format(fmt, syntax, depth) << ')';
  return fmt;
}

// ============================  For  =======================================

} // namespace ir
} // namespace onejit
