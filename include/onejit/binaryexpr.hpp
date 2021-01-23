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
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_BINARYEXPR_HPP
#define ONEJIT_BINARYEXPR_HPP

#include <onejit/expr.hpp>
#include <onejit/op.hpp>

#include <iosfwd>

namespace onejit {

// a binary expression: Op2 and two arguments
class BinaryExpr : public Expr {
  using Base = Expr;

  friend class Func;
  friend class Node;

public:
  /**
   * construct an invalid BinaryExpr.
   * exists only to allow placing BinaryExpr in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid BinaryExpr, use Func::new_binary()
   */
  constexpr BinaryExpr() noexcept : Base{BINARY} {
  }

  static constexpr Type type() noexcept {
    return BINARY;
  }

  using Base::kind;

  constexpr Op2 op() const noexcept {
    return Op2(Base::op());
  }

  static constexpr uint32_t children() noexcept {
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

  Compiler &compile(Compiler &comp) const noexcept;

private:
  // downcast Node to UnaryExpr
  constexpr explicit BinaryExpr(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == BINARY;
  }

  // also autodetects kind
  static BinaryExpr create(Op2 op, const Expr &left, const Expr &right, Code *holder);
};

std::ostream &operator<<(std::ostream &out, const BinaryExpr &be);

} // namespace onejit

#endif // ONEJIT_BINARYEXPR_HPP
