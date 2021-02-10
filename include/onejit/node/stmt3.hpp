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
 * stmt3.hpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_STMT3_HPP
#define ONEJIT_NODE_STMT3_HPP

#include <onejit/node/const.hpp> // VoidExpr
#include <onejit/fmt.hpp>
#include <onejit/node/expr.hpp>
#include <onejit/node/stmt.hpp>
#include <onejit/opstmt.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Stmt3 : public Stmt {
  using Base = Stmt;
  friend class Func;
  friend class Node;

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

  const Fmt &format(const Fmt &out, size_t depth = 0) const;

protected:
  // downcast Node to Stmt3
  constexpr explicit Stmt3(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_3;
  }

  constexpr bool child_result_is_used(uint32_t i) const noexcept {
    return op() == IF ? i == 0 : true;
  }

  static Node create(Func &func, Nodes children, OpStmt3 op) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
class If : public Stmt3 {
  using Base = Stmt3;
  friend class Func;
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
  // the 'else' part can be omitted also by specifying else_ = VoidExpr
  If(Func &func, const Expr &test, const Node &then, const Node &else_ = VoidExpr) noexcept //
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

} // namespace onejit

#endif // ONEJIT_NODE_STMT3_HPP
