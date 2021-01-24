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

  static StmtN create(Code *holder, const Nodes nodes, OpStmtN op) noexcept;
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
   * to create a valid AssignCall, use one of the other constructors
   */
  constexpr AssignCall() noexcept : Base{ASSIGN_TUPLE} {
  }

  // assign multiple values returned by a Call.
  // each assign_to element must be a Var or a Mem.
  AssignCall(Code *holder, std::initializer_list<Expr> assign_to, const Call &call) noexcept
      : AssignCall{create(holder, Exprs{assign_to.begin(), assign_to.size()}, call)} {
  }

  // assign multiple values returned by a Call.
  // each assign_to element must be a Var or a Mem.
  AssignCall(Code *holder, Exprs assign_to, const Call &call) noexcept
      : AssignCall{create(holder, assign_to, call)} {
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

  static AssignCall create(Code *holder, Exprs assign_to, const Call &call) noexcept;
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
   * to create a valid Block, use one of the other constructors
   */
  constexpr Block() noexcept : Base{BLOCK} {
  }

  Block(Code *holder, const Node &node) noexcept //
      : Block{holder, Nodes{&node, 1}} {
  }

  Block(Code *holder, std::initializer_list<Node> nodes) noexcept
      : Block(holder, Nodes{nodes.begin(), nodes.size()}) {
  }

  Block(Code *holder, const Nodes nodes) noexcept //
      : Block{StmtN::create(holder, nodes, BLOCK)} {
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
   * to create a valid Cond, use one of the other constructors
   */
  constexpr Cond() noexcept : Base{COND} {
  }

  Cond(Code *holder, std::initializer_list<Node> nodes) noexcept
      : Cond{StmtN::create(holder, Nodes{nodes.begin(), nodes.size()}, COND)} {
  }

  Cond(Code *holder, Nodes nodes) noexcept //
      : Cond{StmtN::create(holder, nodes, COND)} {
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

}; // namespace onejit

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
   * to create a valid Return, use one of the other constructors
   */
  constexpr Return() noexcept : Base{RETURN} {
  }

  explicit Return(Code *holder) noexcept //
      : Return{create(holder, Exprs{})} {
  }

  Return(Code *holder, Expr expr) noexcept //
      : Return{create(holder, Exprs{&expr, 1})} {
  }

  Return(Code *holder, std::initializer_list<Expr> exprs) noexcept
      : Return{create(holder, Exprs{exprs.begin(), exprs.size()})} {
  }

  Return(Code *holder, Exprs exprs) noexcept //
      : Return{create(holder, exprs)} {
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

  static Node create(Code *holder, Exprs exprs) noexcept;
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
