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
 * unary.hpp
 *
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_UNARYEXPR_HPP
#define ONEJIT_UNARYEXPR_HPP

#include <onejit/expr.hpp>
#include <onejit/op.hpp>

#include <iosfwd>

namespace onejit {

// an unary expression: Op1 and a single argument
class Unary : public Expr {
  using Base = Expr;

  friend class Func;
  friend class Node;

public:
  /**
   * construct an invalid Unary.
   * exists only to allow placing Unary in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Unary, use Func::new_unary()
   */
  constexpr Unary() noexcept : Base{UNARY} {
  }

  static constexpr Type type() noexcept {
    return UNARY;
  }

  using Base::kind;

  constexpr Op1 op() const noexcept {
    return Op1(Base::op());
  }

  static constexpr uint32_t children() noexcept {
    return 1;
  }

  // shortcut for child(0).is<Expr>()
  Expr x() const noexcept {
    return child(0).is<Expr>();
  }

private:
  // downcast Node to Unary
  constexpr explicit Unary(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == UNARY;
  }

  static Unary create(Kind kind, Op1 op, const Expr &child, Code *holder) noexcept;
  // also autodetects kind if op != CAST
  static Unary create(Op1 op, const Expr &child, Code *holder) noexcept;
};

std::ostream &operator<<(std::ostream &out, const Unary &expr);

} // namespace onejit

#endif // ONEJIT_UNARYEXPR_HPP
