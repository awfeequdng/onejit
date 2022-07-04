/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * tuple.hpp
 *
 *  Created on Jan 29, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_TUPLE_HPP
#define ONEJIT_IR_TUPLE_HPP

#include <onejit/ir/expr.hpp>
#include <onejit/op.hpp>
#include <onestl/view.hpp>

namespace onejit {
namespace ir {

////////////////////////////////////////////////////////////////////////////////
class Tuple : public Expr {
  using Base = Expr;
  friend class Call;
  friend class Comma;
  friend class ::onejit::Compiler;
  friend class ::onejit::Func;
  friend class Mem;
  friend class Node;
  friend class Optimizer;

public:
  /**
   * construct an invalid Tuple.
   * exists only to allow placing Tuple in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Tuple, use one of the other constructors
   */
  constexpr Tuple() noexcept : Base{} {
  }

  Tuple(Func &func, OpN op, Node x) noexcept //
      : Base{create(func, x.kind(), op, {x})} {
  }

  Tuple(Func &func, OpN op, Node x, Node y) noexcept //
      : Base{create(func, x.kind(), op, {x, y})} {
  }

  Tuple(Func &func, Kind kind, OpN op, Nodes nodes) noexcept //
      : Base{create(func, kind, op, nodes)} {
  }

  Tuple(Func &func, Kind kind, OpN op, const ChildRange &nodes) noexcept //
      : Base{create(func, kind, op, nodes)} {
  }

  Tuple(Func &func, Kind kind, OpN op, std::initializer_list<Node> nodes) noexcept //
      : Base{create(func, kind, op, nodes)} {
  }

  constexpr OpN op() const noexcept {
    return OpN(Base::op());
  }

  // shortcut for child_is<Expr>(i)
  Expr arg(uint32_t i) noexcept {
    return child_is<Expr>(i);
  }

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

private:
  // downcast Node to Tuple
  constexpr explicit Tuple(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == TUPLE || t == MEM;
  }

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    return true;
  }

  static uint16_t compose_side_effects(OpN op, Nodes nodes) noexcept;
  static uint16_t compose_side_effects(OpN op, const ChildRange &nodes) noexcept;

  static Node create(Func &func, Kind kind, OpN op, Nodes nodes) noexcept;

  static Node create(Func &func, Kind kind, OpN op, const ChildRange &nodes) noexcept;

  static Node create(Func &func, Kind kind, OpN op, std::initializer_list<Node> nodes) noexcept {
    return create(func, kind, op, Nodes{nodes.begin(), nodes.size()});
  }
};

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_TUPLE_HPP
