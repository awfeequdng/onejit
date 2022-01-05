/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * compiler.cpp
 *
 *  Created on Jan 04, 2022
 *      Author Massimiliano Ghilardi
 */

#include <onejit/compiler.hpp>
#include <onejit/func.hpp>
#include <onejit/ir.hpp>
#include <onejit/mir/compiler.hpp>
#include <onejit/mir/mem.hpp>
#include <onejit/mir/util.hpp>

namespace onejit {

Compiler &Compiler::compile_mir(Func &func, Opt flags) noexcept {
  compile(func, flags);
  if (*this && error_.empty()) {
    // pass our internal buffers node_ and error_ to mir::Compiler
    onejit::mir::Compiler{}.compile(func, allocator_, node_, flowgraph_, error_, //
                                    flags, abi_);
  }
  return *this;
}

// ===============================  mir::Compiler  =============================

namespace mir {

Compiler::operator bool() const noexcept {
  return good_ && func_ && *func_;
}

Compiler &Compiler::compile(Func &func, reg::Allocator & /*allocator*/, Array<Node> &node_vec,
                            FlowGraph & /*flowgraph*/, Array<Error> &error_vec, Opt flags,
                            Abi /*abi*/) noexcept {
  if (func.get_compiled(MIR)) {
    // already compiled for MIR
    return *this;
  }
  Node node = func.get_compiled(NOARCH);
  if (!node) {
    error_vec.append(Error{node, "function not compiled yet, cannot materialize it for MIR arch"});
    return *this;
  }

  node_vec.clear();
  func_ = &func;
  node_ = &node_vec;
  error_ = &error_vec;
  flags_ = flags;
  good_ = bool(func);

  return compile(node).finish();
}

Compiler &Compiler::finish() noexcept {
  if (*this && node_) {
    Node compiled;
    switch (node_->size()) {
    case 0:
      compiled = VoidConst;
      break;
    case 1:
      compiled = (*node_)[0];
      break;
    default:
      compiled = Block{*func_, *node_};
      break;
    }
    func_->set_compiled(MIR, compiled);
  }
  return *this;
}

// ===============================  compile(Node)  =============================

Compiler &Compiler::compile(Node node) noexcept {
  const Type t = node.type();
  switch (t) {
  case STMT_0:
    return error(node, "unexpected Stmt0");
  case STMT_1:
    return compile(node.is<Stmt1>());
  case STMT_2:
    return compile(node.is<Stmt2>());
  case STMT_3:
    return compile(node.is<Stmt3>());
  case STMT_4:
    return error(node, "unexpected Stmt4");
  case STMT_N:
    return compile(node.is<StmtN>());
  default:
    if (Expr expr = node.is<Expr>()) {
      return compile(expr);
    }
    return error(node, "unexpected Node");
  }
}

// ===============================  compile(Expr)  =============================

Compiler &Compiler::compile(Expr expr) noexcept {
  return add(simplify(expr));
}

Expr Compiler::simplify(Expr expr) noexcept {
  switch (expr.type()) {
  case VAR:
  case LABEL:
  case CONST:
    return expr;
  case MEM:
    return simplify(expr.is<onejit::Mem>());
  case UNARY:
    return simplify(expr.is<Unary>());
  case BINARY:
    return simplify(expr.is<Binary>());
  case TUPLE:
    return simplify(expr.is<Tuple>());
  default:
    error(expr, "unexpected Expr");
    return expr;
  }
}

Expr Compiler::simplify(onejit::Mem expr) noexcept {
  return expr; // TODO
}

Expr Compiler::simplify(Unary expr) noexcept {
  Expr x = expr.x();
  Expr simpl_x = to_var_mem_const(simplify(x));
  if (simpl_x != x) {
    return Unary{*func_, expr.kind(), expr.op(), simpl_x};
  }
  return expr;
}

Expr Compiler::simplify(Binary expr) noexcept {
  Expr x = expr.x(), y = expr.y();
  Expr simpl_x = x, simpl_y = y;
  simplify_binary(simpl_x, simpl_y);
  if (simpl_x != x || simpl_y != y) {
    return Binary{*func_, expr.op(), simpl_x, simpl_y};
  }
  return expr;
}

void Compiler::simplify_binary(Expr &x, Expr &y) noexcept {
  Expr sx = x;
  Expr sy = y;
  // apply x side effects first
  if (sx.type() == MEM) {
    sx = to_var_mem_const(sx);
    sy = to_var_const(simplify(sy));
  } else {
    sx = to_var(simplify(sx));
    sy = to_var_mem_const(simplify(sy));
  }
  x = sx;
  y = sy;
}

Expr Compiler::simplify(Tuple expr) noexcept {
  return expr; // TODO
}

// ===============================  compile(Stmt1)  ============================

Compiler &Compiler::compile(Stmt1 st) noexcept {
  OpStmt1 op = st.op();
  if (op == GOTO) {
    op = MIR_JMP;
  } else if (op == INC || op == DEC) {
    // TODO convert INC, DEC to MIR_ADD, MIR_SUB
  } else {
    return error(st, "unexpected Stmt1 operation");
  }
  Expr expr = simplify(st.arg());
  return add(Stmt1{*func_, expr, op});
}

// ===============================  compile(Stmt2)  ============================

Compiler &Compiler::compile(Stmt2 st) noexcept {
  if (Assign assign = st.is<Assign>()) {
    return compile(assign);
  }
  return error(st, "unexpected Stmt2");
}

Compiler &Compiler::compile(Assign st) noexcept {
  Expr src = st.src(), dst = st.dst();
  // simplify src first: its side effects, if any, must be applied before dst
  //
  // do not call to_var_mem_const(simplify(src)): it creates redundant code
  // as we support src being an expression
  src = simplify(src);
  dst = to_var_mem_const(simplify(dst));
  if (src.type() == MEM && dst.type() == MEM) {
    // both arguments are memory.
    // not supported by MIR assembly, force one to register
    src = to_var(src);
  }
  return add(simplify_assign(st, dst, src));
}

Node Compiler::simplify_assign(Assign st, Expr dst, Expr src) noexcept {
  OpStmt2 op = st.op();
  if (op >= ADD_ASSIGN && op <= SHR_ASSIGN) {
    // MIR assembly does not have two-operand x += y etc.
    // convert to x = x + y etc.
    src = to_var_mem_const(src);
    dst = to_var_mem_const(dst);
    Expr tmp = to_var(dst);
    return Stmt3{*func_, mir_arith(op, tmp.kind()), dst, tmp, src};
  }
  if (op == ASSIGN) {
    switch (src.type()) {
    case VAR:
    case MEM:
    case CONST:
    case LABEL:
      return Assign{*func_, mir_mov(dst.kind()), dst, src};
    case UNARY:
      return simplify_assign(st, dst, src.is<Unary>());
    case BINARY:
      return simplify_assign(st, dst, src.is<Binary>());
    case TUPLE:
      return simplify_assign(st, dst, src.is<Tuple>());
    default:
      error(st, "unexpected Assign right argument");
      break;
    }
  } else {
    error(st, "unexpected Assign statement");
  }
  return st;
}

Node Compiler::simplify_assign(Assign st, Expr dst, Unary src) noexcept {
  // TODO
  (void)dst;
  (void)src;
  return st;
}

Node Compiler::simplify_assign(Assign st, Expr dst, Binary src) noexcept {
  Op2 op = src.op();
  OpStmt3 op3;
  if (op >= SUB && op <= SHR) {
    op3 = mir_arith(op, dst.kind());
  } else if (op >= LSS && op <= GEQ) {
    op3 = mir_compare(op, dst.kind());
  } else {
    // TODO
    return st;
  }
  return simplify_ternary(op3, dst, src.x(), src.y());
}

Node Compiler::simplify_assign(Assign st, Expr dst, Tuple src) noexcept {
  const OpN op = src.op();
  if (op >= ADD && op <= XOR) {
    Expr x = src.arg(0);
    Expr y = src.arg(1);
    const OpStmt3 op3 = mir_arith(op, dst.kind());
    const uint32_t n = src.children();
    if (n < 2) {
      error(st, "unexpected Tuple inside Assign, expecting at least 2 operands");
      return st;
    } else if (n == 2) {
      return simplify_ternary(op3, dst, x, y);
    }
  } else if (op == CALL) {
    return simplify_call(st, Tuple{*func_, dst.kind(), COMMA, {dst}}, src);
  }
  return st; // TODO
}

Node Compiler::simplify_ternary(OpStmt3 op, Expr dst, Expr x, Expr y) noexcept {
  if (dst.type() == MEM) {
    x = to_var_const(x);
    y = to_var_const(y);
    dst = to_var_mem_const(dst);
  } else {
    x = to_var_mem_const(x);
    if (x.type() == MEM) {
      y = to_var_const(y);
    } else {
      y = to_var_mem_const(y);
    }
    dst = to_var(dst);
  }
  return Stmt3{*func_, op, dst, x, y};
}

Node Compiler::simplify_call(Assign st, Tuple dst, Tuple src) noexcept {
  FuncType ftype = src.child_is<FuncType>(0);
  Expr faddress = src.child_is<Expr>(1);
  if (ftype && faddress) {
    return Stmt4{*func_, MIR_CALL,
                 ftype,  faddress,
                 dst,    Tuple{*func_, src.kind(), COMMA, ChildRange{src, 2, src.children() - 2}}};
  } else {
    error(st, "invalid Call: first two arguments must be FuncType, Expr");
  }
  return st;
}

// ===============================  compile(Stmt3)  ============================

Compiler &Compiler::compile(Stmt3 st) noexcept {
  const OpStmt3 op = st.op();
  if (op >= ASM_JA && op <= ASM_JNE) {
    Label to = st.child_is<Label>(0);
    Expr x = simplify(st.child_is<Expr>(1));
    Expr y = simplify(st.child_is<Expr>(2));
    return add(Stmt3{*func_, mir_jump(op, x.kind()), to, x, y});
  } else {
    return error(st, "unexpected Stmt3");
  }
}

// ===============================  compile(StmtN)  ============================

Compiler &Compiler::compile(StmtN st) noexcept {
  switch (st.op()) {
  case ASSIGN_CALL:
    return compile(st.is<AssignCall>());
  case BLOCK:
    return compile(st.is<Block>());
  case RETURN:
    return compile(st.is<Return>());
  case SET_: // used in function prologue. not needed by MIR
    return *this;
  default:
    return error(st, "unexpected StmtN");
  }
}

Compiler &Compiler::compile(Block st) noexcept {
  for (size_t i = 0, n = st.children(); i < n; i++) {
    compile(st.child(i));
  }
  return *this;
}

Compiler &Compiler::compile(AssignCall st) noexcept {
  return add(st); // TODO
}

Compiler &Compiler::compile(Return st) noexcept {
  return add(st); // TODO
}

////////////////////////////////////////////////////////////////////////////////

Var Compiler::to_var(Expr expr) noexcept {
  Var v = expr.is<Var>();
  if (expr && !v) {
    // copy Expr result to a Var
    v = Var{*func_, expr.kind()};
    add(Stmt2{*func_, mir_mov(expr.kind()), v, expr});
  }
  return v;
}

Expr Compiler::to_var_const(Expr expr) noexcept {
  switch (expr.type()) {
  case VAR:
  case CONST:
  case LABEL:
    return expr;
  default:
    return to_var(expr);
  }
}

Expr Compiler::to_var_mem_const(Expr expr) noexcept {
  switch (expr.type()) {
  case VAR:
  case MEM:
  case CONST:
  case LABEL:
    return expr;
  default:
    return to_var(expr);
  }
}

Compiler &Compiler::add(Node node) noexcept {
  if (node != VoidConst) {
    good_ = good_ && node_ && node_->append(node);
  }
  return *this;
}

Compiler &Compiler::error(Node where, Chars msg) noexcept {
  good_ = good_ && error_ && error_->append(Error{where, msg});
  return *this;
}

Compiler &Compiler::out_of_memory(Node where) noexcept {
  // always set good_ to false
  good_ = good_ && error_ && error_->append(Error{where, "out of memory"}) && false;
  return *this;
}

} // namespace mir
} // namespace onejit
