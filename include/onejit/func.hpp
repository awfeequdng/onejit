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
#include <onejit/fwd.hpp>
#include <onejit/op.hpp>
#include <onejit/stmt0.hpp>
#include <onejit/stmt1.hpp>
#include <onejit/stmt2.hpp>
#include <onejit/unaryexpr.hpp>
#include <onejit/varexpr.hpp>
#include <onestl/vector.hpp>

namespace onejit {

class Func {

public:
  explicit Func(Code *holder);

  constexpr Code *code() const {
    return holder_;
  }

  constexpr explicit operator bool() const {
    return holder_ && *holder_;
  }

  Break new_break() {
    return Break{};
  }

  BinaryExpr new_binary(Kind kind, const Expr &left, Op2 op, const Expr &right) {
    return BinaryExpr::create(kind, left, op, right, holder_);
  }

  Case new_case(const Expr &expr, const Node &body) {
    return Case::create(expr, body, holder_);
  }

  ConstExpr new_const(const Const &c) {
    return ConstExpr::create(c, holder_);
  }

  Continue new_continue() {
    return Continue{};
  }

  Default new_default(const Node &body) {
    return Default::create(body, holder_);
  }

  Fallthrough new_fallthrough() {
    return Fallthrough{};
  }

  Stmt0 new_stmt0(OpStmt0 op) {
    return Stmt0{op};
  }

  Stmt1 new_stmt1(OpStmt1 op, const Node &child) {
    return Stmt1::create(op, child, holder_);
  }

  Stmt2 new_stmt2(OpStmt2 op, const Node &child0, const Node &child1) {
    return Stmt2::create(op, child0, child1, holder_);
  }

  UnaryExpr new_unary(Kind kind, Op1 op, const Expr &arg) {
    return UnaryExpr::create(kind, op, arg, holder_);
  }

  VarExpr new_var(Kind kind);

  Func &add(const Node &node) {
    return add(node.offset_or_direct());
  }

private:
  Func &add(CodeItem item) {
    holder_->add(item);
    return *this;
  }

  Code *holder_;
  Vector<Var> vars_;
};

} // namespace onejit

#endif // ONEJIT_FUNC_HPP
