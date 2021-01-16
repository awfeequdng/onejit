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

public:
  constexpr Expr() : Base{} {
  }

protected:
  constexpr explicit Expr(const Node &node) : Base{node} {
  }

  constexpr Expr(NodeHeader header, Offset offset_or_direct, Code *code) //
      : Base{header, offset_or_direct, code} {
  }
};

} // namespace onejit

#endif // ONEJIT_EXPR_HPP
