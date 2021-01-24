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

#ifndef ONEJIT_STMT3_HPP
#define ONEJIT_STMT3_HPP

#include <onejit/const.hpp> // VoidExpr
#include <onejit/expr.hpp>
#include <onejit/opstmt.hpp>
#include <onejit/stmt.hpp>

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
  constexpr Stmt3() noexcept : Base{STMT_3, Bad, BAD_ST3} {
  }

  constexpr OpStmt3 op() const noexcept {
    return OpStmt3(Base::op());
  }

  static constexpr uint32_t children() noexcept {
    return 3;
  }

protected:
  /* construct an invalid Stmt3 */
  constexpr explicit Stmt3(OpStmt3 op) noexcept : Base{STMT_3, Bad, op} {
  }

  // downcast Node to Stmt3
  constexpr explicit Stmt3(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_3;
  }

  static Node create(Code *holder, Nodes children, OpStmt3 op) noexcept;
};

std::ostream &operator<<(std::ostream &out, const Stmt3 &st);

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
  constexpr If() noexcept : Base{IF} {
  }

  // create a new 'if (cond) { then } else { else_ }'
  // the 'else' part can be omitted also by specifying else_ = VoidExpr
  If(Code *holder, Expr cond, Node then, Node else_ = VoidExpr) noexcept //
      : Base{create(holder, cond, then, else_)} {
  }

  static constexpr OpStmt3 op() noexcept {
    return IF;
  }

  // shortcut for child(0).is<Expr>()
  Expr cond() const noexcept {
    return child(0).is<Expr>();
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

  // create a new If
  static Node create(Code *holder, Expr cond, Node then, Node else_) noexcept;
};

} // namespace onejit

#endif // ONEJIT_STMT3_HPP
