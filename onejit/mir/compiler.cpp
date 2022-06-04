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

namespace mir {

// ===============================  mir::Compiler  =============================

Compiler::operator bool() const noexcept {
  return good_ && func_ && *func_;
}

Compiler &Compiler::compile(Func &func, reg::Allocator & /*allocator*/, Array<Node> &node_vec,
                            FlowGraph & /*flowgraph*/, Array<Error> &error_vec, Opt flags,
                            Abi /*abi*/) noexcept {
  if (func.get_compiled(MIR)) {
    return *this; // already compiled for MIR
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
      compiled = Block{f(), *node_};
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
      // apply side effects, ignore result (unless it's a label)
      expr = simplify(expr);
      if (expr.type() == LABEL) {
        add(expr);
      }
      return *this;
    }
    return error(node, "unexpected Node");
  }
}

// ===============================  compile(Stmt1)  ============================

Compiler &Compiler::compile(Stmt1 st) noexcept {
  Expr expr = simplify(st.arg());
  OpStmt1 op = st.op();
  if (op == GOTO) {
    op = MIR_JMP;
    return add(Stmt1{f(), expr, op});
  } else if (op == INC || op == DEC) {
    // convert INC -> MIR_ADD, and DEC -> MIR_SUB
    Expr tmp = expr;
    if (tmp.type() == MEM) {
      // MIR instructions support at most one memory operand
      tmp = to_var(expr);
    }
    return add(Stmt3{f(), op == INC ? MIR_ADD : MIR_SUB, //
                     expr, tmp, One(f(), expr.kind())});
  } else {
    return error(st, "unexpected Stmt1 operation");
  }
}

// ===============================  compile(Stmt2)  ============================

Compiler &Compiler::compile(Stmt2 st) noexcept {
  if (Assign assign = st.is<Assign>()) {
    return compile(assign);
  }
  return error(st, "unexpected Stmt2");
}

Compiler &Compiler::compile(Assign st) noexcept {
  // simplify dst first: its side effects, if any, must be applied before src.
  // It also provides the destination where to store src result.
  Expr dst = simplify(st.dst(), toAny);
  OpStmt2 op = st.op();
  if (op >= ADD_ASSIGN && op <= SHR_ASSIGN) {

    // MIR assembly does not have two-operands arithmetic:
    // convert "dst OP= src" to "tmp = dst; dst = tmp OP src"
    Expr tmp = dst;
    Mask mask = toAny;
    if (dst.type() != VAR) {
      // MIR assembly support at most one memory operand
      tmp = Var{f(), dst.kind()};
      mask = toVarOrConst;
      add(Stmt2{f(), mir_mov(dst.kind()), tmp, dst});
    }
    Expr src = simplify(st.src(), mask);

    add(Stmt3{f(), mir_arith(op, dst.kind()), dst, tmp, src});

  } else if (op == ASSIGN) {

    Mask mask = dst.type() == MEM ? toVarOrConst : toAny;
    Expr src = simplify(st.src(), mask, dst);
    if (src != dst) {
      add(Stmt2{f(), mir_mov(dst.kind()), dst, src});
    }
  } else {
    error(st, "unexpected Assign statement");
  }
  return *this;
}

// ===============================  compile(Stmt3)  ============================

