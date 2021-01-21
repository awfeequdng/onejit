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
 * func.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FUNC_HPP
#define ONEJIT_FUNC_HPP

#include <onejit/binaryexpr.hpp>
#include <onejit/code.hpp>
#include <onejit/constexpr.hpp>
#include <onejit/functype.hpp>
#include <onejit/label.hpp>
#include <onejit/memexpr.hpp>
#include <onejit/op.hpp>
#include <onejit/stmt0.hpp>
#include <onejit/stmt1.hpp>
#include <onejit/stmt2.hpp>
#include <onejit/stmt3.hpp>
#include <onejit/stmt4.hpp>
#include <onejit/stmtn.hpp>
#include <onejit/unaryexpr.hpp>
#include <onejit/varexpr.hpp>
#include <onestl/vector.hpp>

namespace onejit {

class Func {
public:
  /**
   * construct an invalid Func.
   * exists only to allow placing Func in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Func, use one of the other constructors
   */
  constexpr Func() noexcept : holder_{}, ftype_{}, labels_{}, vars_{}, body_{} {
  }

  Func(const FuncType &ftype, Code *holder) noexcept;

  constexpr Code *code() const noexcept {
    return holder_;
  }

  constexpr explicit operator bool() const noexcept {
    return holder_ && *holder_;
  }

  // convert Func to Label
  Label label() const noexcept;

  BreakStmt new_break() noexcept {
    return BreakStmt{};
  }

  // also autodetects kind
  BinaryExpr new_binary(Op2 op, const Expr &left, const Expr &right) noexcept {
    return BinaryExpr::create(op, left, right, holder_);
  }

  BlockStmt new_block(Nodes nodes) noexcept {
    return BlockStmt::create(nodes, holder_);
  }

  CaseStmt new_case(const Expr &expr, const Node &body) noexcept {
    return CaseStmt::create(expr, body, holder_);
  }

  CondStmt new_cond(Nodes nodes) noexcept {
    return CondStmt::create(nodes, holder_);
  }

  ConstExpr new_const(const Const &c) noexcept {
    return ConstExpr::create(c, holder_);
  }

  ContinueStmt new_continue() noexcept {
    return ContinueStmt{};
  }

  DefaultStmt new_default(const Node &body) noexcept {
    return DefaultStmt::create(body, holder_);
  }

  FallthroughStmt new_fallthrough() noexcept {
    return FallthroughStmt{};
  }

  ForStmt new_for(const Node &init, const Expr &cond, const Node &post, const Node &body) noexcept {
    return ForStmt::create(init, cond, post, body, holder_);
  }

  IfStmt new_if(const Expr &cond, const Node &then, const Node &else_) noexcept {
    return IfStmt::create(cond, then, else_, holder_);
  }

  Label new_label() noexcept;

  MemExpr new_mem(Kind kind, const Expr &address) noexcept {
    return MemExpr::create(kind, address, holder_);
  }

  Stmt0 new_stmt0(OpStmt0 op) noexcept {
    return Stmt0{op};
  }

  Stmt1 new_stmt1(OpStmt1 op, const Node &child) noexcept {
    return Stmt1::create(op, child, holder_);
  }

  Stmt2 new_stmt2(OpStmt2 op, const Node &child0, const Node &child1) noexcept {
    return Stmt2::create(op, child0, child1, holder_);
  }

  Stmt3 new_stmt3(OpStmt3 op, const Node &child0, const Node &child1, const Node &child2) noexcept {
    return Stmt3::create(op, child0, child1, child2, holder_);
  }

  Stmt4 new_stmt4(OpStmt4 op, const Node &child0, const Node &child1, const Node &child2,
                  const Node &child3) noexcept {
    return Stmt4::create(op, child0, child1, child2, child3, holder_);
  }

  StmtN new_stmtn(OpStmtN op, Nodes nodes) noexcept {
    return StmtN::create(op, nodes, holder_);
  }

  // node[0] must be Expr. other nodes must be CaseStmt, plus at most one DefaultStmt
  SwitchStmt new_switch(Nodes nodes) noexcept {
    return SwitchStmt::create(nodes, holder_);
  }

  UnaryExpr new_unary(Kind kind, Op1 op, const Expr &arg) noexcept {
    return UnaryExpr::create(kind, op, arg, holder_);
  }

  // also autodetects kind if op != CAST
  UnaryExpr new_unary(Op1 op, const Expr &arg) noexcept {
    return UnaryExpr::create(op, arg, holder_);
  }

  VarExpr new_var(Kind kind) noexcept;

  constexpr Node get_body() const noexcept {
    return body_;
  }

  Func &set_body(const Node &body) noexcept {
    body_ = body;
    return *this;
  }

private:
  Func &add(CodeItem item) noexcept {
    if (holder_) {
      holder_->add(item);
    }
    return *this;
  }

  Code *holder_;
  FuncType ftype_;
  Vector<Label> labels_;
  Vector<Var> vars_;
  Node body_;
};

} // namespace onejit

#endif // ONEJIT_FUNC_HPP
