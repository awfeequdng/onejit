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
 * binaryexpr.hpp
 *
 *  Created on Jan 16, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_BINARYEXPR_HPP
#define ONEJIT_BINARYEXPR_HPP

#include <onejit/expr.hpp>
#include <onejit/op2.hpp>

#include <iosfwd>

namespace onejit {

// a binary expression: Op2 and two arguments
class BinaryExpr : public Expr {
  using Base = Expr;

  friend class Func;
  friend std::ostream &operator<<(std::ostream &out, const Node &node);

public:
  constexpr BinaryExpr() : Base{NodeHeader{BAD, Void, 0}, BAD2, nullptr} {
  }

  constexpr Type type() const {
    return UNARY;
  }

  using Base::kind;

  constexpr uint16_t children() const {
    return 2;
  }

  constexpr Op2 op() const {
    return Op2(Base::op_or_children());
  }

private:
  constexpr explicit BinaryExpr(const Node &node) : Base{node} {
  }

  constexpr BinaryExpr(NodeHeader header, Offset offset_or_direct, Code *code) //
      : Base{header, offset_or_direct, code} {
  }

  static BinaryExpr create(Kind kind, const Expr &left, Op2 op, const Expr &right, Code *holder);
};

std::ostream &operator<<(std::ostream &out, const BinaryExpr &be);

} // namespace onejit

#endif // ONEJIT_BINARYEXPR_HPP
