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
 * stmt1.hpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_STMT1_HPP
#define ONEJIT_STMT1_HPP

#include <onejit/fmt.hpp>
#include <onejit/label.hpp>
#include <onejit/opstmt.hpp>
#include <onejit/stmt.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Stmt1 : public Stmt {
  using Base = Stmt;
  friend class Compiler;
  friend class Func;
  friend class Node;
  friend class Test;

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

  // shortcut for child(0)
  Node body() const noexcept {
    return child(0);
  }

protected:
  // downcast Node to Stmt1
  constexpr explicit Stmt1(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_1;
  }

  // used by subclasses
  Stmt1(Func &func, Node body, OpStmt1 op) noexcept : Base{create(func, body, op)} {
  }

private:
  static Node create(Func &func, Node body, OpStmt1 op) noexcept;
};

const Fmt &operator<<(const Fmt &out, const Stmt1 &st);

////////////////////////////////////////////////////////////////////////////////
class Goto : public Stmt1 {
  using Base = Stmt1;
  friend class Node;
  friend class Func;

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

  // shortcut for child(0).is<Label>()
  Label to() const noexcept {
    return child(0).is<Label>();
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
  friend class Func;

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

  // shortcut for child(0).is<Expr>()
  Expr expr() const noexcept {
    return child(0).is<Expr>();
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
  friend class Func;

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

  // shortcut for child(0).is<Expr>()
  Expr expr() const noexcept {
    return child(0).is<Expr>();
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
} // namespace onejit

#endif // ONEJIT_STMT1_HPP
