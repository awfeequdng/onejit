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
 * binary.hpp
 *
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_BINARY_HPP
#define ONEJIT_NODE_BINARY_HPP

#include <onejit/node/expr.hpp>
#include <onejit/op.hpp>

namespace onejit {

// a binary expression: Op2 and two arguments
class Binary : public Expr {
  using Base = Expr;

  friend class Func;
  friend class Node;

public:
  /**
   * construct an invalid Binary.
   * exists only to allow placing Binary in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Binary, use one of the other constructors
   */
  constexpr Binary() noexcept : Base{} {
  }

  // also autodetects kind
  Binary(Func &func, Op2 op, const Expr &left, const Expr &right) noexcept
      : Base{create(func, op, left, right)} {
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

  // shortcut for child_is<Expr>(0)
  Expr x() const noexcept {
    return child_is<Expr>(0);
  }

  // shortcut for child_is<Expr>(1)
  Expr y() const noexcept {
    return child_is<Expr>(1);
  }

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

private:
  // downcast Node to Unary
  constexpr explicit Binary(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == BINARY;
  }

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    return true;
  }

  // also autodetects kind
  static Node create(Func &func, Op2 op, const Expr &left, const Expr &right);
};

} // namespace onejit

#endif // ONEJIT_NODE_BINARY_HPP
