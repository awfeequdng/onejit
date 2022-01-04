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
  Expr simpl_x = to_var_mem_const(simplify(x));
  Expr simpl_y = to_var_mem_const(simplify(y));
  if (simpl_x.type() == MEM && simpl_y.type() == MEM) {
    // both arguments are memory.
    // not supported by x86_64 assembly, force one to register
    simpl_y = to_var(simpl_y);
  }
  x = simpl_x;
  y = simpl_y;
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
    // TODO convert INC, DEC to ADD, SUB
  } else {
    return error(st, "unexpected Stmt1 operation");
  }
  Expr expr = simplify(st.arg());
  return add(Stmt1{*func_, expr, op});
}

// ===============================  compile(Stmt2)  ============================

Compiler &Compiler::compile(Stmt2 st) noexcept {
  return add(st); // TODO
}

Compiler &Compiler::compile(Assign st) noexcept {
  return add(st); // TODO
}

Node Compiler::simplify_assign(Assign st, Expr dst, Expr src) noexcept {
  // TODO
  (void)dst;
  (void)src;
  return st;
}

Node Compiler::simplify_assign(Assign st, Expr dst, Unary src) noexcept {
  // TODO
  (void)dst;
  (void)src;
  return st;
}

Node Compiler::simplify_assign(Assign st, Expr dst, Tuple src) noexcept {
  // TODO
  (void)dst;
  (void)src;
  return st;
}

Node Compiler::simplify_assign(Assign st, Expr dst, Binary src) noexcept {
  // TODO
  (void)dst;
  (void)src;
  return st;
}

// ===============================  compile(Stmt3)  ============================

Compiler &Compiler::compile(Stmt3 st) noexcept {

  static const OpStmt3 int64_jump[] = {MIR_UBGT, MIR_UBGE, MIR_UBLT, MIR_UBLE, MIR_BEQ,
                                       MIR_BGT,  MIR_BGE,  MIR_BLT,  MIR_BLE,  MIR_BNE};
#if 0
  static const OpStmt3 int32_jump[] = {MIR_UBGTS, MIR_UBGES, MIR_UBLTS, MIR_UBLES, MIR_BEQS,
                                       MIR_BGTS,  MIR_BGES,  MIR_BLTS,  MIR_BLES,  MIR_BNES};
  static const OpStmt3 float32_jump[] = {MIR_FBGT, MIR_FBGE, MIR_FBLT, MIR_FBLE, MIR_FBEQ,
                                         MIR_FBGT, MIR_FBGE, MIR_FBLT, MIR_FBLE, MIR_FBNE};
  static const OpStmt3 float64_jump[] = {MIR_DBGT, MIR_DBGE, MIR_DBLT, MIR_DBLE, MIR_DBEQ,
                                         MIR_DBGT, MIR_DBGE, MIR_DBLT, MIR_DBLE, MIR_DBNE};
#endif
  const OpStmt3 op = st.op();
  if (op >= ASM_JA && op <= ASM_JNE) {
    // TODO merge comparison and jump
    // TODO convert int32, float32, float64 comparisons
    Label to = st.child_is<Label>(0);
    Expr x = simplify(st.child_is<Expr>(1));
    Expr y = simplify(st.child_is<Expr>(2));
    return add(Stmt3{*func_, to, x, y, int64_jump[op - ASM_JA]});
  } else {
    // TODO
    return add(st);
  }
} // namespace mir

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
    // compile(Assign{...}) would cause infinite recursion
    add(Assign{*func_, ASSIGN, v, expr});
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
