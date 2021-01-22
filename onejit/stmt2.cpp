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

#include <onejit/code.hpp>
#include <onejit/expr.hpp>
#include <onejit/stmt2.hpp>

namespace onejit {

// ============================  Stmt2  ========================================

Stmt2 ONEJIT_NOINLINE Stmt2::create(OpStmt2 op, const Nodes children, Code *holder) noexcept {
  while (holder && children.size() == 2) {
    const NodeHeader header{STMT_2, Void, uint16_t(op)};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add(children, offset)) {
      return Stmt2{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return Stmt2{op};
}

std::ostream &operator<<(std::ostream &out, const Stmt2 &st) {
  out << '(' << st.op();
  if (st.op() != DEFAULT) {
    out << ' ' << st.child(0);
  }
  return out << ' ' << st.child(1) << ')';
}

// ============================  CaseStmt  =====================================

CaseStmt CaseStmt::create(const Expr &expr, const Node &body, Code *holder) noexcept {
  Node children[] = {expr, body};
  return CaseStmt{Stmt2::create(CASE, Nodes{children, 2}, holder)};
}

// ============================  DefaultStmt  ==================================

DefaultStmt DefaultStmt::create(const Node &body, Code *holder) noexcept {
  Node children[] = {VoidExpr, body};
  return DefaultStmt{Stmt2::create(DEFAULT, Nodes{children, 2}, holder)};
}

} // namespace onejit
