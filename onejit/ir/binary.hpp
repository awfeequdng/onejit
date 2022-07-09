/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * binary.hpp
 *
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_BINARY_HPP
#define ONEJIT_IR_BINARY_HPP

#include <onejit/ir/expr.hpp>
#include <onejit/op.hpp>

namespace onejit {
namespace ir {

// a binary expression: Op2 and two arguments
class Binary : public Expr {
  using Base = Expr;

  friend class ::onejit::Func;
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

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_BINARY_HPP
