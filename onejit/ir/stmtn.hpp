/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * stmtn.hpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_STMTN_HPP
#define ONEJIT_IR_STMTN_HPP

#include <onejit/fmt.hpp>
#include <onejit/ir/stmt.hpp>
#include <onejit/ir/stmt2.hpp> // onejit::Case
#include <onejit/ir/util.hpp>  // is_return_op()
#include <onejit/mir/fwd.hpp>
#include <onejit/opstmtn.hpp>
#include <onejit/x64/fwd.hpp>

namespace onejit {
namespace ir {

////////////////////////////////////////////////////////////////////////////////
class StmtN : public Stmt {
  using Base = Stmt;
  friend class Node;
  friend class ::onejit::Compiler;
  friend class ::onejit::Func;
  friend class x64::Compiler;
  friend class mir::Compiler;

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

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

protected:
  // downcast Node to StmtN
  constexpr explicit StmtN(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == STMT_N;
  }

  // used by subclasses
  StmtN(Func &func, OpStmtN op, Nodes children) noexcept //
      : Base{create(func, op, children)} {
  }

  // used by x64::Compiler
  StmtN(Func &func, OpStmtN op, ChildRanges children) noexcept //
      : Base{create(func, op, children)} {
  }

  static Node create(Func &func, OpStmtN op, ChildRanges children) noexcept;
  static Node create(Func &func, OpStmtN op, Nodes children) noexcept;

private:
  constexpr bool child_result_is_used(uint32_t i) const noexcept {
    return op() == BLOCK ? false : op() == COND ? (i & 1) == 0 : true;
  }
};

////////////////////////////////////////////////////////////////////////////////
// collect multiple results of a function call and assign them to multiple variables
// or memory locations
class AssignCall : public StmtN {
  using Base = StmtN;
  friend class Node;
  friend class ::onejit::Func;

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
  friend class ::onejit::Func;

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
      : Base{func, BLOCK, Nodes{&node, 1}} {
  }

  Block(Func &func, std::initializer_list<Node> nodes) noexcept
      : Base{func, BLOCK, Nodes{nodes.begin(), nodes.size()}} {
  }

  Block(Func &func, const Nodes nodes) noexcept //
      : Base{func, BLOCK, nodes} {
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
  friend class ::onejit::Func;

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
      : Base{func, COND, Nodes{nodes.begin(), nodes.size()}} {
  }

  Cond(Func &func, Nodes nodes) noexcept //
      : Base{func, COND, nodes} {
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
/// @return 0, 1 or multiple values
class Return : public StmtN {
  using Base = StmtN;
  friend class Node;
  friend class ::onejit::Func;

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

  explicit Return(Func &func, OpStmtN op = RETURN) noexcept //
      : Base{func, op, Nodes{}} {
  }

  Return(Func &func, Expr expr) noexcept //
      : Base{func, RETURN, Nodes{&expr, 1}} {
  }

  Return(Func &func, OpStmtN op, Expr expr) noexcept //
      : Base{func, op, Nodes{&expr, 1}} {
  }

  Return(Func &func, OpStmtN op, std::initializer_list<Expr> exprs) noexcept
      : Base{func, op, Nodes{exprs.begin(), exprs.size()}} {
  }

  Return(Func &func, OpStmtN op, Exprs exprs) noexcept //
      : Base{func, op, Nodes{exprs.data(), exprs.size()}} {
  }

  // used by x64::Compiler
  Return(Func &func, OpStmtN op, ChildRanges children) noexcept //
      : Base{func, op, children} {
  }

private:
  // downcast Node to Return
  constexpr explicit Return(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return is_return_op(OpStmtN(op));
  }

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    return true;
  }
};

////////////////////////////////////////////////////////////////////////////////
class Switch : public StmtN {
  using Base = StmtN;
  friend class Node;
  friend class ::onejit::Func;

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

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_STMTN_HPP
