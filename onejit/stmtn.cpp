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
 * stmtn.cpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/compiler.hpp>
#include <onejit/expr.hpp>
#include <onejit/stmt2.hpp> // CaseStmt
#include <onejit/stmtn.hpp>
#include <onejit/tupleexpr.hpp> // CallExpr
#include <onestl/chars.hpp>

namespace onejit {

// ============================  StmtN  ========================================

StmtN StmtN::create(OpStmtN op, const Nodes nodes, Code *holder) noexcept {
  const size_t n = nodes.size();
  while (holder && n == uint32_t(n)) {
    const NodeHeader header{STMT_N, Void, uint16_t(op)};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(n) && holder->add(nodes, offset)) {
      return StmtN{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return StmtN{op};
}

Compiler &StmtN::compile(Compiler &comp) const noexcept {
  switch (op()) {
  case ASSIGN_TUPLE:
    return is<AssignStmt>().compile(comp);
  case BLOCK:
    return is<BlockStmt>().compile(comp);
  case COND:
    return is<CondStmt>().compile(comp);
  case RETURN:
    return is<ReturnStmt>().compile(comp);
  case SWITCH:
    return is<SwitchStmt>().compile(comp);
  default:
    return comp.add(*this);
  }
}

std::ostream &operator<<(std::ostream &out, const StmtN &st) {
  const OpStmtN op = st.op();
  out << '(' << op;
  Chars separator = " ";
  if (op == BLOCK || op == SWITCH) {
    separator = "\n    ";
  }
  for (size_t i = 0, n = st.children(); i < n; i++) {
    out << separator << st.child(i);
  }
  return out << ')';
}

// ============================  AssignStmt  ===================================

AssignStmt AssignStmt::create(Exprs assign_to, const CallExpr &call, Code *holder) noexcept {
  const size_t n = assign_to.size();
  while (holder && n == uint32_t(n)) {
    const NodeHeader header{STMT_N, Void, ASSIGN_TUPLE};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(sum_uint32(1, n)) &&
        holder->add(assign_to, offset) && holder->add(call, offset)) {
      return AssignStmt{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return AssignStmt{};
}

Compiler &AssignStmt::compile(Compiler &comp) const noexcept {
  /// TODO: implement
  return comp.add(*this);
}

// ============================  BlockStmt  ====================================

Compiler &BlockStmt::compile(Compiler &comp) const noexcept {
  for (size_t i = 0, n = children(); i < n; i++) {
    child(i).compile(comp);
  }
  return comp;
}

// ============================  CondStmt  =====================================

Compiler &CondStmt::compile(Compiler &comp) const noexcept {
  /// TODO: implement
  return comp.add(*this);
}

// ============================  ReturnStmt  ===================================

ReturnStmt ReturnStmt::create(Exprs exprs, Code *holder) noexcept {
  const size_t n = exprs.size();
  while (holder && n == uint32_t(n)) {
    const NodeHeader header{STMT_N, Void, RETURN};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(n) && holder->add(exprs, offset)) {
      return ReturnStmt{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return ReturnStmt{};
}

Compiler &ReturnStmt::compile(Compiler &comp) const noexcept {
  /// TODO: implement
  return comp.add(*this);
}

// ============================  SwitchStmt  ===================================

// cases can contain at most one DefaultStmt
SwitchStmt SwitchStmt::create(const Expr &expr, const CaseStmts cases, Code *holder) noexcept {
  const size_t n = cases.size();
  while (holder && n == uint32_t(n)) {
    const NodeHeader header{STMT_3, Void, SWITCH};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(sum_uint32(1, n)) && //
        holder->add(expr, offset) && holder->add(cases, offset)) {
      return SwitchStmt{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return SwitchStmt{};
}

Compiler &SwitchStmt::compile(Compiler &comp) const noexcept {
  /// TODO: implement
  return comp.add(*this);
}

} // namespace onejit
