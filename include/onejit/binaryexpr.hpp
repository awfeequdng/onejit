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
  friend class Node;

public:
  constexpr BinaryExpr() : Base{NodeHeader{BAD, Void, 0}, BAD2, nullptr} {
  }

  static constexpr Type type() {
    return UNARY;
  }

  using Base::kind;

  constexpr Op2 op() const {
    return Op2(header().op_or_children());
  }

  static constexpr uint16_t children() {
    return 2;
  }

  // shortcut for child(0).is<Expr>()
  Expr x() const {
    return child(0).is<Expr>();
  }

  // shortcut for child(1).is<Expr>()
  Expr y() const {
    return child(1).is<Expr>();
  }

private:
  constexpr BinaryExpr(NodeHeader header, Offset offset_or_direct, Code *code) //
      : Base{header, offset_or_direct, code} {
  }

  // downcast Node to UnaryExpr
  constexpr explicit BinaryExpr(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) {
    return t == BINARY;
  }

  static BinaryExpr create(Kind kind, const Expr &left, Op2 op, const Expr &right, Code *holder);
};

std::ostream &operator<<(std::ostream &out, const BinaryExpr &be);

} // namespace onejit

#endif // ONEJIT_BINARYEXPR_HPP
