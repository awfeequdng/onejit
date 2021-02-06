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

#include <onejit/binary.hpp>
#include <onejit/call.hpp>
#include <onejit/compiler.hpp>
#include <onejit/const.hpp>
#include <onejit/func.hpp>
#include <onejit/mem.hpp>
#include <onejit/stmt0.hpp>
#include <onejit/stmt1.hpp>
#include <onejit/stmt2.hpp>
#include <onejit/stmt3.hpp>
#include <onejit/stmt4.hpp>
#include <onejit/stmtn.hpp>
#include <onejit/unary.hpp>

namespace onejit {

Compiler::Compiler() noexcept
    : func_{}, break_{}, continue_{}, fallthrough_{}, node_{}, error_{}, good_{true} {
}

Compiler::~Compiler() noexcept {
}

Compiler::operator bool() const noexcept {
  return good_ && func_ && *func_;
}

Compiler &Compiler::compile(Func &func) noexcept {
  func_ = &func;
  break_.clear();
  continue_.clear();
  fallthrough_.clear();
  node_.clear();
  error_.clear();
  good_ = bool(func);
  return compile_add(func.get_body(), false) //
      .finish();
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
      compiled = Block{*func_, node_};
      break;
    }
    func_->set_compiled(compiled);
  }
  return *this;
}

////////////////////////////////////////////////////////////////////////////////

Label Compiler::label_break() const noexcept {
  return break_.empty() ? Label{} : break_.end()[-1];
}

Label Compiler::label_continue() const noexcept {
  return continue_.empty() ? Label{} : continue_.end()[-1];
}

Label Compiler::label_fallthrough() const noexcept {
  return fallthrough_.empty() ? Label{} : fallthrough_.end()[-1];
}

Compiler &Compiler::enter_case(Label l_break, Label l_fallthrough) noexcept {
  good_ = good_ && break_.append(l_break) && fallthrough_.append(l_fallthrough);
  return *this;
}

Compiler &Compiler::exit_case() noexcept {
  good_ = good_ && !break_.empty() && !fallthrough_.empty();
  if (good_) {
    break_.truncate(break_.size() - 1);
    fallthrough_.truncate(fallthrough_.size() - 1);
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
  Var v = node.is<Var>();
  if (e && !v) {
    // copy Expr result to a Var
    v = Var{*func_, e.kind()};
    compile_add(Assign{*func_, ASSIGN, v, e}, false);
  }
  return v;
}

Compiler &Compiler::to_vars(const Node &node, uint32_t start, uint32_t end,
                            Vector<Expr> &vars) noexcept {
  if (!vars.resize(end > start ? end - start : 0)) {
    return out_of_memory(node);
  }
  for (size_t i = start; i < end; i++) {
    vars[i - start] = to_var(node.child(i));
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
  good_ = good_ && error_.append(Error{where, "out of memory"}) && false;
  return *this;
}

////////////////////////////////////////////////////////////////////////////////

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
  case UNARY:
    return compile(expr.is<Unary>(), true);
  case BINARY:
    return compile(expr.is<Binary>(), true);
  case MEM:
  case TUPLE:
    return compile(expr.is<Tuple>(), true);
  case VAR:
  case LABEL:
  case CONST:
  default:
    return expr;
  }
}

Expr Compiler::compile(Unary expr, bool) noexcept {
  Expr x = expr.x();
  Expr comp_x = compile(x, true);
  if (x != comp_x) {
    expr = Unary{*func_, expr.op(), comp_x};
  }
  return expr;
}

Expr Compiler::compile(Binary expr, bool) noexcept {
  Expr x = expr.x(), y = expr.y();
  Expr comp_x = compile(x, true), comp_y = compile(y, true);
  if (x != comp_x || y != comp_y) {
    expr = Binary{*func_, expr.op(), comp_x, comp_y};
  }
  return expr;
}

Expr Compiler::compile(Tuple expr, bool simplify_call) noexcept {
  if (Call call = expr.is<Call>()) {
    return compile(call, simplify_call);
  }
  Vector<Node> nodes;
  const size_t n = expr.children();
  if (!nodes.resize(n)) {
    out_of_memory(expr);
    return expr;
  }
  bool changed = false;
  for (size_t i = 0; i < n; i++) {
    Node child = expr.child(i);
    Node comp_child = nodes[i] = compile(child, simplify_call);
    changed = changed || child != comp_child;
  }
  if (changed) {
    return Tuple{*func_, expr.kind(), expr.op(), nodes};
  }
  return expr;
}