Compiler &Compiler::compile(Stmt3 st) noexcept {
  const OpStmt3 op = st.op();
  if (op >= ASM_JA && op <= ASM_JNE) {
    Label to = st.child_is<Label>(0);
    Expr x = simplify(st.child_is<Expr>(1));
    Expr y = simplify(st.child_is<Expr>(2));
    return add(Stmt3{f(), mir_jump(op, x.kind()), to, x, y});
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
  while (const uint32_t n = st.children()) {
    Call call = st.child_is<Call>(n - 1);
    if (!call) {
      break;
    }
    Array<Node> array;
    if (!array.resize(n - 1)) {
      return out_of_memory(st);
    }
    for (uint32_t i = 0; i < n; i++) {
      array.set(i, simplify(st.child_is<Expr>(i), toVarOrMem));
    }
    Tuple dst{f(), Void, MIR_RETS, array};
    Expr results = simplify(call, dst);
    if (results != dst) {
      return error(
          st, "internal error: miscompiled Call, results are not stored in expected variables");
    }
    return *this;
  }
  return error(st, "missing Call inside AssignCall");
}

Compiler &Compiler::compile(Return st) noexcept {
  const uint32_t n = st.children();
  Array<Expr> array;
  if (!array.resize(n)) {
    return out_of_memory(st);
  }
  for (uint32_t i = 0; i < n; i++) {
    array.set(i, simplify(st.child_is<Expr>(i), toVar));
  }
  return add(Return{f(), MIR_RET, array});
}

// ===============================  simplify(Expr)  ============================

Expr Compiler::simplify(Expr expr, Mask mask, Expr opt_dst) noexcept {
  switch (expr.type()) {
  case VAR:
  case LABEL:
    return expr;
  case CONST:
    return simplify(expr.is<Const>(), mask, opt_dst);
  case MEM:
    return simplify(expr.is<onejit::Mem>(), mask, opt_dst);
  case UNARY:
    return simplify(expr.is<Unary>(), opt_dst);
  case BINARY:
    return simplify(expr.is<Binary>(), opt_dst);
  case TUPLE:
    return simplify(expr.is<Tuple>(), opt_dst);
  default:
    error(expr, "unexpected Expr");
    return expr;
  }
}

Expr Compiler::simplify(Const expr, Mask mask, Expr opt_dst) noexcept {
  if (mask & toConst) {
    return expr;
  }
  return to_var(expr, opt_dst);
}

Expr Compiler::simplify(onejit::Mem expr, Mask mask, Expr opt_dst) noexcept {
  const uint32_t n = expr.children();
  Array<Node> children;
  if (!children.resize(n)) {
    out_of_memory(expr);
    return Expr{};
  }
  for (uint32_t i = 0; i < n; i++) {
    children.set(i, simplify(expr.arg(i), toVarOrConst));
  }
  Kind kind = expr.kind();
  Mem mem{*this, kind, children}; // may fail if too many or too complex args
  if (!mem) {
    Expr arg = simplify(Tuple{f(), kind, ADD, children});
    mem = Mem{*this, expr.kind(), Nodes{&arg, 1}};
    if (!mem) {
      error(expr, "internal error: failed to compile onejit::mir::Mem");
      return Expr{};
    }
  }
  if (mask & toMem) {
    return mem;
  }
  Expr dst = (opt_dst && opt_dst.type() == VAR) ? opt_dst : Var{f(), kind};
  add(Stmt2{f(), mir_mov(kind), dst, mem});
  return dst;
}

Expr Compiler::simplify(Unary expr, Expr opt_dst) noexcept {
  // avoid multiple side effects, they may happen in wrong order
  Expr x = simplify(expr.x(), opt_dst.type() == MEM ? toVarOrConst : toAny);
  Kind kfrom = x.kind();
  Kind kto = expr.kind();
  Op1 op = expr.op();
  Expr dst = opt_dst ? opt_dst : Var{f(), kto};
  switch (op) {
  case XOR1: // unary xor, inverts all bits. MIR only has binary xor: dst = x ^ y
    add(Stmt3{f(), kfrom.ebits() == eBits64 ? MIR_XOR : MIR_XORS, //
              dst, x, MinusOne(f(), kfrom)});
    break;
  case NOT1: // boolean not, swaps zero and non-zero. MIR only has comparison vs. zero
    add(Stmt3{f(), kfrom.ebits() == eBits64 ? MIR_EQ : MIR_EQS, //
              dst, x, Zero(kfrom)});
    break;
  case NEG1: // arithmetic negative, -x. MIR has MIR_NEG and friends
    add(Stmt2{f(), mir_neg(kfrom), dst, x});
    break;
  case CAST: // convert int to float or vice-versa, widen or narrow integers
    add(Stmt2{f(), mir_cast(kto, kfrom), dst, x});
    return dst;
  case BITCOPY: // TODO: must store bytes in ALLOCA:ted memory and load from it
    error(expr, "unimplemented Unary operand: BITCOPY");
    return dst;
  default:
    error(expr, "unexpected Unary expression operand");
    return Expr{};
  }
  return dst;
}

Expr Compiler::simplify(Binary expr, Expr opt_dst) noexcept {
  // avoid multiple side effects, they may happen in wrong order
  Mask mask = (opt_dst.type() == MEM ? toVarOrConst : toAny);
  Expr x = simplify(expr.x(), mask);
  // avoid multiple side effects, they may happen in wrong order
  mask = mask & (x.type() == MEM ? toVarOrConst : toAny);
  Expr y = simplify(expr.y(), mask);

  Op2 op = expr.op();
  Kind kind = expr.kind();
  OpStmt3 mir_op;
  if (op >= SUB && op <= SHR) {
    mir_op = mir_arith(op, kind);
  } else if (op >= LSS && op <= GEQ) {
    mir_op = mir_compare(op, kind);
  } else {
    error(expr, "unexpected Binary expression operand");
    return Expr{};
  }
  Expr dst = opt_dst ? opt_dst : Var{f(), expr.kind()};
  add(Stmt3{f(), mir_op, dst, x, y});
  return dst;
}

Expr Compiler::simplify(Tuple expr, Expr opt_dst) noexcept {
  OpN op = expr.op();
  uint32_t n = expr.children();
  Kind kind = expr.kind();
  switch (op) {
  case CALL:
    return simplify(expr.is<Call>(), opt_dst);
  case MAX:
  case MIN:
    if (n <= 1) {
      // should not happen
      error(expr, "unexpected Tuple number of arguments, should be > 1");
      return Expr{};
    } else {
      OpStmt2 op2mov = mir_mov(kind);
      OpStmt3 op3jmp = mir_jump(op == MAX ? GEQ : LEQ, kind);
      Mask mask = opt_dst.type() == MEM ? toVarOrConst : toAny;
      Expr x = simplify(expr.arg(0), mask);
      Expr dst = Var{f(), kind};
      add(Stmt2{f(), op2mov, dst, x});
      // simplify i-th argument and compute max/min between dst and y,
      // then store result into dst (or opt_dst)
      for (uint32_t i = 1; i < n; i++) {
        mask = mask & (x.type() == MEM ? toVarOrConst : toAny);
        Expr y = simplify(expr.arg(i), mask);
        if (i + 1 == n && opt_dst) {
          add(Stmt2{f(), op2mov, opt_dst, dst});
          dst = opt_dst;
        }
        Label l = f().new_label();
        add(Stmt3{f(), op3jmp, l, dst, y});
        add(Stmt2{f(), op2mov, dst, y});
        add(l);
      }
      return dst;
    }
  default:
    if (op < ADD || op > XOR) {
      error(expr, "unexpected Tuple operand");
      return Expr{};
    } else if (n <= 1) {
      // should not happen
      error(expr, "unexpected Tuple number of arguments, should be > 1");
      return Expr{};
    }
    OpStmt3 op3 = mir_arith(op, kind);
    Mask mask = opt_dst.type() == MEM ? toVarOrConst : toAny;
    Expr x = simplify(expr.arg(0), mask);
    // compile (x OP y) and store result into new x
    for (uint32_t i = 1; i < n; i++) {
      mask = mask & (x.type() == MEM ? toVarOrConst : toAny);
      Expr y = simplify(expr.arg(i), mask);
      Expr dst = (i + 1 == n && opt_dst) ? opt_dst : Var{f(), kind};
      add(Stmt3{f(), op3, dst, x, y});
      x = dst;
    }
    return x;
  }
}

Expr Compiler::simplify(Call call, Expr opt_dst) noexcept {
  Tuple results;
  Expr ret = prepare_call_results(call, opt_dst, results);
  if (!ret) {
    // an error happened, and was already reported
    return ret;
  }
  const uint32_t argn = call.children() - 2;
  Array<Node> children;
  if (!children.resize(argn + 3)) {
    out_of_memory(call);
    return Expr{};
  }
  children.set(0, call.ftype());
  children.set(1, call.address());
  children.set(2, results);
  for (uint32_t i = 0; i < argn; i++) {
    children.set(i + 3, simplify(call.arg(i), toVar));
  }
  add(StmtN{f(), MIR_CALL, children});
  return ret;
}

Expr Compiler::prepare_call_results(Call call, Expr opt_dst, Tuple &results) noexcept {
  FuncType ftype = call.ftype();
  if (!ftype || !call.address()) {
    error(call, "invalid Call: first two arguments must be FuncType, Expr");
    return Expr{};
  }

  Expr ret;
  if (opt_dst) {
    if (Tuple tuple = opt_dst.is<Tuple>()) {
      // opt_dst contains the places where to store multiple results
      if (tuple.children() != ftype.result_n()) {
        error(call, "invalid Call destination: number of results does not match");
        return Expr{};
      }
      if (tuple.op() != MIR_RETS) {
        error(opt_dst, "invalid Call destination OpN, expecting MIR_RETS");
        return Expr{};
      }
      ret = results = tuple;
    } else if (opt_dst.type() == VAR || opt_dst.type() == MEM) {
      // opt_dst contains the place where to store a single result. wrap it in a Tuple
      if (ftype.result_n() != 1) {
        error(call, "invalid Call destination: number of results does not match");
        return Expr{};
      }
      ret = opt_dst;
      results = Tuple{f(), Void, MIR_RETS, {opt_dst}};
    } else {
      error(opt_dst, "invalid Call destination type");
      return Expr{};
    }
  } else {
    // no result specified by caller: create temporary variables
    const uint16_t n = ftype.result_n();
    Array<Node> array;
    if (!array.resize(n)) {
      out_of_memory(call);
      return Expr{};
    }
    for (uint16_t i = 0; i < n; i++) {
      array.set(i, Var{f(), ftype.result(i)});
    }
    ret = results = Tuple{f(), Void, MIR_RETS, array};
  }
  return ret;
}

////////////////////////////////////////////////////////////////////////////////

Var Compiler::to_var(Expr simplified_expr, Expr opt_dst) noexcept {
  if (!simplified_expr) {
    error(simplified_expr, "invalid Expr");
    return Var{};
  }
  if (Var v = opt_dst.is<Var>()) {
    // opt_dst is a Var, copy Expr into it
    add(Stmt2{f(), mir_mov(v.kind()), v, simplified_expr});
    return v;
  }
  if (Var v = simplified_expr.is<Var>()) {
    // Expr is already a Var, just return it
    return v;
  }
  // copy Expr result to a Var, then return the Var
  Var v{f(), simplified_expr.kind()};
  add(Stmt2{f(), mir_mov(v.kind()), v, simplified_expr});
  return v;
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
