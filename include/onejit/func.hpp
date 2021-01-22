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
 * func.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FUNC_HPP
#define ONEJIT_FUNC_HPP

#include <onejit/binaryexpr.hpp>
#include <onejit/code.hpp>
#include <onejit/constexpr.hpp>
#include <onejit/error.hpp>
#include <onejit/functype.hpp>
#include <onejit/label.hpp>
#include <onejit/memexpr.hpp>
#include <onejit/op.hpp>
#include <onejit/stmt0.hpp>
#include <onejit/stmt1.hpp>
#include <onejit/stmt2.hpp>
#include <onejit/stmt3.hpp>
#include <onejit/stmt4.hpp>
#include <onejit/stmtn.hpp>
#include <onejit/tupleexpr.hpp>
#include <onejit/unaryexpr.hpp>
#include <onejit/varexpr.hpp>
#include <onestl/chars.hpp>
#include <onestl/string.hpp>
#include <onestl/vector.hpp>

namespace onejit {

class Func {
public:
  /**
   * construct an invalid Func.
   * exists only to allow placing Func in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Func, use one of the other constructors
   */
  Func() noexcept;

  Func(String &&name, const FuncType &ftype, Code *holder) noexcept;

  Func(Func &&other) noexcept = default;
  Func &operator=(Func &&other) noexcept = default;

  ~Func() noexcept;

  constexpr Code *code() const noexcept {
    return holder_;
  }

  constexpr explicit operator bool() const noexcept {
    return holder_ && *holder_;
  }

  // get function name
  constexpr Chars name() const noexcept {
    return Chars{name_};
  }

  // convert Func to Label.
  Label label() const noexcept;

  // get function type
  constexpr FuncType ftype() const noexcept {
    return ftype_;
  }

  // get number of params
  constexpr uint16_t param_n() const noexcept {
    return param_n_;
  }
  // get number of results
  constexpr uint16_t result_n() const noexcept {
    return result_n_;
  }

  /// \return i-th parameter, or VarExpr{} if out-of-bounds
  VarExpr param(uint16_t i) const noexcept;
  /// \return i-th result, or VarExpr{} if out-of-bounds
  VarExpr result(uint16_t i) const noexcept;

  //////////////////////////////////////////////////////////////////////////////

  AssignStmt new_assign(std::initializer_list<Expr> assign_to, const CallExpr &call) {
    return new_assign(Exprs{assign_to.begin(), assign_to.size()}, call);
  }

  AssignStmt new_assign(Exprs assign_to, const CallExpr &call) {
    return AssignStmt::create(assign_to, call, holder_);
  }

  BreakStmt new_break() noexcept {
    return BreakStmt{};
  }

  // also autodetects kind
  BinaryExpr new_binary(Op2 op, const Expr &left, const Expr &right) noexcept {
    return BinaryExpr::create(op, left, right, holder_);
  }

  BlockStmt new_block(const Node &node) noexcept {
    return new_block(Nodes{&node, 1});
  }
  BlockStmt new_block(std::initializer_list<Node> nodes) noexcept {
    return new_block(Nodes{nodes.begin(), nodes.size()});
  }
  BlockStmt new_block(Nodes nodes) noexcept {
    return BlockStmt::create(nodes, holder_);
  }

  // create a call to specified Func
  CallExpr new_call(const Func &func) {
    return new_call(func, Exprs{});
  }
  CallExpr new_call(const Func &func, std::initializer_list<Expr> args) {
    return new_call(func, Exprs{args.begin(), args.size()});
  }
  CallExpr new_call(const Func &func, Exprs args) {
    return CallExpr::create(func.ftype(), func.label(), args, holder_);
  }

  // create a call to an arbitrary function, for example a Func or an already compiled C function
  CallExpr new_call(const FuncType &ftype, const Label &flabel) {
    return new_call(ftype, flabel, Exprs{});
  }
  CallExpr new_call(const FuncType &ftype, const Label &flabel, std::initializer_list<Expr> args) {
    return new_call(ftype, flabel, Exprs{args.begin(), args.size()});
  }
  CallExpr new_call(const FuncType &ftype, const Label &flabel, Exprs args) {
    return CallExpr::create(ftype, flabel, args, holder_);
  }

