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
 * stmtn.hpp
 *
 *  Created on Jan 18, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_STMTN_HPP
#define ONEJIT_STMTN_HPP

#include <onejit/opstmt.hpp>
#include <onejit/stmt.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class StmtN : public Stmt {
  using Base = Stmt;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid StmtN.
   * exists only to allow placing StmtN in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid StmtN, use Func::new_stmtn()
   */
  constexpr StmtN() : Base{STMT_N, Bad, BAD_STN} {
  }

protected:
  /* construct an invalid StmtN */
  constexpr explicit StmtN(OpStmtN op) : Base{STMT_N, Bad, op} {
  }

  // downcast Node to StmtN
  constexpr explicit StmtN(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) {
    return t == STMT_N;
  }

  static StmtN create(OpStmtN op, const Nodes children, Code *holder);
};

std::ostream &operator<<(std::ostream &out, const StmtN &st);

////////////////////////////////////////////////////////////////////////////////
class BlockStmt : public StmtN {
  using Base = StmtN;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid BlockStmt.
   * exists only to allow placing BlockStmt in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid BlockStmt, use Func::new_block()
   */
  constexpr BlockStmt() : Base{BLOCK} {
  }

  static constexpr OpStmtN op() {
    return BLOCK;
  }

private:
  // downcast Node to BlockStmt
  constexpr explicit BlockStmt(const Node &node) : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) {
    return op == BLOCK;
  }

  static BlockStmt create(const Nodes children, Code *holder) {
    return BlockStmt{StmtN::create(BLOCK, children, holder)};
  }
};

} // namespace onejit

#endif // ONEJIT_STMTN_HPP
