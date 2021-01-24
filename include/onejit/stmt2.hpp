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
 * stmt2.hpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_STMT2_HPP
#define ONEJIT_STMT2_HPP

#include <onejit/const.hpp> // VoidExpr
#include <onejit/label.hpp>
#include <onejit/opstmt.hpp>
#include <onejit/stmt.hpp>
#include <onestl/view.hpp>

#include <initializer_list>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Stmt2 : public Stmt {
  using Base = Stmt;
  friend class Node;
  friend class Func;
  friend class Default;

public:
  /**
   * construct an invalid Stmt2.
   * exists only to allow placing Stmt2 in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Stmt2, use Func::new_stmt2()
   */
  constexpr Stmt2() noexcept : Base{STMT_2, Bad, BAD_ST2} {
  }

  constexpr OpStmt2 op() const noexcept {
    return OpStmt2(Base::op());
  }

  static constexpr uint32_t children() noexcept {
    return 2;
  }

protected:
  /* construct an invalid Stmt2 */
  constexpr explicit Stmt2(OpStmt2 op) noexcept : Base{STMT_2, Bad, op} {
  }

  // downcast Node to Stmt2
  constexpr explicit Stmt2(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_2;
  }

  static Stmt2 create(OpStmt2 op, Nodes children, Code *holder) noexcept;
};

std::ostream &operator<<(std::ostream &out, const Stmt2 &st);

////////////////////////////////////////////////////////////////////////////////
class Assign : public Stmt2 {
  using Base = Stmt2;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Assign.
   * exists only to allow placing Assign in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Assign, use Func::new_assign()
   */
  constexpr Assign() noexcept : Base{ASSIGN} {
  }

  constexpr OpStmt2 op() const noexcept {
    return OpStmt2(Base::op());
  }

  // shortcut for child(0).is<Expr>()
  Expr dst() const noexcept {
    return child(0).is<Expr>();
  }

  // shortcut for child(1).is<Expr>()
  Expr src() const noexcept {
    return child(1).is<Expr>();
  }

private:
  // downcast Node to Assign
  constexpr explicit Assign(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op >= ADD_ASSIGN && op <= ASSIGN;
  }

  static Assign create(OpStmt2 op, const Expr &dst, const Expr &src, Code *holder) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
class Case : public Stmt2 {
  using Base = Stmt2;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Case.
   * exists only to allow placing Case in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Case, use Func::new_case()
   */
  constexpr Case() noexcept : Base{CASE} {
  }

  // can return either CASE or DEFAULT
  constexpr OpStmt2 op() const noexcept {
    return OpStmt2(Base::op());
  }

  // shortcut for child(0).is<Expr>()
  Expr expr() noexcept {
    return child(0).is<Expr>();
  }

  // shortcut for child(1)
  Node body() noexcept {
    return child(1);
  }

protected:
  // needed by Default{}
  constexpr Case(OpStmt2 op) noexcept : Base{op} {
  }

  // downcast Node to Case
  constexpr explicit Case(const Node &node) noexcept : Base{node} {
  }

private:
  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == CASE || op == DEFAULT;
  }

  static Case create(const Expr &expr, const Node &body, Code *holder) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
// Default is represented as a Case where child(0) is VoidExpr
class Default : public Case {
  using Base = Case;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Default.
   * exists only to allow placing Default in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Default, use Func::new_default()
   */
  constexpr Default() noexcept : Base{DEFAULT} {
  }

  static constexpr OpStmt2 op() noexcept {
    return DEFAULT;
  }

  // always returns VoidExpr
  static constexpr Expr expr() noexcept {
    return VoidExpr;
  }

private:
  // downcast Node to Default
  constexpr explicit Default(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == DEFAULT;
  }

  static Default create(const Node &body, Code *holder) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
// conditional jump. usually only found in compiled code
class JumpIf : public Stmt2 {
  using Base = Stmt2;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid JumpIf.
   * exists only to allow placing JumpIf in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid JumpIf, use Func::new_jump_if()
   */
  constexpr JumpIf() noexcept : Base{JUMP_IF} {
  }

  static constexpr OpStmt2 op() noexcept {
    return JUMP_IF;
  }

  // shortcut for child(0).is<Label>()
  Label to() const noexcept {
    return child(0).is<Label>();
  }

  // shortcut for child(1).is<Expr>()
  Expr cond() const noexcept {
    return child(1).is<Expr>();
  }

private:
  // downcast Node to JumpIf
  constexpr explicit JumpIf(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == JUMP_IF;
  }

  static JumpIf create(const Label &to, const Expr &cond, Code *holder) noexcept;
};

} // namespace onejit

#endif // ONEJIT_STMT2_HPP
