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
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_STMT0_HPP
#define ONEJIT_NODE_STMT0_HPP

#include <onejit/fmt.hpp>
#include <onejit/node/stmt.hpp>
#include <onejit/opstmt.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Stmt0 : public Stmt {
  using Base = Stmt;
  friend class Node;

public:
  /**
   * construct an invalid Stmt0
   * exists only to allow placing Stmt0 in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Stmt0, use Stmt0(OpStmt0) or a derived class constructor
   */
  constexpr Stmt0() noexcept : Base{} {
  }

  constexpr explicit Stmt0(OpStmt0 op) noexcept : Base{create(op)} {
  }

  static constexpr Type type() noexcept {
    return STMT_0;
  }

  static constexpr Kind kind() noexcept {
    return Void;
  }

  constexpr OpStmt0 op() const noexcept {
    return OpStmt0(Base::op());
  }

  static constexpr uint32_t children() noexcept {
    return 0;
  }

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

protected:
  // downcast Node to Stmt0.
  // Stmt0 is always direct => set code_ to nullptr
  constexpr explicit Stmt0(const Node &node) noexcept //
      : Base{Node{node.header(), node.offset_or_direct(), nullptr}} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_0;
  }

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    // no children
    return false;
  }

  static constexpr Node create(OpStmt0 op) noexcept {
    return Node{NodeHeader{STMT_0, Void, op}, direct(op), nullptr};
  }

  static constexpr CodeItem direct(OpStmt0 op) noexcept {
    return op <= FALLTHROUGH ? CodeItem(op) : CodeItem(op) << 4 | 0x6;
  }

  static constexpr OpStmt0 parse_direct_op(CodeItem item) noexcept {
    return OpStmt0(item >> 4);
  }
};

////////////////////////////////////////////////////////////////////////////////
class BadStmt : public Stmt0 {
  using Base = Stmt0;
  friend class Node;

public:
  constexpr BadStmt() noexcept : Base{BAD} {
  }

  static constexpr OpStmt0 op() noexcept {
    return BAD;
  }

private:
  // downcast Node to BadStmt
  constexpr explicit BadStmt(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == BAD;
  }
};

////////////////////////////////////////////////////////////////////////////////
class Break : public Stmt0 {
  using Base = Stmt0;
  friend class Node;

public:
  constexpr Break() noexcept : Base{BREAK} {
  }

  static constexpr OpStmt0 op() noexcept {
    return BREAK;
  }

private:
  // downcast Node to Break
  constexpr explicit Break(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == BREAK;
  }
};

////////////////////////////////////////////////////////////////////////////////
class Continue : public Stmt0 {
  using Base = Stmt0;
  friend class Node;

public:
  constexpr Continue() noexcept : Base{CONTINUE} {
  }

  static constexpr OpStmt0 op() noexcept {
    return CONTINUE;
  }

private:
  // downcast Node to Continue
  constexpr explicit Continue(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == CONTINUE;
  }
};

////////////////////////////////////////////////////////////////////////////////
class Fallthrough : public Stmt0 {
  using Base = Stmt0;
  friend class Node;

public:
  constexpr Fallthrough() noexcept : Base{FALLTHROUGH} {
  }

  static constexpr OpStmt0 op() noexcept {
    return FALLTHROUGH;
  }

private:
  // downcast Node to Fallthrough
  constexpr explicit Fallthrough(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == FALLTHROUGH;
  }
};

} // namespace onejit

#endif // ONEJIT_NODE_STMT0_HPP
