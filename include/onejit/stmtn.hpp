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
 *  Created on Jan 18, 2021
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
  constexpr StmtN() noexcept : Base{STMT_N, Bad, BAD_STN} {
  }

  constexpr OpStmtN op() const noexcept {
    return OpStmtN(Base::op());
  }

protected:
  /* construct an invalid StmtN */
  constexpr explicit StmtN(OpStmtN op) noexcept : Base{STMT_N, Bad, op} {
  }

  // downcast Node to StmtN
  constexpr explicit StmtN(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_N;
  }

  static StmtN create(OpStmtN op, const Nodes nodes, Code *holder) noexcept;
};

std::ostream &operator<<(std::ostream &out, const StmtN &st);

////////////////////////////////////////////////////////////////////////////////
// collect multiple results of a function call and assign them to multiple variables
// or memory locations
class AssignCall : public StmtN {
  using Base = StmtN;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid AssignCall.
   * exists only to allow placing AssignCall in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid AssignCall, use Func::new_block()
   */
  constexpr AssignCall() noexcept : Base{ASSIGN_TUPLE} {
  }

  static constexpr OpStmtN op() noexcept {
    return ASSIGN_TUPLE;
  }

private:
  // downcast Node to AssignCall
  constexpr explicit AssignCall(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == ASSIGN_TUPLE;
  }

  // each assign_to element must be a Var or a Mem
  static AssignCall create(Exprs assign_to, const Call &call, Code *holder) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
class Block : public StmtN {
  using Base = StmtN;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Block.
   * exists only to allow placing Block in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Block, use Func::new_block()
   */
  constexpr Block() noexcept : Base{BLOCK} {
  }

  static constexpr OpStmtN op() noexcept {
    return BLOCK;
  }

private:
  // downcast Node to Block
  constexpr explicit Block(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == BLOCK;
  }

  static Block create(const Nodes nodes, Code *holder) noexcept {
    return Block{StmtN::create(BLOCK, nodes, holder)};
  }
};

////////////////////////////////////////////////////////////////////////////////
// equivalent to a list of nested if-elseif-...-else
// i.e.
// (COND expr1 body1 expr2 body2 ... exprn bodyn)
// is equivalent to
// (IF expr1 body1 (IF expr2 body2 (... (IF exprn bodyn void) ...)))
class Cond : public StmtN {
  using Base = StmtN;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Cond.
   * exists only to allow placing Cond in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Cond, use Func::new_block()
   */
  constexpr Cond() noexcept : Base{COND} {
  }

  static constexpr OpStmtN op() noexcept {
    return COND;
  }

private:
  // downcast Node to Cond
  constexpr explicit Cond(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == COND;
  }

  static Cond create(const Nodes nodes, Code *holder) noexcept {
    return Cond{StmtN::create(COND, nodes, holder)};
  }
};

////////////////////////////////////////////////////////////////////////////////
// return 0, 1 or multiple values
class Return : public StmtN {
  using Base = StmtN;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Return.
   * exists only to allow placing Return in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Return, use Func::new_return()
   */
  constexpr Return() noexcept : Base{RETURN} {
  }

  static constexpr OpStmtN op() noexcept {
    return RETURN;
  }

private:
  // downcast Node to Return
  constexpr explicit Return(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == RETURN;
  }

  static Return create(Exprs exprs, Code *holder) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
class Switch : public StmtN {
  using Base = StmtN;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Switch.
   * exists only to allow placing Switch in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Switch, use Func::new_switch()
   */
  constexpr Switch() noexcept : Base{SWITCH} {
  }

  static constexpr OpStmtN op() noexcept {
    return SWITCH;
  }

private:
  // downcast Node to Switch
  constexpr explicit Switch(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == SWITCH;
  }

  // cases can contain at most one Default
  static Switch create(const Expr &expr, const Cases cases, Code *holder) noexcept;
};

} // namespace onejit

#endif // ONEJIT_STMTN_HPP
