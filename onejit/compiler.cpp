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
#include <onejit/eval.hpp>
#include <onejit/func.hpp>
#include <onejit/node/binary.hpp>
#include <onejit/node/call.hpp>
#include <onejit/node/const.hpp>
#include <onejit/node/mem.hpp>
#include <onejit/node/stmt0.hpp>
#include <onejit/node/stmt1.hpp>
#include <onejit/node/stmt2.hpp>
#include <onejit/node/stmt3.hpp>
#include <onejit/node/stmt4.hpp>
#include <onejit/node/stmtn.hpp>
#include <onejit/node/unary.hpp>

namespace onejit {

enum Compiler::Flags : uint8_t {
  SimplifyNone = 0,
  SimplifyCall = 1 << 0,
  SimplifyLandLor = 1 << 1,
  SimplifyAll = 0x3,
  SimplifyDefault = SimplifyLandLor,
};

constexpr inline Compiler::Flags operator~(Compiler::Flags a) noexcept {
  return Compiler::Flags(~unsigned(a));
}
constexpr inline Compiler::Flags operator&(Compiler::Flags a, Compiler::Flags b) noexcept {
  return Compiler::Flags(unsigned(a) & unsigned(b));
}
constexpr inline Compiler::Flags operator|(Compiler::Flags a, Compiler::Flags b) noexcept {
  return Compiler::Flags(unsigned(a) | unsigned(b));
}
constexpr inline Compiler::Flags operator^(Compiler::Flags a, Compiler::Flags b) noexcept {
  return Compiler::Flags(unsigned(a) ^ unsigned(b));
}

inline Compiler::Flags &operator&=(Compiler::Flags &a, Compiler::Flags b) noexcept {
  return a = a & b;
}
inline Compiler::Flags &operator|=(Compiler::Flags &a, Compiler::Flags b) noexcept {
  return a = a | b;
}
inline Compiler::Flags &operator^=(Compiler::Flags &a, Compiler::Flags b) noexcept {
  return a = a ^ b;
}

////////////////////////////////////////////////////////////////////////////////

Compiler::Compiler() noexcept
    : optimizer_{}, func_{}, break_{}, continue_{}, fallthrough_{}, //
      node_{}, error_{}, good_{true} {
}

Compiler::~Compiler() noexcept {
}

Compiler::operator bool() const noexcept {
  return good_ && func_ && *func_;
}

Compiler &Compiler::compile(Func &func, Opt flags) noexcept {
  if (func.get_compiled(NOARCH)) {
    // already compiled
    return *this;
  }

  func_ = &func;
  break_.clear();
  continue_.clear();
  fallthrough_.clear();
  node_.clear();
  error_.clear();
  good_ = bool(func);

  Node node = optimizer_.optimize(func, func.get_body(), flags);

  return compile_add(node, SimplifyDefault).finish();
}

Compiler &Compiler::finish() noexcept {
  if (*this) {
    Node compiled;
    switch (node_.size()) {
    case 0:
      compiled = VoidConst;
      break;
    case 1:
      compiled = node_.get(0);
      break;
    default:
      compiled = Block{*func_, node_};
      break;
    }
    func_->set_compiled(NOARCH, compiled);
  }
  return *this;
}

////////////////////////////////////////////////////////////////////////////////

Node Compiler::compile(Node node, Flags flags) noexcept {
  const Type t = node.type();
  switch (t) {
  case STMT_0:
    return compile(node.is<Stmt0>(), flags);
  case STMT_1:
    return compile(node.is<Stmt1>(), flags);
  case STMT_2:
    return compile(node.is<Stmt2>(), flags);
  case STMT_3:
    return compile(node.is<Stmt3>(), flags);
  case STMT_4:
    return compile(node.is<Stmt4>(), flags);
  case STMT_N:
    return compile(node.is<StmtN>(), flags);
  default:
    if (Expr expr = node.is<Expr>()) {
      return compile(expr, flags);
    }
    return node;
  }
}

Expr Compiler::compile(Expr expr, Flags) noexcept {
  const Type t = expr.type();
  switch (t) {
  case UNARY:
    return compile(expr.is<Unary>(), SimplifyAll);
  case BINARY:
    return compile(expr.is<Binary>(), SimplifyAll);
  case MEM:
  case TUPLE:
    return compile(expr.is<Tuple>(), SimplifyAll);
  case VAR:
  case LABEL:
  case CONST:
  default:
    return expr;
  }
}

Expr Compiler::compile(Unary expr, Flags) noexcept {
  Expr x = expr.x();
  Expr comp_x = compile(x, SimplifyAll);
  if (x != comp_x) {
    expr = Unary{*func_, expr.op(), comp_x};
  }
  return expr;
}

Expr Compiler::compile(Binary expr, Flags flags) noexcept {
  Expr x = expr.x(), y = expr.y();
  Op2 op = expr.op();
  Expr comp_x = compile(x, SimplifyAll);
  Expr comp_y = compile(y, SimplifyAll);
  bool changed = x != comp_x || y != comp_y;
  x = comp_x;
  y = comp_y;
  if ((flags & SimplifyLandLor)) {
    if (Expr expr2 = simplify_boolean(op, x, y)) {
      return expr2;
    }
  }
  if (changed) {
    expr = Binary{*func_, op, x, y};
  }
  return expr;
}

Expr Compiler::simplify_boolean(Op2 op, Expr x, Expr y) noexcept {
  if (op == LAND) {
    return simplify_land(x, y);
  } else if (op == LOR) {
    return simplify_lor(x, y);
  }
  return Expr{};
}

Expr Compiler::simplify_land(Expr x, Expr y) noexcept {
  if (Const cx = x.is<Const>()) {
    return cx.val() ? y : x;
  }
  if (Const cy = y.is<Const>()) {
    if (cy.val()) {
      return x;
    } else {
      add(x);
      return y;
    }
  }
  if (y.is<Var>()) {
    return Expr{}; // no simplification
  }
  Var dst{*func_, Bool};
  Label out{*func_};
  if (x.type() == BINARY && is_comparison(Op2(x.op()))) {
    add(Assign{*func_, ASSIGN, dst, FalseExpr});
    compile_add(JumpIf{*func_, out, Unary{*func_, NOT1, x}}, SimplifyDefault);
  } else {
    add(Assign{*func_, ASSIGN, dst, x});
    compile_add(JumpIf{*func_, out, Unary{*func_, NOT1, dst}}, SimplifyDefault);
  }
  compile_add(Assign{*func_, ASSIGN, dst, y}, SimplifyDefault);
  add(out);
  return dst;
}

Expr Compiler::simplify_lor(Expr x, Expr y) noexcept {
  if (Const cx = x.is<Const>()) {
    return cx.val() ? x : y;
  }
  if (Const cy = y.is<Const>()) {
    if (cy.val()) {
      add(x);
      return y;
    } else {
      return x;
    }
  }
  if (y.is<Var>()) {
    return Expr{}; // no simplification
  }
  Var dst{*func_, Bool};
  Label out{*func_};
  if (x.type() == BINARY && is_comparison(Op2(x.op()))) {
    add(Assign{*func_, ASSIGN, dst, TrueExpr});
    compile_add(JumpIf{*func_, out, x}, SimplifyDefault);
  } else {
    add(Assign{*func_, ASSIGN, dst, x});
    compile_add(JumpIf{*func_, out, dst}, SimplifyDefault);
  }
  compile_add(Assign{*func_, ASSIGN, dst, y}, SimplifyDefault);
  add(out);
  return dst;
}

Expr Compiler::compile(Tuple expr, Flags flags) noexcept {
  if (Call call = expr.is<Call>()) {
    return compile(call, flags);
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
    Node comp_child = compile(child, flags);
    nodes.set(i, comp_child);
    changed = changed || child != comp_child;
  }
  if (changed) {
    expr = Tuple{*func_, expr.kind(), expr.op(), nodes};
  }
  return expr;
}

Expr Compiler::compile(Call call, Flags flags) noexcept {
  const uint32_t n = call.children();

  if (!call.children_are<Var>(2, n)) {
    Vector<Expr> vargs;
    // convert all call arguments to Var
    to_vars(call, 2, n, vargs);
    call = Call{*func_, call.ftype(), call.label(), vargs};
  }

  if (!(flags & SimplifyCall)) {
    return call;
  }
  // avoid calls inside other expressions,
  // and copy first result to a Var.
  //
  // we could also use to_var(call), but it risks
  // infinite recursion because it invokes compile(Node)
  Var dst{*func_, call.kind()};
  add(Assign{*func_, ASSIGN, dst, call});
  return dst;
}

// ===============================  compile(Stmt0)  ============================

Node Compiler::compile(Stmt0 st, Flags) noexcept {
  switch (st.op()) {
  case BREAK:
    if (Label l = label_break()) {
      add(Goto{*func_, l});
    } else {
      error(st, "misplaced Break");
    }
    break;
  case CONTINUE:
    if (Label l = label_continue()) {
      add(Goto{*func_, l});
    } else {
      error(st, "misplaced Continue");
    }
    break;
  case FALLTHROUGH:
    if (Label l = label_fallthrough()) {
      add(Goto{*func_, l});
    } else {
      error(st, "misplaced Fallthrough");
    }
    break;
  case BAD:
  default:
    error(st, "bad Stmt0");
    break;
  }
  // all compile(Stmt*) must return VoidConst
  return VoidConst;
}

// ===============================  compile(Stmt1)  ============================

Node Compiler::compile(Stmt1 st, Flags) noexcept {
  Expr expr = st.arg();
  Expr comp_expr = compile(expr, SimplifyDefault);
  if (expr != comp_expr) {
    st = Stmt1{*func_, comp_expr, st.op()};
  }
  add(st);
  // all compile(Stmt*) must return VoidConst
  return VoidConst;
}

// ===============================  compile(Stmt2)  ============================

Node Compiler::compile(Stmt2 st, Flags flags) noexcept {
  switch (st.op()) {
  case CASE:
    error(st, "misplaced Case");
    break;
  case DEFAULT:
    error(st, "misplaced Default");
    break;
  case JUMP_IF:
    return compile(st.is<JumpIf>(), flags);
  default:
    if (Assign assign = st.is<Assign>()) {
      return compile(assign, flags);
    }
    break;
  }
  add(st);
  // all compile(Stmt*) must return VoidConst
  return VoidConst;
}

Node Compiler::compile(Assign assign, Flags) noexcept {
  Expr src = assign.src();
  Expr dst = assign.dst();
  // compile src first: its side effects, if any, must be applied before dst
  Expr comp_src = compile(src, SimplifyAll);
  Expr comp_dst = compile(dst, SimplifyDefault);
  if (src != comp_src || dst != comp_src) {
    assign = Assign{*func_, assign.op(), comp_dst, comp_src};
  }
  add(assign);
  // all compile(Stmt*) must return VoidConst
  return VoidConst;
}

static OpStmt1 comparison_to_condjump(Op2 op2, bool is_signed) noexcept {
  OpStmt1 op = BAD_ST1;
  switch (op2) {
  case LSS:
    op = is_signed ? ASM_JL : ASM_JB;
    break;
  case LEQ:
    op = is_signed ? ASM_JLE : ASM_JBE;
    break;
  case NEQ:
    op = ASM_JNE;
    break;
  case EQL:
    op = ASM_JE;
    break;
  case GTR:
    op = is_signed ? ASM_JG : ASM_JA;
    break;
  case GEQ:
    op = is_signed ? ASM_JGE : ASM_JAE;
    break;
  default:
    break;
  }
  return op;
}

Node Compiler::compile(JumpIf jump_if, Flags) noexcept {
  Label to = jump_if.to();
  // preserve any binary comparison, it's optimized below
  Expr test = compile(jump_if.test(), SimplifyAll & ~SimplifyLandLor);
  Expr x, y;
  Op2 op = BAD2;
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
    op = expr.op();
  }
  if (op == BAD2) {
    x = test;
    y = Zero(test.kind());
    op = NEQ;
  }
  if (Const cx = x.is<Const>()) {
    if (Const cy = y.is<Const>()) {
      // test is a constant,
      // optimize to unconditional jump
      Value v = eval_binary(op, cx.val(), cy.val());
      if (v.is_valid()) {
        if (negate ? !v : v) {
          add(Goto{*func_, to});
        }
        return VoidConst;
      }
    }
  }
  OpStmt1 jop = comparison_to_condjump(op, x.kind().is_signed());
  if (negate) {
    jop = negate_condjump(jop);
  }
  add(Stmt2{*func_, x, y, ASM_CMP});
  add(Stmt1{*func_, to, jop});
  // all compile(Stmt*) must return VoidConst
  return VoidConst;
}

