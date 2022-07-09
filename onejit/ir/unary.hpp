/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * unary.hpp
 *
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_UNARY_HPP
#define ONEJIT_IR_UNARY_HPP

#include <onejit/fmt.hpp>
#include <onejit/ir/expr.hpp>
#include <onejit/op.hpp>

namespace onejit {
namespace ir {

// an unary expression: Op1 and a single argument
class Unary : public Expr {
  using Base = Expr;

  friend class ::onejit::Func;
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

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

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

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_UNARY_HPP
