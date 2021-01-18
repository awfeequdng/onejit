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
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_STMT_HPP
#define ONEJIT_STMT_HPP

#include <onejit/node.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Stmt0 : public Node {
  using Base = Node;

  friend class Node;

public:
  constexpr Stmt0() : Base{} {
  }

  constexpr explicit Stmt0(OpStmt0 op) : Base{NodeHeader{STMT_0, VOID, op}, 0, nullptr} {
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

////////////////////////////////////////////////////////////////////////////////
class Bad : public Node {
public:
  constexpr Bad() : Stmt0(BAD_OP_STMT_0) {
  }
};

////////////////////////////////////////////////////////////////////////////////
class Break : public Stmt0 {
public:
  constexpr Break() : Stmt0(BREAK) {
  }
};

////////////////////////////////////////////////////////////////////////////////
class Continue : public Stmt0 {
public:
  constexpr Continue() : Stmt0(CONTINUE) {
  }
};

////////////////////////////////////////////////////////////////////////////////
class Fallthrough : public Stmt0 {
public:
  constexpr Fallthrough() : Stmt0(FALLTHROUGH) {
  }
};

} // namespace onejit

#endif // ONEJIT_STMT_HPP