// ===============================  compile(Stmt3)  ============================

Node Compiler::compile(Stmt3 st, Flags flags) noexcept {
  switch (st.op()) {
  case IF:
    return compile(st.is<If>(), flags);
  default:
    add(st);
    return VoidConst;
  }
}

Node Compiler::compile(If st, Flags) noexcept {
  Node then = st.then();
  Node else_ = st.else_();
  Expr test = compile(st.test(), SimplifyDefault);
  if (Const ctest = test.is<Const>()) {
    compile_add(ctest.val() ? then : else_, SimplifyDefault);
    return VoidConst;
  }
  bool have_else = else_.type() != CONST;
  Label else_label{*func_};
  Label endif_label = have_else ? Label{*func_} : else_label;

  test = Unary{*func_, NOT1, test};
  JumpIf jump_if{*func_, else_label, test};

  compile_add(jump_if, SimplifyDefault) //
      .compile_add(then, SimplifyDefault);
  if (have_else) {
    add(Goto{*func_, endif_label}) //
        .add(else_label)
        .compile_add(else_, SimplifyDefault);
  }
  add(endif_label);
  return VoidConst;
}

// ===============================  compile(Stmt4)  ============================

Node Compiler::compile(Stmt4 st, Flags flags) noexcept {
  switch (st.op()) {
  case FOR:
    return compile(st.is<For>(), flags);
  default:
    add(st);
    return VoidConst;
  }
}

