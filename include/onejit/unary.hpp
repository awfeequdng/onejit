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

#ifndef ONEJIT_UNARY_HPP
#define ONEJIT_UNARY_HPP

#include <onejit/expr.hpp>
#include <onejit/fmt.hpp>
#include <onejit/op.hpp>

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
   * to create a valid Unary, use one of the other constructors
   */
  constexpr Unary() noexcept : Base{} {
  }

  // also autodetects kind if op != CONVERT && op != BITCOPY
  Unary(Func &func, Op1 op, Expr child) noexcept //
      : Base{create(func, op, child)} {
  }

  // overrides Kind autodetection. needed by CONVERT and BITCOPY
  Unary(Func &func, Kind kind, Op1 op, Expr child) noexcept //
      : Base{create(func, kind, op, child)} {
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

  // shortcut for child_is<Expr>(0)
  Expr x() const noexcept {
    return child_is<Expr>(0);
  }

private:
  // downcast Node to Unary
  constexpr explicit Unary(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == UNARY;
  }

  // also autodetects kind if op != CONVERT && op != BITCOPY
  static Node create(Func &func, Op1 op, Expr child) noexcept;
  // overrides Kind autodetection. needed by CONVERT and BITCOPY
  static Node create(Func &func, Kind kind, Op1 op, Expr child) noexcept;
};

const Fmt &operator<<(const Fmt &out, const Unary &expr);

} // namespace onejit

#endif // ONEJIT_UNARY_HPP
