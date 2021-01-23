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
  /**
   * construct an invalid Stmt0
   * exists only to allow placing Stmt0 in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Stmt0, use Stmt0(OpStmt0)
   */
  constexpr Stmt0() noexcept : Base{STMT_0, Bad, BAD} {
  }

  constexpr explicit Stmt0(OpStmt0 op) noexcept : Base{STMT_0, Void, op} {
  }

  static constexpr uint32_t children() noexcept {
    return 0;
  }

  Compiler &compile(Compiler &comp) const noexcept;

protected:
  // downcast Node to Stmt0
  constexpr explicit Stmt0(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_0;
  }
};

inline std::ostream &operator<<(std::ostream &out, const Stmt0 &st) {
  return out << st.op();
}

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
class BreakStmt : public Stmt0 {
  using Base = Stmt0;
  friend class Node;

public:
  constexpr BreakStmt() noexcept : Base{BREAK} {
  }

  static constexpr OpStmt0 op() noexcept {
    return BREAK;
  }

private:
  // downcast Node to BreakStmt
  constexpr explicit BreakStmt(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == BREAK;
  }
};

////////////////////////////////////////////////////////////////////////////////
class ContinueStmt : public Stmt0 {
  using Base = Stmt0;
  friend class Node;

public:
  constexpr ContinueStmt() noexcept : Base{CONTINUE} {
  }

  static constexpr OpStmt0 op() noexcept {
    return CONTINUE;
  }

private:
  // downcast Node to ContinueStmt
  constexpr explicit ContinueStmt(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == CONTINUE;
  }
};

////////////////////////////////////////////////////////////////////////////////
class FallthroughStmt : public Stmt0 {
  using Base = Stmt0;
  friend class Node;

public:
  constexpr FallthroughStmt() noexcept : Base{FALLTHROUGH} {
  }

  static constexpr OpStmt0 op() noexcept {
    return FALLTHROUGH;
  }

private:
  // downcast Node to FallthroughStmt
  constexpr explicit FallthroughStmt(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == FALLTHROUGH;
  }
};

} // namespace onejit

#endif // ONEJIT_STMT0_HPP