Expr Compiler::compile(Call call, bool simplify_call) noexcept {
  const uint32_t n = call.children();

  if (!call.children_are<Var>(2, n)) {
    Vector<Expr> vargs;
    // convert all call arguments to Var
    to_vars(call, 2, n, vargs);
    call = Call{*func_, call.ftype(), call.label(), vargs};
  }

  if (!simplify_call) {
    return call;
  }
  // avoid calls inside other expressions,
  // and copy result value to a Var.
  //
  // we could also use to_var(call), but it risks
  // infinite recursion because it invokes compile(call)
  Var dst{*func_, call.kind()};
  add(Assign{*func_, ASSIGN, dst, call});
  return dst;
}

// ===============================  compile(Stmt0)  ============================

Node Compiler::compile(Stmt0 st, bool) noexcept {
  switch (st.op()) {
  case BREAK:
    if (Label l = label_break()) {
      compile_add(Goto{*func_, l}, false);
    } else {
      error(st, "misplaced Break");
    }
    break;
  case CONTINUE:
    if (Label l = label_continue()) {
      compile_add(Goto{*func_, l}, false);
    } else {
      error(st, "misplaced Continue");
    }
    break;
  case FALLTHROUGH:
    if (Label l = label_fallthrough()) {
      compile_add(Goto{*func_, l}, false);
    } else {
      error(st, "misplaced Fallthrough");
    }
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
  Node body = st.body();
  Node comp_body = compile(body, false);
  if (body != comp_body) {
    st = Stmt1{*func_, comp_body, st.op()};
  }
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
    assign = Assign{*func_, assign.op(), comp_dst, comp_src};
  }
  add(assign);
  // all compile(Stmt*) must return VoidExpr
  return VoidExpr;
}

static OpStmt1 comparison_to_condjump(Op2 op2, bool is_unsigned) noexcept {
  OpStmt1 op = BAD_ST1;
  switch (op2) {
  case LSS:
    op = is_unsigned ? ASM_JB : ASM_JL;
    break;
  case LEQ:
    op = is_unsigned ? ASM_JBE : ASM_JLE;
    break;
  case NEQ:
    op = ASM_JNE;
    break;
  case EQL:
    op = ASM_JE;
    break;
  case GTR:
    op = is_unsigned ? ASM_JA : ASM_JG;
    break;
  case GEQ:
    op = is_unsigned ? ASM_JAE : ASM_JGE;
    break;
  default:
    break;
  }
  return op;
}

