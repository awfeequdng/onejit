/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * expr.hpp
 *
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_EXPR_HPP
#define ONEJIT_IR_EXPR_HPP

#include <onejit/ir/node.hpp>
#include <onejit/op.hpp>

namespace onejit {
namespace ir {

// base class of Binary, Const, Label, Mem, Tuple, Unary, Var
class Expr : public Node {
  using Base = Node;

  friend class Node;

public:
  /**
   * construct an invalid Expr.
   * exists only to allow placing Expr in containers
   * and similar uses that require a default constructor.
   *
   * to construct a valid Expr, use one of the subclasses constructors
   */
  constexpr Expr() noexcept : Base{} {
  }

  SideEffects side_effects() const noexcept {
    return SideEffects(Base::op() & 0xF000);
  }

protected:
  // downcast Node to Expr
  constexpr explicit Expr(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t >= VAR && t <= CONST;
  }
};

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_EXPR_HPP
