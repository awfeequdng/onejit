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
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FUNC_HPP
#define ONEJIT_FUNC_HPP

#include <onejit/binary.hpp>
#include <onejit/call.hpp>
#include <onejit/code.hpp>
#include <onejit/functype.hpp>
#include <onejit/label.hpp>
#include <onejit/mem.hpp>
#include <onejit/op.hpp>
#include <onejit/stmt0.hpp>
#include <onejit/stmt1.hpp>
#include <onejit/stmt2.hpp>
#include <onejit/stmt3.hpp>
#include <onejit/stmt4.hpp>
#include <onejit/stmtn.hpp>
#include <onejit/unary.hpp>
#include <onejit/var.hpp>
#include <onestl/chars.hpp>
#include <onestl/string.hpp>
#include <onestl/vector.hpp>

namespace onejit {

class Func {
  friend class Var;

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

  /// \return i-th parameter, or Var{} if out-of-bounds
  Var param(uint16_t i) const noexcept;
  /// \return i-th result, or Var{} if out-of-bounds
  Var result(uint16_t i) const noexcept;

  //////////////////////////////////////////////////////////////////////////////

  AssignCall new_assign_tuple(std::initializer_list<Expr> assign_to, const Call &call) {
    return new_assign_tuple(Exprs{assign_to.begin(), assign_to.size()}, call);
  }

  AssignCall new_assign_tuple(Exprs assign_to, const Call &call) {
    return AssignCall::create(assign_to, call, holder_);
  }

  // also autodetects kind
  Binary new_binary(Op2 op, const Expr &left, const Expr &right) noexcept {
    return Binary::create(op, left, right, holder_);
  }

  Block new_block(const Node &node) noexcept {
    return new_block(Nodes{&node, 1});
  }
  Block new_block(std::initializer_list<Node> nodes) noexcept {
    return new_block(Nodes{nodes.begin(), nodes.size()});
  }
  Block new_block(Nodes nodes) noexcept {
    return Block::create(nodes, holder_);
  }

  // create a call to specified Func
  Call new_call(const Func &func, std::initializer_list<Expr> args) {
    return new_call(func, Exprs{args.begin(), args.size()});
  }
  Call new_call(const Func &func, Exprs args) {
    return Call::create(func.ftype(), func.label(), args, holder_);
  }

  // create a call to an arbitrary function, for example a Func or an already compiled C function
  Call new_call(const FuncType &ftype, const Label &flabel, std::initializer_list<Expr> args) {
    return new_call(ftype, flabel, Exprs{args.begin(), args.size()});
  }
  Call new_call(const FuncType &ftype, const Label &flabel, Exprs args) {
    return Call::create(ftype, flabel, args, holder_);
  }

  Cond new_cond(std::initializer_list<Node> nodes) noexcept {
    return new_cond(Nodes{nodes.begin(), nodes.size()});
  }
  Cond new_cond(Nodes nodes) noexcept {
    return Cond::create(nodes, holder_);
  }

  Default new_default(const Node &body) noexcept {
    return Default::create(body, holder_);
  }

  For new_for(const Node &init, const Expr &cond, const Node &post, const Node &body) noexcept {
    return For::create(init, cond, post, body, holder_);
  }

  Goto new_goto(const Label &target) noexcept {
    return Goto::create(target, holder_);
  }

  // create a new 'if (cond) { then } else { else_ }'
  // the 'else' part can be omitted by specifying else_ = VoidExpr
  If new_if(const Expr &cond, const Node &then, const Node &else_) noexcept {
    return If::create(cond, then, else_, holder_);
  }

  // create a new conditional jump. usually only found in compiled code.
  JumpIf new_jump_if(const Label &to, const Expr &cond) noexcept {
    return JumpIf::create(to, cond, holder_);
  }

  // create a new local label, used for jumps within the function
  Label new_label() noexcept;

  Mem new_mem(Kind kind, const Expr &address) noexcept {
    return Mem::create(kind, address, holder_);
  }

  // create a new return statement
  Return new_return() noexcept {
    return new_return(Exprs{});
  }
  Return new_return(const Expr &expr) noexcept {
    return new_return(Exprs{&expr, 1});
  }
  Return new_return(std::initializer_list<Expr> exprs) noexcept {
    return new_return(Exprs{exprs.begin(), exprs.size()});
  }
  Return new_return(Exprs exprs) noexcept {
    return Return::create(exprs, holder_);
  }

  Stmt0 new_stmt0(OpStmt0 op) noexcept {
    return Stmt0{op};
  }

  // cases can contain at most one Default
  Switch new_switch(const Expr &expr, std::initializer_list<Case> cases) noexcept {
    return new_switch(expr, Cases{cases.begin(), cases.size()});
  }
  Switch new_switch(const Expr &expr, const Cases cases) noexcept {
    return Switch::create(expr, cases, holder_);
  }

  // create a new unary expression, overriding Kind autodetection.
  // needed by op == CAST
  Unary new_unary(Kind kind, Op1 op, const Expr &arg) noexcept {
    return Unary::create(kind, op, arg, holder_);
  }

  // also autodetects kind if op != CAST
  Unary new_unary(Op1 op, const Expr &arg) noexcept {
    return Unary::create(op, arg, holder_);
  }

  constexpr Node get_body() const noexcept {
    return body_;
  }

  Func &set_body(const Node &body) noexcept {
    body_var_n_ = vars_.size();
    body_ = body;
    return *this;
  }

  constexpr Node get_compiled() const noexcept {
    return compiled_;
  }

  Func &set_compiled(const Node &compiled) noexcept {
    compiled_var_n_ = vars_.size();
    compiled_ = compiled;
    return *this;
  }

private:
  // create a new local variable. called by Var{Func&, Kind}
  // and internally calls Var::create()
  Var new_var(Kind kind) noexcept;

private:
  Code *holder_;
  uint16_t param_n_;
  uint16_t result_n_;
  uint32_t body_var_n_;     // # local vars used by body_
  uint32_t compiled_var_n_; // # local vars used by compiled_

  FuncType ftype_;
  Vector<Var> vars_;
  Vector<Label> labels_;
  String name_;
  Node body_;
  Node compiled_;
};

} // namespace onejit

#endif // ONEJIT_FUNC_HPP
