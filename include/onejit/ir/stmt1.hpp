/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * stmt1.hpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_STMT1_HPP
#define ONEJIT_IR_STMT1_HPP

#include <onejit/fmt.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/stmt.hpp>
#include <onejit/mir/fwd.hpp>
#include <onejit/opstmt.hpp>
#include <onejit/x64/fwd.hpp>

namespace onejit {
namespace ir {

////////////////////////////////////////////////////////////////////////////////
class Stmt1 : public Stmt {
  using Base = Stmt;
  friend class Node;
  friend class ::onejit::Compiler;
  friend class ::onejit::Func;
  friend class ::onejit::Test;
  friend class x64::Compiler;
  friend class mir::Compiler;

public:
  /**
   * construct an invalid Stmt1
   * exists only to allow placing Stmt1 in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Stmt1, construct one of the derived classes
   */
  constexpr Stmt1() noexcept : Base{} {
  }

  constexpr OpStmt1 op() const noexcept {
    return OpStmt1(Base::op());
  }

  static constexpr Type type() noexcept {
    return STMT_1;
  }

  static constexpr uint32_t children() noexcept {
    return 1;
  }

  // shortcut for child_is<Expr>(0)
  Expr arg() const noexcept {
    return child_is<Expr>(0);
  }

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

protected:
  // downcast Node to Stmt1
  constexpr explicit Stmt1(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_1;
  }

  // used by subclasses and by Compiler::compile()
  Stmt1(Func &func, Expr arg, OpStmt1 op) noexcept : Base{create(func, arg, op)} {
  }

private:
  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    return true;
  }

  static Node create(Func &func, Expr arg, OpStmt1 op) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
class Goto : public Stmt1 {
  using Base = Stmt1;
  friend class Node;
  friend class ::onejit::Func;

public:
  /**
   * construct an invalid Goto
   * exists only to allow placing Goto in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Goto, use one of the other constructors.
   */
  constexpr Goto() noexcept : Base{} {
  }

  Goto(Func &func, Label target) noexcept //
      : Base{func, target, GOTO} {
  }

  static constexpr OpStmt1 op() noexcept {
    return GOTO;
  }

  // shortcut for child_is<Label>(0)
  Label to() const noexcept {
    return child_is<Label>(0);
  }

private:
  // downcast Node to Goto
  constexpr explicit Goto(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == GOTO;
  }
};

////////////////////////////////////////////////////////////////////////////////
// statement to increment an expression by one, i.e. expr++
class Inc : public Stmt1 {
  using Base = Stmt1;
  friend class Node;
  friend class ::onejit::Func;

public:
  /**
   * construct an invalid Inc
   * exists only to allow placing Inc in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Inc, use one of the other constructors.
   */
  constexpr Inc() noexcept : Base{} {
  }

  // expr must be Var or Mem
  Inc(Func &func, Expr expr) noexcept //
      : Base{func, expr, INC} {
  }

  static constexpr OpStmt1 op() noexcept {
    return INC;
  }

  // shortcut for child_is<Expr>(0)
  Expr expr() const noexcept {
    return child_is<Expr>(0);
  }

private:
  // downcast Node to Inc
  constexpr explicit Inc(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == INC;
  }
};

////////////////////////////////////////////////////////////////////////////////
// statement to decrement an expression by one, i.e. expr--
class Dec : public Stmt1 {
  using Base = Stmt1;
  friend class Node;
  friend class ::onejit::Func;

public:
  /**
   * construct an invalid Dec
   * exists only to allow placing Dec in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Dec, use one of the other constructors.
   */
  constexpr Dec() noexcept : Base{} {
  }

  // expr must be Var or Mem
  Dec(Func &func, Expr expr) noexcept //
      : Base{func, expr, DEC} {
  }

  static constexpr OpStmt1 op() noexcept {
    return DEC;
  }

  // shortcut for child_is<Expr>(0)
  Expr expr() const noexcept {
    return child_is<Expr>(0);
  }

private:
  // downcast Node to Dec
  constexpr explicit Dec(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == DEC;
  }
};

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_STMT1_HPP
