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

#ifndef ONEJIT_NODE_STMTN_HPP
#define ONEJIT_NODE_STMTN_HPP

#include <onejit/fmt.hpp>
#include <onejit/node/stmt.hpp>
#include <onejit/node/stmt2.hpp> // onejit::Case
#include <onejit/opstmt.hpp>
#include <onejit/x64/fwd.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class StmtN : public Stmt {
  using Base = Stmt;
  friend class Compiler;
  friend class Func;
  friend class Node;
  friend class x64::Compiler;

public:
  /**
   * construct an invalid StmtN.
   * exists only to allow placing StmtN in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid StmtN, use one of the other constructors
   */
  constexpr StmtN() noexcept : Base{} {
  }

  static constexpr Type type() noexcept {
    return STMT_N;
  }

  constexpr OpStmtN op() const noexcept {
    return OpStmtN(Base::op());
  }

  const Fmt &format(const Fmt &out, Syntax syntax = Syntax::Default, size_t depth = 0) const;

protected:
  // downcast Node to StmtN
  constexpr explicit StmtN(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_N;
  }

  // used by subclasses
  StmtN(Func &func, Nodes children, OpStmtN op) noexcept //
      : Base{create(func, children, op)} {
  }

  // used by x64::Compiler
  StmtN(Func &func, const ChildRanges &children, OpStmtN op) noexcept //
      : Base{create(func, children, op)} {
  }

private:
  constexpr bool child_result_is_used(uint32_t i) const noexcept {
    return op() == BLOCK ? false : op() == COND ? (i & 1) == 0 : true;
  }

  static Node create(Func &func, Nodes children, OpStmtN op) noexcept;
  static Node create(Func &func, const ChildRanges &children, OpStmtN op) noexcept;
};

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
  constexpr AssignCall() noexcept : Base{} {
  }

  // assign multiple values returned by a Call.
  // each assign_to element must be a Var or a Mem.
  AssignCall(Func &func, std::initializer_list<Expr> assign_to, const Call &call) noexcept
      : Base{create(func, Exprs{assign_to.begin(), assign_to.size()}, call)} {
  }

  // assign multiple values returned by a Call.
  // each assign_to element must be a Var or a Mem.
  AssignCall(Func &func, Exprs assign_to, const Call &call) noexcept
      : Base{create(func, assign_to, call)} {
  }

  static constexpr OpStmtN op() noexcept {
    return ASSIGN_CALL;
  }

private:
  // downcast Node to AssignCall
  constexpr explicit AssignCall(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == ASSIGN_CALL;
  }

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    return true;
  }

  static Node create(Func &func, Exprs assign_to, const Call &call) noexcept;
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
  constexpr Block() noexcept : Base{} {
  }

  Block(Func &func, const Node &node) noexcept //
      : Base{func, Nodes{&node, 1}, BLOCK} {
  }

  Block(Func &func, std::initializer_list<Node> nodes) noexcept
      : Base{func, Nodes{nodes.begin(), nodes.size()}, BLOCK} {
  }

  Block(Func &func, const Nodes nodes) noexcept //
      : Base{func, nodes, BLOCK} {
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

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    return false;
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
  constexpr Cond() noexcept : Base{} {
  }

  Cond(Func &func, std::initializer_list<Node> nodes) noexcept
      : Base{func, Nodes{nodes.begin(), nodes.size()}, COND} {
  }

  Cond(Func &func, Nodes nodes) noexcept //
      : Base{func, nodes, COND} {
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

  static constexpr bool child_result_is_used(uint32_t i) noexcept {
    return (i & 1) == 0;
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
   * to create a valid Return, use one of the other constructors
   */
  constexpr Return() noexcept : Base{} {
  }

  explicit Return(Func &func) noexcept //
      : Base{create(func, Exprs{})} {
  }

  Return(Func &func, Expr expr) noexcept //
      : Base{create(func, Exprs{&expr, 1})} {
  }

  Return(Func &func, std::initializer_list<Expr> exprs) noexcept
      : Base{create(func, Exprs{exprs.begin(), exprs.size()})} {
  }

  Return(Func &func, Exprs exprs) noexcept //
      : Base{create(func, exprs)} {
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

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    return true;
  }

  static Node create(Func &func, Exprs exprs) noexcept;
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
   * to create a valid Switch, use one of the other constructors
   */
  constexpr Switch() noexcept : Base{} {
  }

  // cases can contain at most one Default
  Switch(Func &func, const Expr &expr, std::initializer_list<Case> cases) noexcept //
      : Base{create(func, expr, Cases{cases.begin(), cases.size()})} {
  }

  // cases can contain at most one Default
  Switch(Func &func, const Expr &expr, const Cases cases) noexcept //
      : Base{create(func, expr, cases)} {
  }

  static constexpr OpStmtN op() noexcept {
    return SWITCH;
  }

  // shortcut for child_is<Expr>(0)
  Expr expr() const noexcept {
    return child_is<Expr>(0);
  }

  // shortcut for child(i+1).is<Case>()
  Case case_(uint32_t i) const noexcept;

private:
  // downcast Node to Switch
  constexpr explicit Switch(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == SWITCH;
  }

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    return true;
  }

  // cases can contain at most one Default
  static Node create(Func &func, const Expr &expr, const Cases cases) noexcept;
};

} // namespace onejit

#endif // ONEJIT_NODE_STMTN_HPP
