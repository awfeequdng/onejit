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
 *  Created on Jan 18, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_STMT3_HPP
#define ONEJIT_STMT3_HPP

#include <onejit/expr.hpp>
#include <onejit/opstmt.hpp>
#include <onejit/stmt.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Stmt3 : public Stmt {
  using Base = Stmt;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Stmt3.
   * exists only to allow placing Stmt3 in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Stmt3, use Func::new_stmt3()
   */
  constexpr Stmt3() : Base{STMT_3, Bad, BAD_ST3} {
  }

  static constexpr uint32_t children() {
    return 3;
  }

protected:
  /* construct an invalid Stmt3 */
  constexpr explicit Stmt3(OpStmt3 op) : Base{STMT_3, Bad, op} {
  }

  // downcast Node to Stmt3
  constexpr explicit Stmt3(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) {
    return t == STMT_3;
  }

  static Stmt3 create(OpStmt3 op, const Node &child0, const Node &child1, const Node &child2,
                      Code *holder);
};

std::ostream &operator<<(std::ostream &out, const Stmt3 &st);

////////////////////////////////////////////////////////////////////////////////
class IfStmt : public Stmt3 {
  using Base = Stmt3;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid IfStmt.
   * exists only to allow placing IfStmt in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid IfStmt, use Func::new_if()
   */
  constexpr IfStmt() : Base{IF} {
  }

  static constexpr OpStmt3 op() {
    return IF;
  }

  // shortcut for child(0).is<Expr>()
  Expr cond() const {
    return child(0).is<Expr>();
  }

  // shortcut for child(1)
  Node then() const {
    return child(1);
  }

  // shortcut for child(2)
  Node else_() const {
    return child(2);
  }

private:
  // downcast Node to IfStmt
  constexpr explicit IfStmt(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) {
    return op == IF;
  }

  static IfStmt create(const Expr &cond, const Node &then, const Node &else_, Code *holder) {
    return IfStmt{Stmt3::create(IF, cond, then, else_, holder)};
  }
};

} // namespace onejit

#endif // ONEJIT_STMT3_HPP
