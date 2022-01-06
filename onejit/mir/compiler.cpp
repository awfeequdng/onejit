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
  // simplify dst first: its side effects, if any, must be applied before src.
  // It also provides the destination where to store src result.
  Expr dst = simplify(st.dst(), toAny);
  OpStmt2 op = st.op();
  if (op >= ADD_ASSIGN && op <= SHR_ASSIGN) {

    // MIR assembly does not have two-operands arithmetic:
    // convert "dst OP= src" to "tmp = dst; dst = tmp OP src"
    Expr tmp = dst;
    Mask mask = toAny;
    if (dst.type() == MEM) {
      // MIR assembly support at most one memory operand
      tmp = Var{*func_, dst.kind()};
      mask = toVarOrConst;
    }
    Expr src = simplify(st.src(), mask);

    add(Stmt3{*func_, mir_arith(op, dst.kind()), dst, tmp, src});

  } else if (op == ASSIGN) {

    Mask mask = dst.type() == MEM ? toVarOrConst : toAny;
    Expr src = simplify(st.src(), mask, dst);
    if (src != dst) {
      add(Stmt2{*func_, mir_mov(dst.kind()), dst, src});
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
  (void)mask;
  (void)opt_dst;
  return expr; // TODO
}

Expr Compiler::simplify(Unary expr, Expr opt_dst) noexcept {
  // avoid multiple memory side effects, they may happen in wrong order
  Expr x = simplify(expr.x(), toConst | (opt_dst.type() == MEM ? toVar : toMem));
  Kind kfrom = x.kind();
  Kind kto = expr.kind();
  Op1 op = expr.op();
  Expr dst = opt_dst ? opt_dst : Var{*func_, kto};
  switch (op) {
  case XOR1: // unary xor, inverts all bits. MIR only has binary xor: dst = x ^ y
    add(Stmt3{*func_, kfrom.bits() == Bits64 ? MIR_XOR : MIR_XORS, //
              dst, x, MinusOne(*func_, kfrom)});
    break;
  case NOT1: // boolean not, swaps zero and non-zero. MIR only has comparison vs. zero
    add(Stmt3{*func_, kfrom.bits() == Bits64 ? MIR_EQ : MIR_EQS, //
              dst, x, Zero(kfrom)});
    break;
  case NEG1: // arithmetic negative, -x. MIR has MIR_NEG and friends
    add(Stmt2{*func_, mir_neg(kfrom), dst, x});
    break;
  case CAST: // convert int to float or vice-versa, widen or narrow integers
    add(Stmt2{*func_, mir_cast(kto, kfrom), dst, x});
    return dst;
  case BITCOPY:
    // TODO
    // [[fallthrough]]
  default:
    error(expr, "unexpected Unary expression");
  }
  return dst;
}

Expr Compiler::simplify(Binary expr, Expr opt_dst) noexcept {
  // avoid multiple memory side effects, they may happen in wrong order
  const Mask mask = (opt_dst.type() == MEM ? toConst : toConst | toMem);
  Expr x = simplify(expr.x(), mask);
  // avoid multiple memory side effects, they may happen in wrong order
  Expr y = simplify(expr.y(), mask & (x.type() == MEM ? toConst : toConst | toMem));

  Op2 op = expr.op();
  Kind kind = expr.kind();
  OpStmt3 mir_op;
  if (op >= SUB && op <= SHR) {
    mir_op = mir_arith(op, kind);
  } else if (op >= LSS && op <= GEQ) {
    mir_op = mir_compare(op, kind);
  } else {
    // TODO
    return expr;
  }
  Expr dst = opt_dst ? opt_dst : Var{*func_, expr.kind()};
  add(Stmt3{*func_, mir_op, dst, x, y});
  return dst;
}

Expr Compiler::simplify(Tuple expr, Expr opt_dst) noexcept {
  if (Call call = expr.is<Call>()) {
    return simplify(call, opt_dst);
  }
  return expr; // TODO
}

Expr Compiler::simplify(Call call, Expr opt_dst) noexcept {
  FuncType ftype = call.ftype();
  Expr faddress = call.address();
  if (!ftype || !faddress) {
    error(call, "invalid Call: first two arguments must be FuncType, Expr");
    return call;
  }
  Tuple results;
  Expr ret;
  if (opt_dst) {
    if (Tuple tuple = opt_dst.is<Tuple>()) {
      // opt_dst contains the places where to store multiple results
      if (tuple.children() != ftype.result_n()) {
        error(call, "invalid Call destination: number of results does not match");
      }
      ret = results = tuple;
    } else if (opt_dst.type() == VAR || opt_dst.type() == MEM) {
      // opt_dst contains the place where to store a single result. wrap it in a Tuple
      if (ftype.result_n() != 1) {
        error(call, "invalid Call destination: number of results does not match");
      }
      ret = opt_dst;
      results = Tuple{*func_, Void, MIR_RETS, {opt_dst}};
    } else {
      error(opt_dst, "invalid Call destination");
    }
  } else {
    // no result specified by caller: create temporary variables
    const size_t n = ftype.result_n();
    Array<Expr> array(n);
    if (array.size() != n) {
      out_of_memory(call);
      return Expr{};
    }
    for (size_t i = 0; i < n; i++) {
      array[i] = Var{*func_, ftype.result(i)};
    }
    ret = results = Tuple{*func_, Void, MIR_RETS, Nodes{array.data(), n}};
  }
  add(Stmt4{*func_, MIR_CALL, ftype, faddress, results,
            Tuple{*func_, Void, MIR_ARGS, ChildRange{call, 2, call.children() - 2}}});
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
    add(Stmt2{*func_, mir_mov(v.kind()), v, simplified_expr});
    return v;
  }
  if (Var v = simplified_expr.is<Var>()) {
    // Expr is already a Var, just return it
    return v;
  }
  // copy Expr result to a Var, then return the Var
  Var v{*func_, simplified_expr.kind()};
  add(Stmt2{*func_, mir_mov(v.kind()), v, simplified_expr});
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