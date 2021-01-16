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
#include <onejit/op1.hpp>
#include <onejit/op2.hpp>
#include <onejit/unaryexpr.hpp>
#include <onejit/varexpr.hpp>
#include <onejit/vector.hpp>

namespace onejit {

class Func {

public:
  explicit Func(Code *holder);

  ConstExpr new_const(const Const &c) {
    return ConstExpr::create(c, holder_);
  }

  VarExpr new_var(Kind kind);

  UnaryExpr new_unary(Kind kind, Op1 op, const Expr &arg) {
    return UnaryExpr::create(kind, op, arg, holder_);
  }

  BinaryExpr new_binary(Kind kind, const Expr &left, Op2 op, const Expr &right) {
    return BinaryExpr::create(kind, left, op, right, holder_);
  }

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