Node Compiler::compile(JumpIf jump_if, bool) noexcept {
  Label to = jump_if.to();
  Expr test = compile(jump_if.test(), true);
  Expr x, y;
  OpStmt1 op = BAD_ST1;
  bool negate = false;
  while (Unary expr = test.is<Unary>()) {
    if (expr.op() != NOT1) {
      break;
    }
    negate = !negate;
    test = expr.x();
  }
  if (Binary expr = test.is<Binary>()) {
    x = expr.x();
    y = expr.y();
    op = comparison_to_condjump(expr.op(), x.kind().is_unsigned());
  }
  if (op == BAD_ST1) {
    x = test;
    y = Zero(test.kind());
    op = ASM_JNE;
  }

  if (negate) {
    op = negate_condjump(op);
  }
  add(Stmt2{*func_, x, y, ASM_CMP});
  add(Stmt1{*func_, to, op});
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
  Node then = st.then();
  Node else_ = st.else_();
  bool have_else = else_.type() != CONST;

  Label else_label{*func_};
  Label endif_label = have_else ? Label{*func_} : else_label;

  Expr test = Unary{*func_, NOT1, st.test()};
  test = compile(test, false);
  JumpIf jump_if{*func_, else_label, test};

  compile_add(jump_if, false) //
      .compile_add(then, false);
  if (have_else) {
    compile_add(Goto{*func_, endif_label}, false) //
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
  compile_add(st.init(), false);

  Expr test = st.test();
  bool have_test = test != VoidExpr;

  Label l_loop{*func_};
  Label l_continue = have_test ? Label{*func_} : l_loop;
  Label l_break{*func_};

  if (have_test) {
    add(Goto{*func_, l_continue});
  }
  add(l_loop);
  enter_loop(l_break, l_continue);
  compile_add(st.body(), false);
  compile_add(st.post(), false);
  exit_loop();

  if (have_test) {
    add(l_continue);
    compile_add(JumpIf{*func_, l_loop, test}, false);
  } else {
    compile_add(Goto{*func_, l_loop}, false);
  }
  add(l_break);
  return VoidExpr;
}

// ===============================  compile(StmtN)  ============================

Node Compiler::compile(StmtN st, bool simplify_call) noexcept {
  switch (st.op()) {
  case ASSIGN_CALL:
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
  while (const size_t n = st.children()) {
    Call call = st.child(n - 1).is<Call>();
    Call comp_call = compile(call, false).is<Call>();
    if (call == comp_call && st.children_are<Var>(0, n - 1)) {
      break;
    }
    Vector<Expr> vars;
    to_vars(st, 0, n - 1, vars);
    st = AssignCall{*func_, vars, comp_call};
    break;
  }
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
  const size_t n = st.children();
  if (n == 0) {
    // nothing to do
  } else if (n & 1) {
    error(st, "unexpected odd number of children in Cond: expecting an even number of them");
    add(st);
  } else {
    Label l_end{*func_};
    Goto goto_end = n <= 2 ? Goto{} : Goto{*func_, l_end};
    for (size_t i = 0; i < n; i += 2) {
      bool is_last = i + 2 >= n;
      Label l_next = is_last ? l_end : Label{*func_};
      compile_add(JumpIf{*func_, l_next, //
                         Unary{*func_, NOT1, st.child_is<Expr>(i)}},
                  false);
      compile_add(st.child(i + 1), false);
      if (!is_last && goto_end) {
        add(goto_end);
      }
      add(l_next);
    }
  }
  return VoidExpr;
}

Node Compiler::compile(Return st, bool) noexcept {
  const size_t n = st.children();
  if (n != func_->result_n()) {
    error(st, "bad number of return values");
    add(st);
    return VoidExpr;
  }
  bool uses_func_result = true;
  for (size_t i = 0; uses_func_result && i < n; i++) {
    uses_func_result = st.child(i) == func_->result(i);
  }
  if (uses_func_result) {
    // nothing to do
    add(st);
    return VoidExpr;
  }
  Buffer<Expr> vars;
  for (size_t i = 0; i < n; i++) {
    Var var = func_->result(i);
    Expr expr = st.child_is<Expr>(i);
    if (expr != var) {
      // compile expression and copy its result
      // to expected location func_->result(i)
      expr = compile(expr, false);
      add(Assign{*func_, ASSIGN, var, expr});
    }
    vars.append(var);
  }
  if (!vars) {
    out_of_memory(st);
  }
  add(Return{*func_, vars});
  return VoidExpr;
}

Node Compiler::compile(Switch st, bool) noexcept {
  const size_t n = st.children();
  bool have_default = false;

  for (size_t i = 1; i < n; i++) {
    Node node_i = st.child(i);
    Case case_i = node_i.is<Case>();
    if (!case_i) {
      error(node_i, "unexpected node in Switch, expecting Case or Default");
      return VoidExpr;
    }
    if (case_i.op() == DEFAULT) {
      if (have_default) {
        error(node_i, "unexpected duplicate Default in Switch, expecting at most one");
        return VoidExpr;
      }
      have_default = true;
    }
  }

  Expr expr = to_var(st.expr());

  Label l_next;
  Label l_fallthrough;
  Label l_this;
  Label l_this_fallthrough;
  Label l_default;
  Label l_break{*func_};
  Goto goto_break{*func_, l_break};

  // skip child(0) = expr
  for (size_t i = 1; i < n; i++) {
    Case case_i = st.child_is<Case>(i);
    bool is_last = i + 1 >= n;
    if (is_last) {
      l_next = l_default ? l_default : l_break;
      l_fallthrough = l_break; // 'fallthrough' in last Case currently does 'break'
    } else {
      l_next = Label{*func_};
      l_fallthrough = Label{*func_};
    }
    if (l_this) {
      add(l_this);
    }
    if (case_i.op() == DEFAULT) {
      if (!is_last) {
        // non-last Default, can be entered only if no other case matches
        add(Goto{*func_, l_next});
        if (!l_this_fallthrough) {
          l_this_fallthrough = Label{*func_};
        }
        // remember label to execute Default
        l_default = l_this_fallthrough;
      }
    } else {
      compile_add(JumpIf{*func_, l_next, Binary{*func_, NEQ, expr, case_i.expr()}}, false);
    }
    if (l_this_fallthrough) {
      add(l_this_fallthrough);
    }
    enter_case(l_break, l_fallthrough);
    compile_add(case_i.body(), false);
    exit_case();
    // automatically add 'break' at the end of each case.
    // can be overridden, by adding 'fallthrough' as last statement in Case{...}
    // to restore C/C++ behavior
    if (!is_last) {
      add(goto_break);
    }
    l_this = l_next;
    l_this_fallthrough = l_fallthrough;
  }
  add(l_break);
  return VoidExpr;
} // namespace onejit

} // namespace onejit