Node Compiler::compile(For st, Flags) noexcept {
  compile_add(st.init(), SimplifyDefault);

  Expr test = st.test();
  bool have_test = true;
  if (Const ctest = test.is<Const>()) {
    have_test = test != VoidExpr || !ctest.val();
    if (have_test) {
      // test is always false: compile only st.init()
      return VoidConst;
    }
  }

  Label l_loop{*func_};
  Label l_continue = have_test ? Label{*func_} : l_loop;
  Label l_break{*func_};

  if (have_test) {
    add(Goto{*func_, l_continue});
  }
  add(l_loop);
  enter_loop(l_break, l_continue);
  compile_add(st.body(), SimplifyDefault);
  compile_add(st.post(), SimplifyDefault);
  exit_loop();

  if (have_test) {
    add(l_continue);
    compile_add(JumpIf{*func_, l_loop, test}, SimplifyDefault);
  } else {
    compile_add(Goto{*func_, l_loop}, SimplifyDefault);
  }
  add(l_break);
  return VoidConst;
}

// ===============================  compile(StmtN)  ============================

Node Compiler::compile(StmtN st, Flags flags) noexcept {
  switch (st.op()) {
  case ASSIGN_CALL:
    return compile(st.is<AssignCall>(), flags);
  case BLOCK:
    return compile(st.is<Block>(), flags);
  case COND:
    return compile(st.is<Cond>(), flags);
  case RETURN:
    return compile(st.is<Return>(), flags);
  case SWITCH:
    return compile(st.is<Switch>(), flags);
  default:
    add(st);
    return VoidConst;
  }
}

