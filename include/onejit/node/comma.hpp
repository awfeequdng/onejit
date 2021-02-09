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
 * comma.hpp
 *
 *  Created on Feb 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_COMMA_HPP
#define ONEJIT_NODE_COMMA_HPP

#include <onejit/node/tuple.hpp>
#include <onestl/view.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// evaluate N-1 expressions only for their side effects,
// then return result of last expression.
class Comma : public Tuple {
  using Base = Tuple;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Comma.
   * exists only to allow placing Comma in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Comma, use one of the other constructors
   */
  constexpr Comma() noexcept : Base{} {
  }

  Comma(Func &func, Exprs args) noexcept : Base{create(func, args)} {
  }

  static constexpr OpN op() noexcept {
    return COMMA;
  }

  // shortcut for child_is<Expr>(i)
  Expr arg(uint32_t i) const noexcept {
    return child_is<Expr>(i);
  }

private:
  // downcast Node to Comma
  constexpr explicit Comma(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == COMMA;
  }

  static Node create(Func &func, Exprs args) noexcept;
};

} // namespace onejit

#endif // ONEJIT_NODE_COMMA_HPP
