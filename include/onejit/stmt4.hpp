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
 * stmt4.hpp
 *
 *  Created on Jan 19, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_STMT4_HPP
#define ONEJIT_STMT4_HPP

#include <onejit/expr.hpp>
#include <onejit/opstmt.hpp>
#include <onejit/stmt.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Stmt4 : public Stmt {
  using Base = Stmt;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Stmt4.
   * exists only to allow placing Stmt4 in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Stmt4, use Func::new_stmt4()
   */
  constexpr Stmt4() : Base{STMT_4, Bad, BAD_ST4} {
  }

  static constexpr uint32_t children() {
    return 4;
  }

protected:
  /* construct an invalid Stmt4 */
  constexpr explicit Stmt4(OpStmt4 op) : Base{STMT_4, Bad, op} {
  }

  // downcast Node to Stmt4
  constexpr explicit Stmt4(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) {
    return t == STMT_4;
  }

  static Stmt4 create(OpStmt4 op, const Node &child0, const Node &child1, const Node &child2,
                      const Node &child3, Code *holder);
};

std::ostream &operator<<(std::ostream &out, const Stmt4 &st);

////////////////////////////////////////////////////////////////////////////////
class ForStmt : public Stmt4 {
  using Base = Stmt4;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid ForStmt.
   * exists only to allow placing ForStmt in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid ForStmt, use Func::new_for()
   */
  constexpr ForStmt() : Base{FOR} {
  }

  static constexpr OpStmt4 op() {
    return FOR;
  }

  // shortcut for child(0)
  Node init() const {
    return child(0);
  }

  // shortcut for child(1).is<Expr>()
  Expr cond() const {
    return child(1).is<Expr>();
  }

  // shortcut for child(2)
  Node post() const {
    return child(2);
  }

  // shortcut for child(3)
  Node body() const {
    return child(3);
  }

private:
  // downcast Node to ForStmt
  constexpr explicit ForStmt(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) {
    return op == FOR;
  }

  static ForStmt create(const Node &init, const Expr &cond, const Node &post, const Node &body,
                        Code *holder) {
    return ForStmt{Stmt4::create(FOR, init, cond, post, body, holder)};
  }
};

} // namespace onejit

#endif // ONEJIT_STMT4_HPP
