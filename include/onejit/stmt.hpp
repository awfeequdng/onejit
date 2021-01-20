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
 * stmt.hpp
 *
 *  Created on Jan 18, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_STMT_HPP
#define ONEJIT_STMT_HPP

#include <onejit/node.hpp>
#include <onejit/opstmt.hpp>

namespace onejit {

// base class of Stmt0, Stmt1, Stmt2, Stmt3, Stmt4, StmtN
class Stmt : public Node {
  using Base = Node;
  friend class Node;

public:
  constexpr Stmt() noexcept : Base{} {
  }

  static constexpr Kind kind() noexcept {
    return Void;
  }

protected:
  constexpr Stmt(Type t, Kind kind, uint16_t op) noexcept
      : Base{NodeHeader{t, kind, op}, 0, nullptr} {
  }

  // downcast Node to Stmt
  constexpr explicit Stmt(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t >= STMT_0 && t <= STMT_N;
  }
};

} // namespace onejit

#endif // ONEJIT_STMT_HPP
