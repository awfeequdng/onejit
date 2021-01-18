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
 *  Created on Jan 18, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_STMT2_HPP
#define ONEJIT_STMT2_HPP

#include <onejit/opstmt.hpp>
#include <onejit/stmt.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Stmt2 : public Stmt {
  using Base = Stmt;
  friend class Node;
  friend class Func;

public:
  constexpr Stmt2() : Base{} {
  }

  static constexpr uint32_t children() {
    return 2;
  }

protected:
  constexpr explicit Stmt2(OpStmt2 op) : Base{STMT_2, op} {
  }

  constexpr Stmt2(NodeHeader header, Offset offset_or_direct, const Code *code) //
      : Base{header, offset_or_direct, code} {
  }

  // downcast Node to Stmt2
  constexpr explicit Stmt2(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) {
    return t == STMT_2;
  }

  static Stmt2 create(OpStmt2 op, const Node &child0, const Node &child1, Code *holder);
};

std::ostream &operator<<(std::ostream &out, const Stmt2 &st2);

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
  constexpr Case() : Base(BAD_OP_STMT_2) {
  }

  static constexpr OpStmt2 op() {
    return CASE;
  }

private:
  constexpr Case(NodeHeader header, Offset offset_or_direct, const Code *code) //
      : Base{header, offset_or_direct, code} {
  }

  // downcast Node to Case
  constexpr explicit Case(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) {
    return op == CASE;
  }

  static Case create(const Expr &expr, const Node &body, Code *holder);
};

} // namespace onejit

#endif // ONEJIT_STMT2_HPP
