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
 * stmt2.cpp
 *
 *  Created on Jan 18, 2020
 *      Author Massimiliano Ghilardi
 */

#include "onejit/stmt2.hpp"
#include "onejit/code.hpp"
#include "onejit/expr.hpp"

namespace onejit {

// ============================  Stmt2  ========================================

Stmt2 Stmt2::create(OpStmt2 op, const Node &child0, const Node &child1, Code *holder) {
  const NodeHeader header{STMT_2, Void, uint16_t(op)};
  CodeItem offset = holder->length();

  if (!holder->add(header) || !holder->add(child0, offset) || !holder->add(child1, offset)) {
    holder->truncate(offset);
    return Stmt2{};
  }
  return Stmt2{header, offset, holder};
}

std::ostream &operator<<(std::ostream &out, const Stmt2 &st2) {
  return out << '(' << st2.op() << ' ' << st2.child(0) << ' ' << st2.child(1) << ')';
}

// ============================  Case  ======================================

Case Case::create(const Expr &expr, const Node &body, Code *holder) {
  return Stmt2::create(CASE, expr, body, holder).is<Case>();
}

} // namespace onejit
