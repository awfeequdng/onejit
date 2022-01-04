/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * stmt2.hpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_STMT2_HPP
#define ONEJIT_IR_STMT2_HPP

#include <onejit/fmt.hpp>
#include <onejit/ir/const.hpp> // VoidConst
#include <onejit/ir/label.hpp>
#include <onejit/ir/stmt.hpp>
#include <onejit/opstmt2.hpp>
#include <onejit/x64/fwd.hpp>
#include <onestl/view.hpp>

#include <initializer_list>

namespace onejit {
namespace ir {

////////////////////////////////////////////////////////////////////////////////
class Stmt2 : public Stmt {
  using Base = Stmt;
  friend class Default;
  friend class Node;
  friend class ::onejit::Compiler;
  friend class ::onejit::Func;
  friend class x64::Compiler;

public:
  /**
   * construct an invalid Stmt2.
   * exists only to allow placing Stmt2 in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Stmt2, use Func::new_stmt2()
   */
  constexpr Stmt2() noexcept : Base{} {
  }

  static constexpr Type type() noexcept {
    return STMT_2;
  }

  constexpr OpStmt2 op() const noexcept {
    return OpStmt2(Base::op());
  }

  static constexpr uint32_t children() noexcept {
    return 2;
  }

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

protected:
  // downcast Node to Stmt2
  constexpr explicit Stmt2(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_2;
  }

  // used by subclasses
  Stmt2(Func &func, Node child0, Node child1, OpStmt2 op) noexcept
      : Base{create(func, child0, child1, op)} {
  }

private:
  constexpr bool child_result_is_used(uint32_t i) const noexcept {
    return op() == CASE ? i == 0 : op() == DEFAULT ? false : true;
  }

  static Node create(Func &func, Node child0, Node child1, OpStmt2 op) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
class Assign : public Stmt2 {
  using Base = Stmt2;
  friend class Node;

public:
  /**
   * construct an invalid Assign.
   * exists only to allow placing Assign in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Assign, use one of the other constructors
   */
  constexpr Assign() noexcept : Base{} {
  }

  Assign(Func &func, OpStmt2 op, Expr dst, Expr src) noexcept //
      : Base{func, dst, src, op} {
  }

  constexpr OpStmt2 op() const noexcept {
    return OpStmt2(Base::op());
  }

  // shortcut for child_is<Expr>(0)
  Expr dst() const noexcept {
    return child_is<Expr>(0);
  }

  // shortcut for child_is<Expr>(1)
  Expr src() const noexcept {
    return child_is<Expr>(1);
  }

private:
  // downcast Node to Assign
  constexpr explicit Assign(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return is_assign(OpStmt2(op));
  }

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    return true;
  }
};

////////////////////////////////////////////////////////////////////////////////
class Case : public Stmt2 {
  using Base = Stmt2;
  friend class Node;

public:
  /**
   * construct an invalid Case.
   * exists only to allow placing Case in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Case, use one of the other constructors
   */
  constexpr Case() noexcept : Base{} {
  }

  Case(Func &func, Expr expr, Node body) noexcept //
      : Base{func, expr, body, CASE} {
  }

  // can return either CASE or DEFAULT
  constexpr OpStmt2 op() const noexcept {
    return OpStmt2(Base::op());
  }

  // shortcut for child_is<Expr>(0)
  Expr expr() noexcept {
    return child_is<Expr>(0);
  }

  // shortcut for child(1)
  Node body() noexcept {
    return child(1);
  }

protected:
  // downcast Node to Case
  constexpr explicit Case(const Node &node) noexcept : Base{node} {
  }

  // used by subclasses
  Case(Func &func, Expr expr, Node body, OpStmt2 op) noexcept //
      : Base{func, expr, body, op} {
  }

private:
  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == CASE || op == DEFAULT;
  }

  static constexpr bool child_result_is_used(uint32_t i) noexcept {
    return i == 0;
  }
};

////////////////////////////////////////////////////////////////////////////////
// Default is represented as a Case where op == DEFAULT and child(0) == VoidConst
class Default : public Case {
  using Base = Case;
  friend class Node;

public:
  /**
   * construct an invalid Default.
   * exists only to allow placing Default in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Default, use one of the other constructors
   */
  constexpr Default() noexcept : Base{} {
  }

  Default(Func &func, Node body) noexcept //
      : Base{func, VoidConst, body, DEFAULT} {
  }

  static constexpr OpStmt2 op() noexcept {
    return DEFAULT;
  }

  // always returns VoidConst
  static constexpr Expr expr() noexcept {
    return VoidConst;
  }

private:
  // downcast Node to Default
  constexpr explicit Default(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == DEFAULT;
  }

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    return false;
  }
};

////////////////////////////////////////////////////////////////////////////////
// conditional jump. usually only found in compiled code
class JumpIf : public Stmt2 {
  using Base = Stmt2;
  friend class Node;

public:
  /**
   * construct an invalid JumpIf.
   * exists only to allow placing JumpIf in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid JumpIf, use one of the other constructors
   */
  constexpr JumpIf() noexcept : Base{} {
  }

  JumpIf(Func &func, Label to, Expr test) noexcept //
      : Base{func, to, test, JUMP_IF} {
  }

  static constexpr OpStmt2 op() noexcept {
    return JUMP_IF;
  }

  // shortcut for child_is<Label>(0)
  Label to() const noexcept {
    return child_is<Label>(0);
  }

  // shortcut for child_is<Expr>(1)
  Expr test() const noexcept {
    return child_is<Expr>(1);
  }

private:
  // downcast Node to JumpIf
  constexpr explicit JumpIf(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == JUMP_IF;
  }

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    return true;
  }
};

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_STMT2_HPP