  CaseStmt new_case(const Expr &expr, const Node &body) noexcept {
    return CaseStmt::create(expr, body, holder_);
  }

  CondStmt new_cond(std::initializer_list<Node> nodes) noexcept {
    return new_cond(Nodes{nodes.begin(), nodes.size()});
  }
  CondStmt new_cond(Nodes nodes) noexcept {
    return CondStmt::create(nodes, holder_);
  }

  ConstExpr new_const(const Const &c) noexcept {
    return ConstExpr::create(c, holder_);
  }

  ContinueStmt new_continue() noexcept {
    return ContinueStmt{};
  }

  DefaultStmt new_default(const Node &body) noexcept {
    return DefaultStmt::create(body, holder_);
  }

  FallthroughStmt new_fallthrough() noexcept {
    return FallthroughStmt{};
  }

  ForStmt new_for(const Node &init, const Expr &cond, const Node &post, const Node &body) noexcept {
    return ForStmt::create(init, cond, post, body, holder_);
  }

  GotoStmt new_goto(const Label &target) noexcept {
    return GotoStmt::create(target, holder_);
  }

  IfStmt new_if(const Expr &cond, const Node &then, const Node &else_) noexcept {
    return IfStmt::create(cond, then, else_, holder_);
  }

  // create a new local label, used for jumps within the function
  Label new_label() noexcept;

  MemExpr new_mem(Kind kind, const Expr &address) noexcept {
    return MemExpr::create(kind, address, holder_);
  }

  // create a new return expression
  ReturnStmt new_return() noexcept {
    return new_return(Exprs{});
  }
  ReturnStmt new_return(const Expr &expr) noexcept {
    return new_return(Exprs{&expr, 1});
  }
  ReturnStmt new_return(std::initializer_list<Expr> exprs) noexcept {
    return new_return(Exprs{exprs.begin(), exprs.size()});
  }
  ReturnStmt new_return(Exprs exprs) noexcept {
    return ReturnStmt::create(exprs, holder_);
  }

  Stmt0 new_stmt0(OpStmt0 op) noexcept {
    return Stmt0{op};
  }

  // cases can contain at most one DefaultStmt
  SwitchStmt new_switch(const Expr &expr, std::initializer_list<CaseStmt> cases) noexcept {
    return new_switch(expr, CaseStmts{cases.begin(), cases.size()});
  }
  SwitchStmt new_switch(const Expr &expr, const CaseStmts cases) noexcept {
    return SwitchStmt::create(expr, cases, holder_);
  }

  // create a new unary expression, overriding Kind autodetection.
  // needed by op == CAST
  UnaryExpr new_unary(Kind kind, Op1 op, const Expr &arg) noexcept {
    return UnaryExpr::create(kind, op, arg, holder_);
  }

  // also autodetects kind if op != CAST
  UnaryExpr new_unary(Op1 op, const Expr &arg) noexcept {
    return UnaryExpr::create(op, arg, holder_);
  }

  VarExpr new_var(Kind kind) noexcept;

  constexpr Node get_body() const noexcept {
    return body_;
  }

  Func &set_body(const Node &body) noexcept {
    body_var_n_ = vars_.size();
    body_ = body;
    return *this;
  }

  // get current destination for "break"
  Label label_break() const noexcept;

  // get current destination for "continue"
  Label label_continue() const noexcept;

  // add a compiled node
  Func &compile(const Node &node) noexcept;

  // add a compile error
  Func &compile_error(const Node &where, Chars msg) noexcept;

private:
  Code *holder_;
  uint16_t param_n_;
  uint16_t result_n_;
  uint32_t body_var_n_; // # local vars used by body_

  FuncType ftype_;
  Vector<VarExpr> vars_;
  Vector<Label> labels_;
  String name_;
  Node body_;

  // buffers needed by compile()
  Vector<Label> breaks_;    // stack of 'break' destination labels
  Vector<Label> continues_; // stack of 'continue' destination labels
  NodeVec compiled_;
  ErrorVec error_;
};

} // namespace onejit

#endif // ONEJIT_FUNC_HPP
