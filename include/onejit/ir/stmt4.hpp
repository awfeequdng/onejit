/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * stmt4.hpp
 *
 *  Created on Jan 19, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_STMT4_HPP
#define ONEJIT_IR_STMT4_HPP

#include <onejit/fmt.hpp>
#include <onejit/ir/expr.hpp>
#include <onejit/ir/stmt.hpp>
#include <onejit/opstmt.hpp>

namespace onejit {
namespace ir {

////////////////////////////////////////////////////////////////////////////////
class Stmt4 : public Stmt {
  using Base = Stmt;
  friend class Node;
  friend class ::onejit::Func;

public:
  /**
   * construct an invalid Stmt4.
   * exists only to allow placing Stmt4 in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Stmt4, use Func::new_stmt4()
   */
  constexpr Stmt4() noexcept : Base{} {
  }

  static constexpr Type type() noexcept {
    return STMT_4;
  }

  constexpr OpStmt4 op() const noexcept {
    return OpStmt4(Base::op());
  }

  static constexpr uint32_t children() noexcept {
    return 4;
  }

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

protected:
  // downcast Node to Stmt4
  constexpr explicit Stmt4(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_4;
  }

  // used by subclasses
  Stmt4(Func &func, const Node &child0, const Node &child1, //
        const Node &child2, const Node &child3, OpStmt4 op) noexcept
      : Base{create(func, child0, child1, child2, child3, op)} {
  }

private:
  constexpr bool child_result_is_used(uint32_t i) const noexcept {
    return op() == FOR ? i == 1 : true;
  }

  static Node create(Func &func, const Node &child0, const Node &child1, //
                     const Node &child2, const Node &child3, OpStmt4 op) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
class For : public Stmt4 {
  using Base = Stmt4;
  friend class Node;
  friend class ::onejit::Func;

public:
  /**
   * construct an invalid For.
   * exists only to allow placing For in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid For, use one of the other constructors
   */
  constexpr For() noexcept : Base{} {
  }

  For(Func &func, const Node &init, const Expr &test, const Node &post, const Node &body) noexcept
      : Base{func, init, test, post, body, FOR} {
  }

  static constexpr OpStmt4 op() noexcept {
    return FOR;
  }

  // shortcut for child(0)
  Node init() const noexcept {
    return child(0);
  }

  // shortcut for child_is<Expr>(1)
  Expr test() const noexcept {
    return child_is<Expr>(1);
  }

  // shortcut for child(2)
  Node post() const noexcept {
    return child(2);
  }

  // shortcut for child(3)
  Node body() const noexcept {
    return child(3);
  }

private:
  // downcast Node to For
  constexpr explicit For(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == FOR;
  }

  static constexpr bool child_result_is_used(uint32_t i) noexcept {
    return i == 1;
  }
};

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_STMT4_HPP
