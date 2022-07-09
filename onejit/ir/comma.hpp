/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * comma.hpp
 *
 *  Created on Feb 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_COMMA_HPP
#define ONEJIT_IR_COMMA_HPP

#include <onejit/ir/tuple.hpp>
#include <onestl/view.hpp>

namespace onejit {
namespace ir {

////////////////////////////////////////////////////////////////////////////////
// evaluate N-1 expressions only for their side effects,
// then return result of last expression.
class Comma : public Tuple {
  using Base = Tuple;
  friend class Node;
  friend class ::onejit::Func;

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

  Comma(Func &func, std::initializer_list<Expr> args) noexcept
      : Base{create(func, Exprs{args.begin(), args.size()})} {
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
    return OpN(op) == COMMA;
  }

  static Node create(Func &func, Exprs args) noexcept;
};

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_COMMA_HPP