Node Compiler::compile(AssignCall st, Flags) noexcept {
  while (const size_t n = st.children()) {
    Call call = st.child(n - 1).is<Call>();
    Call comp_call = compile(call, SimplifyDefault).is<Call>();
    if (call == comp_call && st.children_are<Var>(0, n - 1)) {
      break;
    }
    Vector<Expr> vars;
    to_vars(st, 0, n - 1, vars);
    st = AssignCall{*func_, vars, comp_call};
    break;
  }
  add(st);
  return VoidConst;
}

Node Compiler::compile(Block st, Flags) noexcept {
  for (size_t i = 0, n = st.children(); i < n; i++) {
    compile(st.child(i), SimplifyDefault);
  }
  return VoidConst;
}

Node Compiler::compile(Cond st, Flags) noexcept {
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
                  SimplifyDefault);
      compile_add(st.child(i + 1), SimplifyDefault);
      if (!is_last && goto_end) {
        add(goto_end);
      }
      add(l_next);
    }
  }
  return VoidConst;
}

Node Compiler::compile(Return st, Flags) noexcept {
  const size_t n = st.children();
  if (n != func_->result_n()) {
    error(st, "number of return values does not match function signature");
    add(st);
    return VoidConst;
  }
  bool uses_func_result = true;
  for (size_t i = 0; uses_func_result && i < n; i++) {
    uses_func_result = st.child(i) == func_->result(i);
  }
  if (uses_func_result) {
    // nothing to do
    add(st);
    return VoidConst;
  }
  Buffer<Expr> vars;
  for (size_t i = 0; i < n; i++) {
    Var var = func_->result(i);
    Expr expr = st.child_is<Expr>(i);
    if (var.kind() != expr.kind()) {
      error(st, "return value kind does not match function signature");
    }
    if (expr != var) {
      // compile expression and copy its result
      // to expected location func_->result(i)
      expr = compile(expr, SimplifyDefault);
      add(Assign{*func_, ASSIGN, var, expr});
    }
    vars.append(var);
  }
  if (!vars) {
    out_of_memory(st);
  }
  add(Return{*func_, vars});
  return VoidConst;
}

