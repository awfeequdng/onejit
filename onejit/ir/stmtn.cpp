/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * stmtn.cpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/call.hpp>
#include <onejit/ir/expr.hpp>
#include <onejit/ir/stmt2.hpp> // Case
#include <onejit/ir/stmtn.hpp>
#include <onejit/space.hpp>
#include <onestl/chars.hpp>

namespace onejit {
namespace ir {

// ============================  StmtN  ========================================

Node StmtN::create(Func &func, const Nodes children, OpStmtN op) noexcept {
  return Base::create_indirect(func,                               //
                               Header{STMT_N, Void, uint16_t(op)}, //
                               children);
}

ONEJIT_NOINLINE Node StmtN::create(Func &func, const ChildRanges &children, OpStmtN op) noexcept {
  return Base::create_indirect_from_ranges(func,                               //
                                           Header{STMT_N, Void, uint16_t(op)}, //
                                           children);
}

const Fmt &StmtN::format(const Fmt &fmt, Syntax syntax, size_t depth) const {
  const OpStmtN op = this->op();

  fmt << '(' << op;
  ++depth;

  const bool is_call = op == X86_CALL_;

  // if op == X86_CALL_, skip child(0) i.e. FuncType
  for (size_t i = size_t(is_call), n = children(); i < n; i++) {
    if (op == BLOCK || op == COND || op == SWITCH) {
      fmt << '\n' << Space{depth * 4};
    } else {
      fmt << ' ';
    }
    child(i).format(fmt, syntax, depth);
  }
  return fmt << ')';
}

// ============================  AssignCall  ===================================

Node AssignCall::create(Func &func, Exprs assign_to, const Call &call) noexcept {
  const size_t n = assign_to.size();
  Code *holder = func.code();
  while (holder && n == uint32_t(n)) {
    const Header header{STMT_N, Void, ASSIGN_CALL};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(add_uint32(1, n)) &&
        holder->add(assign_to, offset) && holder->add(call, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Node{};
}

// ============================  Block  ====================================

// ============================  Cond  =====================================

// ============================  Return  ===================================

Node Return::create(Func &func, Exprs exprs) noexcept {
  return Node::create_indirect(func,                         //
                               Header{STMT_N, Void, RETURN}, //
                               Nodes{exprs.data(), exprs.size()});
}

// ============================  Switch  ===================================

// cases can contain at most one Default
Node Switch::create(Func &func, const Expr &expr, const Cases cases) noexcept {
  const size_t n = cases.size();
  Code *holder = func.code();
  while (holder && n == uint32_t(n)) {
    const Header header{STMT_N, Void, SWITCH};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(add_uint32(1, n)) && //
        holder->add(expr, offset) && holder->add(cases, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Node{};
}

Case Switch::case_(uint32_t i) const noexcept {
  return child_is<Case>(add_uint32(1, i));
}

} // namespace ir
} // namespace onejit
