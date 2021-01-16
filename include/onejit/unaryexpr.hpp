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
 * unaryexpr.hpp
 *
 *  Created on Jan 16, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_UNARYEXPR_HPP
#define ONEJIT_UNARYEXPR_HPP

#include <onejit/expr.hpp>
#include <onejit/op1.hpp>

#include <iosfwd>

namespace onejit {

// an unary expression: Op and a single argument
class UnaryExpr : public Expr {
  using Base = Expr;

  friend class Func;
  friend std::ostream &operator<<(std::ostream &out, const Node &node);

public:
  constexpr UnaryExpr() : Base{NodeHeader{BAD, Void, 0}, BAD1, nullptr} {
  }

  constexpr Type type() const {
    return UNARY;
  }

  using Base::kind;

  constexpr uint16_t children() const {
    return 1;
  }

  constexpr Op1 op() const {
    return Op1(Base::op_or_children());
  }

private:
  constexpr explicit UnaryExpr(const Node &node) : Base{node} {
  }

  constexpr UnaryExpr(NodeHeader header, Offset offset_or_direct, Code *code) //
      : Base{header, offset_or_direct, code} {
  }

  static UnaryExpr create(Kind kind, Op1 op, const Expr &child, Code *holder);
};

std::ostream &operator<<(std::ostream &out, const UnaryExpr &ue);

} // namespace onejit

#endif // ONEJIT_VAR_HPP
