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
#include <onejit/compiler.hpp>
#include <onejit/constexpr.hpp>
#include <onejit/func.hpp>
#include <onejit/stmt3.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// ============================  Stmt3  ========================================

Stmt3 ONEJIT_NOINLINE Stmt3::create(OpStmt3 op, Nodes children, Code *holder) noexcept {
  while (holder && children.size() == 3) {
    const NodeHeader header{STMT_3, Void, uint16_t(op)};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add(children, offset)) {
      return Stmt3{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return Stmt3{op};
}

Node Stmt3::compile(Compiler &comp, bool parent_is_expr) const noexcept {
  if (const IfStmt st = is<IfStmt>()) {
    return st.compile(comp, parent_is_expr);
  }
  /// TODO: implement
  comp.add(*this);
  return VoidExpr;
}

std::ostream &operator<<(std::ostream &out, const Stmt3 &st) {
  return out << '(' << st.op() << ' ' << st.child(0) << Chars("\n    ") << st.child(1)
             << Chars("\n    ") << st.child(2) << ')';
}

// ============================  IfStmt  =======================================

IfStmt IfStmt::create(const Expr &cond, const Node &then, const Node &else_,
                      Code *holder) noexcept {
  const Node buf[] = {cond, then, else_};
  return IfStmt{Stmt3::create(IF, Nodes{buf, 3}, holder)};
}

Node IfStmt::compile(Compiler &comp, bool) const noexcept {
  Func &func = comp.func();

  Expr cond = func.new_unary(NOT1, this->cond());
  cond = cond.compile(comp, false);

  Node then = this->then();
  Node else_ = this->else_();
  bool have_else = else_.type() != CONST;

  Label else_label = have_else ? func.new_label() : Label{};
  Label endif_label = func.new_label();

  JumpIfStmt jump_if = func.new_jump_if(have_else ? else_label : endif_label, cond);

  comp.compile_add(jump_if, false) //
      .compile_add(then, false);
  if (have_else) {
    comp.compile_add(func.new_goto(endif_label), false) //
        .compile_add(else_label, false)
        .compile_add(else_, false);
  }
  comp.compile_add(endif_label, false);
  return VoidExpr;
}

} // namespace onejit
