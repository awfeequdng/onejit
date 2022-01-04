/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * stmt3.hpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_STMT3_HPP
#define ONEJIT_IR_STMT3_HPP

#include <onejit/fmt.hpp>
#include <onejit/fwd.hpp>
#include <onejit/ir/const.hpp> // VoidConst
#include <onejit/ir/expr.hpp>
#include <onejit/ir/stmt.hpp>
#include <onejit/mir/fwd.hpp>
#include <onejit/opstmt3.hpp>

namespace onejit {
namespace ir {

////////////////////////////////////////////////////////////////////////////////
class Stmt3 : public Stmt {
  using Base = Stmt;
  friend class Node;
  friend class ::onejit::Compiler;
  friend class mir::Compiler;

public:
  /**
   * construct an invalid Stmt3.
   * exists only to allow placing Stmt3 in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Stmt3, use Func::new_stmt3()
   */
  constexpr Stmt3() noexcept : Base{} {
  }

  static constexpr Type type() noexcept {
    return STMT_3;
  }

  constexpr OpStmt3 op() const noexcept {
    return OpStmt3(Base::op());
  }

  static constexpr uint32_t children() noexcept {
    return 3;
  }

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

protected:
  // downcast Node to Stmt3
  constexpr explicit Stmt3(const Node &node) noexcept : Base{node} {
  }

  Stmt3(Func &func, Label to, Expr x, Expr y, OpStmt3 op) noexcept
      : Base{create(func, to, x, y, op)} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_3;
  }

  constexpr bool child_result_is_used(uint32_t i) const noexcept {
    return op() == IF ? i == 0 : true;
  }

  static Node create(Func &func, Node a, Node b, Node c, OpStmt3 op) noexcept;
  static Node create(Func &func, Nodes children, OpStmt3 op) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
class If : public Stmt3 {
  using Base = Stmt3;
  friend class ::onejit::Func;
  friend class Node;
  friend class Stmt3;

public:
  /**
   * construct an invalid If.
   * exists only to allow placing If in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid If, use one of the other constructors
   */
  constexpr If() noexcept : Base{} {
  }

  // create a new 'if (test) { then } else { else_ }'
  // the 'else' part can be omitted also by specifying else_ = VoidConst
  If(Func &func, const Expr &test, const Node &then, const Node &else_ = VoidConst) noexcept //
      : Base{create(func, test, then, else_)} {
  }

  static constexpr OpStmt3 op() noexcept {
    return IF;
  }

  // shortcut for child_is<Expr>(0)
  Expr test() const noexcept {
    return child_is<Expr>(0);
  }

  // shortcut for child(1)
  Node then() const noexcept {
    return child(1);
  }

  // shortcut for child(2)
  Node else_() const noexcept {
    return child(2);
  }

private:
  // downcast Node to If
  constexpr explicit If(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == IF;
  }

  static constexpr bool child_result_is_used(uint32_t i) noexcept {
    return i == 0;
  }

  // create a new If
  static Node create(Func &func, const Expr &test, const Node &then, const Node &else_) noexcept;
};

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_STMT3_HPP
