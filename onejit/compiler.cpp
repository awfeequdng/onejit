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
 * compiler.cpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/compiler.hpp>
#include <onejit/func.hpp>

namespace onejit {

Compiler::Compiler(Func &func) noexcept
    : func_{func}, break_{}, continue_{}, node_{}, error_{}, good_{func} {
}

Compiler::~Compiler() noexcept {
}

Compiler::operator bool() const noexcept {
  return good_ && func_;
}

Compiler &Compiler::finish() noexcept {
  if (*this) {
    Node compiled;
    switch (node_.size()) {
    case 0:
      compiled = VoidExpr;
      break;
    case 1:
      compiled = node_[0];
      break;
    default:
      compiled = func_.new_block(node_);
      break;
    }
    func_.set_compiled(compiled);
  }
  return *this;
}

Label Compiler::label_break() const noexcept {
  return break_.empty() ? Label{} : break_.end()[-1];
}

Label Compiler::label_continue() const noexcept {
  return continue_.empty() ? Label{} : continue_.end()[-1];
}

Compiler &Compiler::enter_case(Label l_break) noexcept {
  good_ = good_ && break_.append(l_break);
  return *this;
}

Compiler &Compiler::exit_case() noexcept {
  good_ = good_ && !break_.empty();
  if (good_) {
    break_.truncate(break_.size() - 1);
  }
  return *this;
}

Compiler &Compiler::enter_loop(Label l_break, Label l_continue) noexcept {
  good_ = good_ && break_.append(l_break) && continue_.append(l_continue);
  return *this;
}

Compiler &Compiler::exit_loop() noexcept {
  good_ = good_ && !break_.empty() && !continue_.empty();
  if (good_) {
    break_.truncate(break_.size() - 1);
    continue_.truncate(continue_.size() - 1);
  }
  return *this;
}

Var Compiler::to_var(const Node &node) noexcept {
  Expr e = node.is<Expr>();
  Var ve = node.is<Var>();
  if (e && !ve) {
    // copy Expr result to a Var
    ve = func_.new_var(e.kind());
    compile_add(func_.new_assign(ASSIGN, ve, e), false);
  }
  return ve;
}

Compiler &Compiler::to_vars(const Node &node, uint32_t start, uint32_t end,
                            Vector<Expr> &vars) noexcept {
  for (size_t i = start; i < end; i++) {
    if (!vars.append(to_var(node.child(i)))) {
      return out_of_memory(node);
    }
  }
  return *this;
}

Compiler &Compiler::add(const Node &node) noexcept {
  if (node != VoidExpr) {
    good_ = good_ && node_.append(node);
  }
  return *this;
}

Compiler &Compiler::error(const Node &where, Chars msg) noexcept {
  good_ = good_ && error_.append(Error{where, msg});
  return *this;
}

Compiler &Compiler::out_of_memory(const Node &where) noexcept {
  // always set good_ to false
  good_ = error_.append(Error{where, "out of memory"}) && false;
  return *this;
}

////////////////////////////////////////////////////////////////////////////////

Compiler &Compiler::compile() noexcept {
  node_.clear();
  compile_add(func_.get_body(), false);
  return *this;
}

Node Compiler::compile(Node node, bool simplify_call) noexcept {
  const Type t = node.type();
  switch (t) {
  case STMT_0:
    return compile(node.is<Stmt0>(), simplify_call);
  case STMT_1:
    return compile(node.is<Stmt1>(), simplify_call);
  case STMT_2:
    return compile(node.is<Stmt2>(), simplify_call);
  case STMT_3:
    return compile(node.is<Stmt3>(), simplify_call);
  case STMT_4:
    return compile(node.is<Stmt4>(), simplify_call);
  case STMT_N:
    return compile(node.is<StmtN>(), simplify_call);
  default:
    if (const Expr expr = node.is<Expr>()) {
      return compile(expr, simplify_call);
    }
    return node;
  }
}

////////////////////////////////////////////////////////////////////////////////

Expr Compiler::compile(Expr expr, bool) noexcept {
  const Type t = expr.type();
  switch (t) {
  case MEM:
    return compile(expr.is<Mem>(), true);
  case UNARY:
    return compile(expr.is<Unary>(), true);
  case BINARY:
    return compile(expr.is<Binary>(), true);
  case CALL:
    return compile(expr.is<Call>(), true);
  case VAR:
  case LABEL:
  case CONST:
  default:
    return expr;
  }
}

Expr Compiler::compile(Mem expr, bool) noexcept {
  Expr addr = expr.addr();
  Expr comp_addr = compile(addr, true);
  if (addr != comp_addr) {
    expr = func_.new_mem(expr.kind(), comp_addr);
  }
  return expr;
}

Expr Compiler::compile(Unary expr, bool) noexcept {
  Expr x = expr.x();
  Expr comp_x = compile(x, true);
  if (x != comp_x) {
    expr = func_.new_unary(expr.op(), comp_x);
  }
  return expr;
}

Expr Compiler::compile(Binary expr, bool) noexcept {
  Expr x = expr.x(), y = expr.y();
  Expr comp_x = compile(x, true), comp_y = compile(y, true);
  if (x != comp_x || y != comp_y) {
    expr = func_.new_binary(expr.op(), comp_x, comp_y);
  }
  return expr;
}

Expr Compiler::compile(Call call, bool simplify_call) noexcept {
  const uint32_t n = call.children();

  if (!call.children_are<Var>(2, n)) {
    Vector<Expr> vargs;
    // convert all arguments to Var
    to_vars(call, 2, n, vargs);
    call = func_.new_call(call.ftype(), call.label(), vargs);
  }

  if (!simplify_call) {
    return call;
  }
  // avoid calls inside other expressions,
  // and copy result value to a Var.
  //
  // we could also use to_var(call), but it risks
  // infinite recursion because it invokes compile(call)
  Var dst = func_.new_var(call.kind());
  add(func_.new_assign(ASSIGN, dst, call));
  return dst;
}

// ===============================  compile(Stmt0)  ============================

Node Compiler::compile(Stmt0 st, bool) noexcept {
  switch (st.op()) {
  case BREAK:
    if (Label l = label_break()) {
      compile_add(func_.new_goto(l), false);
    } else {
      error(st, "misplaced Break");
    }
    break;
  case CONTINUE:
    if (Label l = label_continue()) {
      compile_add(func_.new_goto(l), false);
    } else {
      error(st, "misplaced Continue");
    }
    break;
  case FALLTHROUGH:
    error(st, "misplaced Fallthrough");
    break;
  case BAD:
  default:
    error(st, "bad Stmt0");
    break;
  }
  // all compile(Stmt*) must return VoidExpr
  return VoidExpr;
}

// ===============================  compile(Stmt1)  ============================

Node Compiler::compile(Stmt1 st, bool) noexcept {
  add(st);
  // all compile(Stmt*) must return VoidExpr
  return VoidExpr;
}

// ===============================  compile(Stmt2)  ============================

Node Compiler::compile(Stmt2 st, bool simplify_call) noexcept {
  switch (st.op()) {
  case CASE:
    error(st, "misplaced Case");
    break;
  case DEFAULT:
    error(st, "misplaced Default");
    break;
  case JUMP_IF:
    return compile(st.is<JumpIf>(), simplify_call);
  default:
    if (auto assign = st.is<Assign>()) {
      return compile(assign, simplify_call);
    }
    break;
  }
  add(st);
  // all compile(Stmt*) must return VoidExpr
  return VoidExpr;
}

Node Compiler::compile(Assign assign, bool) noexcept {
  Expr src = assign.src();
  Expr dst = assign.dst();
  // compile src first: its side effects, if any, must be applied before dst
  Expr comp_src = compile(src, false);
  Expr comp_dst = compile(dst, false);
  if (src != comp_src || dst != comp_src) {
    assign = func_.new_assign(assign.op(), comp_dst, comp_src);
  }
  add(assign);
  // all compile(Stmt*) must return VoidExpr
  return VoidExpr;
}

Node Compiler::compile(JumpIf jump_if, bool) noexcept {
  Expr cond = jump_if.cond();
  Expr comp_cond = compile(cond, true);
  if (cond != comp_cond) {
    jump_if = func_.new_jump_if(jump_if.to(), comp_cond);
  }
  add(jump_if);
  // all compile(Stmt*) must return VoidExpr
  return VoidExpr;
}

// ===============================  compile(Stmt3)  ============================

Node Compiler::compile(Stmt3 st, bool) noexcept {
  switch (st.op()) {
  case IF:
    return compile(st.is<If>(), false);
  default:
    add(st);
    return VoidExpr;
  }
}

Node Compiler::compile(If st, bool) noexcept {
  Expr cond = func_.new_unary(NOT1, st.cond());
  cond = compile(cond, false);

  Node then = st.then();
  Node else_ = st.else_();
  bool have_else = else_.type() != CONST;

  Label else_label = have_else ? func_.new_label() : Label{};
  Label endif_label = func_.new_label();

  JumpIf jump_if = func_.new_jump_if(have_else ? else_label : endif_label, cond);

  compile_add(jump_if, false) //
      .compile_add(then, false);
  if (have_else) {
    compile_add(func_.new_goto(endif_label), false) //
        .compile_add(else_label, false)
        .compile_add(else_, false);
  }
  compile_add(endif_label, false);
  return VoidExpr;
}

// ===============================  compile(Stmt4)  ============================

Node Compiler::compile(Stmt4 st, bool) noexcept {
  switch (st.op()) {
  case FOR:
    return compile(st.is<For>(), false);
  default:
    add(st);
    return VoidExpr;
  }
}

Node Compiler::compile(For st, bool) noexcept {
  /// TODO: implement
  add(st);
  return VoidExpr;
}

// ===============================  compile(StmtN)  ============================

Node Compiler::compile(StmtN st, bool simplify_call) noexcept {
  switch (st.op()) {
  case ASSIGN_TUPLE:
    return compile(st.is<AssignCall>(), simplify_call);
  case BLOCK:
    return compile(st.is<Block>(), simplify_call);
  case COND:
    return compile(st.is<Cond>(), simplify_call);
  case RETURN:
    return compile(st.is<Return>(), simplify_call);
  case SWITCH:
    return compile(st.is<Switch>(), simplify_call);
  default:
    add(st);
    return VoidExpr;
  }
}

Node Compiler::compile(AssignCall st, bool) noexcept {
  /// TODO: implement
  add(st);
  return VoidExpr;
}

Node Compiler::compile(Block st, bool) noexcept {
  for (size_t i = 0, n = st.children(); i < n; i++) {
    compile(st.child(i), false);
  }
  return VoidExpr;
}

Node Compiler::compile(Cond st, bool) noexcept {
  /// TODO: implement
  add(st);
  return VoidExpr;
}

Node Compiler::compile(Return st, bool) noexcept {
  const size_t n = st.children();
  if (n != func_.result_n()) {
    error(st, "bad number of return values");
    add(st);
    return VoidExpr;
  }
  bool uses_func_result = true;
  for (size_t i = 0; uses_func_result && i < n; i++) {
    uses_func_result = st.child(i) == func_.result(i);
  }
  if (uses_func_result) {
    // nothing to do
    add(st);
    return VoidExpr;
  }
  Buffer<Expr> vars;
  for (size_t i = 0; i < n; i++) {
    Var var = func_.result(i);
    Expr expr = st.child(i).is<Expr>();
    if (expr != var) {
      // compile expression and copy its result
      // to expected location func_.result(i)
      expr = compile(expr, false);
      add(func_.new_assign(ASSIGN, var, expr));
    }
    vars.append(var);
  }
  if (!vars) {
    out_of_memory(st);
  }
  add(func_.new_return(vars));
  return VoidExpr;
}

Node Compiler::compile(Switch st, bool) noexcept {
  /// TODO: implement
  add(st);
  return VoidExpr;
}

} // namespace onejit
