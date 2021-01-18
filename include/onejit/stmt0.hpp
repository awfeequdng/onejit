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
 * stmt0.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_STMT0_HPP
#define ONEJIT_STMT0_HPP

#include <onejit/opstmt.hpp>
#include <onejit/stmt.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Stmt0 : public Stmt {
  using Base = Stmt;
  friend class Node;

public:
  constexpr Stmt0() : Base{} {
  }

  constexpr explicit Stmt0(OpStmt0 op) : Base{STMT_0, op} {
  }

  static constexpr uint32_t children() {
    return 0;
  }

protected:
  constexpr Stmt0(NodeHeader header, Offset offset_or_direct, const Code *code) //
      : Base{header, offset_or_direct, code} {
  }

  // downcast Node to Stmt0
  constexpr explicit Stmt0(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) {
    return t == STMT_0;
  }
};

std::ostream &operator<<(std::ostream &out, const Stmt0 &st0);

////////////////////////////////////////////////////////////////////////////////
class Bad : public Stmt0 {
  using Base = Stmt0;
  friend class Node;

public:
  constexpr Bad() : Base(BAD_OP_STMT_0) {
  }

  static constexpr OpStmt0 op() {
    return BAD_OP_STMT_0;
  }

private:
  constexpr Bad(NodeHeader header, Offset offset_or_direct, const Code *code) //
      : Base{header, offset_or_direct, code} {
  }

  // downcast Node to Bad
  constexpr explicit Bad(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) {
    return op == BAD_OP_STMT_0;
  }
};

////////////////////////////////////////////////////////////////////////////////
class Break : public Stmt0 {
  using Base = Stmt0;
  friend class Node;

public:
  constexpr Break() : Base(BREAK) {
  }

  static constexpr OpStmt0 op() {
    return BREAK;
  }

private:
  constexpr Break(NodeHeader header, Offset offset_or_direct, const Code *code) //
      : Base{header, offset_or_direct, code} {
  }

  // downcast Node to Break
  constexpr explicit Break(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) {
    return op == BREAK;
  }
};

////////////////////////////////////////////////////////////////////////////////
class Continue : public Stmt0 {
  using Base = Stmt0;
  friend class Node;

public:
  constexpr Continue() : Base(CONTINUE) {
  }

  static constexpr OpStmt0 op() {
    return CONTINUE;
  }

private:
  constexpr Continue(NodeHeader header, Offset offset_or_direct, const Code *code) //
      : Base{header, offset_or_direct, code} {
  }

  // downcast Node to Continue
  constexpr explicit Continue(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) {
    return op == CONTINUE;
  }
};

////////////////////////////////////////////////////////////////////////////////
class Fallthrough : public Stmt0 {
  using Base = Stmt0;
  friend class Node;

public:
  constexpr Fallthrough() : Base(FALLTHROUGH) {
  }

  static constexpr OpStmt0 op() {
    return FALLTHROUGH;
  }

private:
  constexpr Fallthrough(NodeHeader header, Offset offset_or_direct, const Code *code) //
      : Base{header, offset_or_direct, code} {
  }

  // downcast Node to Fallthrough
  constexpr explicit Fallthrough(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) {
    return op == FALLTHROUGH;
  }
};

} // namespace onejit

#endif // ONEJIT_STMT0_HPP
