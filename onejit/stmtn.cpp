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

#include <onejit/call.hpp> // Call
#include <onejit/code.hpp>
#include <onejit/expr.hpp>
#include <onejit/func.hpp>
#include <onejit/stmt2.hpp> // Case
#include <onejit/stmtn.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// ============================  StmtN  ========================================

Node StmtN::create(Func &func, const Nodes nodes, OpStmtN op) noexcept {
  Code *holder = func.code();
  const size_t n = nodes.size();
  while (holder && n == uint32_t(n)) {
    const NodeHeader header{STMT_N, Void, uint16_t(op)};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(n) && holder->add(nodes, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return StmtN{};
}

const Fmt &operator<<(const Fmt &out, const StmtN &st) {
  const OpStmtN op = st.op();
  out << '(' << op;
  Chars separator = " ";
  if (op == BLOCK || op == COND || op == SWITCH) {
    separator = "\n    ";
  }
  for (size_t i = 0, n = st.children(); i < n; i++) {
    out << separator << st.child(i);
  }
  return out << ')';
}

// ============================  AssignCall  ===================================

Node AssignCall::create(Func &func, Exprs assign_to, const Call &call) noexcept {
  const size_t n = assign_to.size();
  Code *holder = func.code();
  while (holder && n == uint32_t(n)) {
    const NodeHeader header{STMT_N, Void, ASSIGN_CALL};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(sum_uint32(1, n)) &&
        holder->add(assign_to, offset) && holder->add(call, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return AssignCall{};
}

// ============================  Block  ====================================

// ============================  Cond  =====================================

// ============================  Return  ===================================

Node Return::create(Func &func, Exprs exprs) noexcept {
  const size_t n = exprs.size();
  Code *holder = func.code();
  while (holder && n == uint32_t(n)) {
    const NodeHeader header{STMT_N, Void, RETURN};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(n) && holder->add(exprs, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Return{};
}

// ============================  Switch  ===================================

// cases can contain at most one Default
Node Switch::create(Func &func, const Expr &expr, const Cases cases) noexcept {
  const size_t n = cases.size();
  Code *holder = func.code();
  while (holder && n == uint32_t(n)) {
    const NodeHeader header{STMT_N, Void, SWITCH};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(sum_uint32(1, n)) && //
        holder->add(expr, offset) && holder->add(cases, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Switch{};
}

} // namespace onejit
