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
 * expr.hpp
 *
 *  Created on Jan 16, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_EXPR_HPP
#define ONEJIT_EXPR_HPP

#include <onejit/node.hpp>

namespace onejit {

// base class of BinaryExpr, ConstExpr, UnaryExpr, VarExpr
class Expr : public Node {
  using Base = Node;

  friend class Node;

public:
  constexpr Expr() : Base{} {
  }

protected:
  /* construct and invalid Expr */
  constexpr explicit Expr(const Type t) : Base{NodeHeader{t, Bad, 0}, 0, nullptr} {
  }

  // downcast Node to Expr
  constexpr explicit Expr(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) {
    return t >= VAR && t <= CONST;
  }
};

} // namespace onejit

#endif // ONEJIT_EXPR_HPP