Node Compiler::compile(Switch st, Flags) noexcept {
  const size_t n = st.children();
  bool have_default = false;

  for (size_t i = 1; i < n; i++) {
    Node node_i = st.child(i);
    Case case_i = node_i.is<Case>();
    if (!case_i) {
      error(node_i, "unexpected node in Switch, expecting Case or Default");
      return VoidConst;
    }
    if (case_i.op() == DEFAULT) {
      if (have_default) {
        error(node_i, "unexpected duplicate Default in Switch, expecting at most one");
        return VoidConst;
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
      compile_add(JumpIf{*func_, l_next, Binary{*func_, NEQ, expr, case_i.expr()}}, //
                  SimplifyDefault);
    }
    if (l_this_fallthrough) {
      add(l_this_fallthrough);
    }
    enter_case(l_break, l_fallthrough);
    compile_add(case_i.body(), SimplifyDefault);
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
  return VoidConst;
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

Var Compiler::to_var(Node node) noexcept {
  Expr e = node.is<Expr>();
  Var v = node.is<Var>();
  if (e && !v) {
    // copy Expr result to a Var
    v = Var{*func_, e.kind()};
    compile_add(Assign{*func_, ASSIGN, v, e}, SimplifyDefault);
  }
  return v;
}

Compiler &Compiler::to_vars(Node node, uint32_t start, uint32_t end, //
                            Vector<Expr> &vars) noexcept {
  if (!vars.resize(end > start ? end - start : 0)) {
    return out_of_memory(node);
  }
  for (size_t i = start; i < end; i++) {
    vars.set(i - start, to_var(node.child(i)));
  }
  return *this;
}

Compiler &Compiler::add(const Node &node) noexcept {
  if (node != VoidConst) {
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

} // namespace onejit
