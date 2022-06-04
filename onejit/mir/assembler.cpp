/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * assembler.cpp
 *
 *  Created on Jan 23, 2022
 *      Author Massimiliano Ghilardi
 */

#include <onejit/func.hpp>
#include <onejit/ir/binary.hpp>
#include <onejit/ir/expr.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/stmt0.hpp>
#include <onejit/ir/stmt1.hpp>
#include <onejit/ir/stmt2.hpp>
#include <onejit/ir/stmt3.hpp>
#include <onejit/ir/stmt4.hpp>
#include <onejit/ir/stmtn.hpp>
#include <onejit/ir/tuple.hpp>
#include <onejit/ir/unary.hpp>
#include <onejit/mir/assembler.hpp>
#include <onejit_config.h>
#include <onestl/fmt.hpp>
#include <onestl/writer_cstdio.hpp> // for debugging only
#include <onestl/writer_string.hpp>

#include <cstdio> // for debugging only

#if defined(HAVE_MIR_H)
#include <mir.h>
#elif defined(HAVE_MIR_MIR_H)
#include <mir/mir.h>
#endif

#if defined(HAVE_MIR_GEN_H)
#include <mir-gen.h>
#elif defined(HAVE_MIR_MIR_GEN_H)
#include <mir/mir-gen.h>
#endif

namespace onejit {
namespace mir {

#ifdef HAVE_MIR

struct Op {
  Op() noexcept : impl{} {
  }

  constexpr explicit operator bool() const noexcept {
    return impl.mode != MIR_OP_UNDEF;
  }

  MIR_op_t impl;
};

static MIR_type_t to_mir_memkind(Kind kind) noexcept {
  static const MIR_type_t mtypes[] = {
      MIR_T_UNDEF, MIR_T_UNDEF, MIR_T_U8,             // eBad, eVoid, eBool
      MIR_T_I8,    MIR_T_I16,   MIR_T_I32, MIR_T_I64, // eInt*
      MIR_T_U8,    MIR_T_U16,   MIR_T_U32, MIR_T_U64, // eUint*
      MIR_T_UNDEF, MIR_T_F,     MIR_T_D,   MIR_T_LD,  // eFloat*
      MIR_T_U64,                                      // ePtr
  };
  return mtypes[kind.nosimd().val()];
}

static MIR_type_t to_mir_kind(Kind kind) noexcept {
  static const MIR_type_t mtypes[] = {
      MIR_T_UNDEF, MIR_T_UNDEF, MIR_T_I64,            // eBad, eVoid, eBool
      MIR_T_I64,   MIR_T_I64,   MIR_T_I64, MIR_T_I64, // eInt*
      MIR_T_I64,   MIR_T_I64,   MIR_T_I64, MIR_T_I64, // eUint*
      MIR_T_UNDEF, MIR_T_F,     MIR_T_D,   MIR_T_LD,  // eFloat*
      MIR_T_I64,                                      // ePtr
  };
  return mtypes[kind.nosimd().val()];
}

static MIR_insn_code_t to_mir_code2(OpStmt2 op) noexcept {
#define ONEJIT_X(NAME, name) ::MIR_##NAME,
  static const MIR_insn_code_t mcodes[] = {
      ONEJIT_OPSTMT2_MIR(ONEJIT_X) //
  };
#undef ONEJIT_X
  if (op >= MIR_MOV && op - MIR_MOV < ONEJIT_N_OF(mcodes)) {
    return mcodes[op - MIR_MOV];
  }
  return ::MIR_INVALID_INSN;
}

static MIR_insn_code_t to_mir_code3(OpStmt3 op) noexcept {
#define ONEJIT_X(NAME, name) ::MIR_##NAME,
  static const MIR_insn_code_t mcodes[] = {
      ONEJIT_OPSTMT3_MIR(ONEJIT_X) //
  };
#undef ONEJIT_X
  if (op >= MIR_ADD && op - MIR_ADD < ONEJIT_N_OF(mcodes)) {
    return mcodes[op - MIR_ADD];
  }
  return ::MIR_INVALID_INSN;
}

// ----------------------------------- Assembler -------------------------------

Assembler::Assembler()
    : mctx_{}, mmod_{}, mfunc_{}, mlabels_{}, func_{}, param_names_{}, error_{}, good_{true} {
  mctx_ = MIR_init();
  if (mctx_) {
    MIR_gen_init(mctx_, 1 /*threads*/);
    MIR_gen_set_optimize_level(mctx_, 0 /*thread*/, 3 /*-O3*/);
    mmod_ = MIR_new_module(mctx_, "m");
    if (!mmod_) {
      error(Node{}, "MIR_new_module() failed");
    }
  } else {
    error(Node{}, "MIR_init() failed");
  }
}

Assembler::~Assembler() noexcept {
  try {
    if (mfunc_) {
      mfunc_ = nullptr;
      MIR_finish_func(mctx_);
    }
    if (mmod_) {
      mmod_ = nullptr;
      MIR_finish_module(mctx_);
    }
    if (mctx_) {
      MIR_finish(mctx_);
      mctx_ = nullptr;
    }
  } catch (...) {
  }
}

void *Assembler::assemble(const Func &func) {
  Node body = func.get_compiled(MIR);
  if (!body) {
    error(Node{}, "function is not compiled for MIR arch, cannot assemble it");
    return nullptr;
  }
  create_mir_func(func);
  declare_mir_labels();
  declare_mir_vars();
  add_node(body);
  MIR_finish_func(mctx_);

  // temporary: does not support mutually-recursive functions
  MIR_finish_module(mctx_);
  MIR_load_module(mctx_, mmod_);
  MIR_link(mctx_, MIR_set_gen_interface, nullptr);
  void *jit_compiled_func = MIR_gen(mctx_, 0, mfunc_);

  mfunc_ = nullptr;
  mmod_ = nullptr;

  return jit_compiled_func;
}

void Assembler::create_mir_func(const Func &func) {
  if (!mmod_) {
    mmod_ = MIR_new_module(mctx_, "m");
    if (!mmod_) {
      error(Node{}, "MIR_new_module() failed");
    }
  }
  func_ = &func;

  Vars params = func.params();
  Vars results = func.results();
  size_t param_n = params.size();
  size_t result_n = results.size();
  Array<MIR_var_t> mparams(param_n);
  Array<MIR_type_t> mresults(result_n);

  param_names_.resize(param_n);
  mparams.resize(param_n);

  for (size_t i = 0; i < param_n; i++) {
    Fmt{&param_names_[i]} << "arg" << (i + 1);

    mparams.set(i, MIR_var_t{to_mir_kind(params[i].kind()), //
                             param_names_[i].c_str(), 0});
  }
  for (size_t i = 0; i < result_n; i++) {
    mresults.set(i, to_mir_kind(results[i].kind()));
  }

  String name(func.name().chars()); // must be '\0' terminated
  mfunc_ = MIR_new_func_arr(mctx_, name.c_str(), mresults.size(), mresults.data(), //
                            mparams.size(), mparams.data());
}

void Assembler::declare_mir_labels() {
  uint16_t max_idx = 0;
  for (Label label : func_->labels()) {
    const uint16_t idx = label.index();
    if (max_idx < idx) {
      max_idx = idx;
    }
  }
  mlabels_.resize(max_idx + 1);
  for (Label label : func_->labels()) {
    mlabels_.set(label.index(), MIR_new_label(mctx_));
  }
}

void Assembler::declare_mir_vars() {
  {
    Vars params = func_->params();
    const size_t n = params.size();
    for (size_t i = 0; i < n; i++) {
      Var param = params[i];
      mvars_[param.id()] = MIR_reg(mctx_, param_names_[i].c_str(), mfunc_->u.func);
    }
  }
  String name;
  Fmt fmt{&name};
  for (Var var : func_->vars()) {
    const Id id = var.id();
    if (mvars_.find(id) != mvars_.end()) {
      // skip params created above
      continue;
    }
    fmt << id;
    mvars_[id] = MIR_new_func_reg(mctx_, mfunc_->u.func, to_mir_kind(var.kind()), name.c_str());
    name.clear();
  }
}

// ------------------------ statements ------------------------------

void Assembler::add_node(Node node) {
  const Type t = node.type();
  switch (t) {
  case STMT_0:
    return add_stmt0(node.is<Stmt0>());
  case STMT_1:
    return add_stmt1(node.is<Stmt1>());
  case STMT_2:
    return add_stmt2(node.is<Stmt2>());
  case STMT_3:
    return add_stmt3(node.is<Stmt3>());
  case STMT_4:
    return add_stmt4(node.is<Stmt4>());
  case STMT_N:
    return add_stmtn(node.is<StmtN>());
  default:
    if (Expr expr = node.is<Expr>()) {
      return add_expr(expr);
    }
    error(node, "unsupported MIR statement");
  }
}

void Assembler::add_stmt0(Stmt0 stmt) {
  error(stmt, "unimplemented MIR statement Stmt0");
}
void Assembler::add_stmt1(Stmt1 stmt) {
  switch (stmt.op()) {
  case MIR_JMP:
    return add_mir_insn(stmt, ::MIR_JMP, {op(stmt.arg().is<Label>())});
  default:
    break;
  }
  error(stmt, "unimplemented MIR statement Stmt1");
}
void Assembler::add_stmt2(Stmt2 stmt) {
  const MIR_insn_code_t code = to_mir_code2(stmt.op());
  if (code != ::MIR_INVALID_INSN) {
    return add_mir_insn(stmt, code,
                        {op(stmt.child(0).is<Expr>()), //
                         op(stmt.child(1).is<Expr>())});
  }
  error(stmt, "unimplemented MIR statement Stmt2");
}

void Assembler::add_stmt3(Stmt3 stmt) {
  const MIR_insn_code_t code = to_mir_code3(stmt.op());
  if (code != ::MIR_INVALID_INSN) {
    return add_mir_insn(stmt, code,
                        {op(stmt.child(0).is<Expr>()), //
                         op(stmt.child(1).is<Expr>()), //
                         op(stmt.child(2).is<Expr>())});
  }
  error(stmt, "unimplemented MIR statement Stmt3");
}

void Assembler::add_stmt4(Stmt4 stmt) {
  error(stmt, "unimplemented MIR statement Stmt4");
}

void Assembler::add_stmtn(StmtN stmt) {
  switch (stmt.op()) {
  case ASSIGN_CALL:
    break;
  case BLOCK:
    return add_block(stmt.is<Block>());
  case MIR_CALL:
    return add_call(stmt);
  case MIR_RET:
    return add_return(stmt.is<Return>());
  case MIR_SWITCH:
  case COND:
  case SWITCH:
  case RETURN:
  default:
    break;
  }
  error(stmt, "unimplemented MIR statement StmtN");
}

void Assembler::add_block(Block stmt) {
  for (size_t i = 0, n = stmt.children(); i < n; i++) {
    add_node(stmt.child(i));
  }
}

void Assembler::add_call(StmtN stmt) {
  error(stmt, "unimplemented MIR statement Call");
}

void Assembler::add_return(Return stmt) {
  const size_t n = stmt.children();
  std::vector<Op> operands(n);
  for (size_t i = 0; i < n; i++) {
    operands[i] = op(stmt.child(i).is<Expr>());
  }
  add_mir_insn(stmt, ::MIR_RET, Ops{operands.data(), n});
}

void Assembler::add_mir_insn(Node node, int mir_opcode, Ops operands) {
  static_assert(sizeof(Op) == sizeof(MIR_op_t),
                "sizeof(onejit::mir::Op) must match sizeof(MIR_op_t)");
  for (const Op &operand : operands) {
    if (!operand) {
      error(node, "unimplemented MIR statement: operand was converted to null MIR_op_t");
      return;
    }
  }
  MIR_insn_t insn =
      MIR_new_insn_arr(mctx_, MIR_insn_code_t(mir_opcode), operands.size(),
                       reinterpret_cast<MIR_op_t *>(const_cast<Op *>(operands.data())));
  MIR_append_insn(mctx_, mfunc_, insn);
}

// ------------------------ expressions -----------------------------

void Assembler::add_expr(Expr expr) {
  switch (expr.type()) {
  case UNARY:
    break;
  case BINARY:
    break;
  case MEM:
    break;
  case TUPLE:
    break;
  case VAR:
    break;
  case LABEL:
    return add_label(expr.is<Label>());
  case CONST:
    break;
  default:
    break;
  }
  error(expr, "unimplemented MIR statement Expr");
}

void Assembler::add_label(Label label) {
  // supports only LOCAL labels,
  // i.e. does NOT support labels representing other functions
  MIR_label_t mlabel = mlabels_[label.index()];
  if (mlabel) {
    MIR_append_insn(mctx_, mfunc_, mlabel);
  } else {
    error(label, "missing MIR label");
  }
}

Op Assembler::op(Expr expr) {
  switch (expr.type()) {
  case VAR:
    return op(expr.is<Var>());
  case UNARY:
    return op(expr.is<Unary>());
  case BINARY:
    return op(expr.is<Binary>());
  case MEM:
  case TUPLE:
    return op(expr.is<Tuple>());
  case LABEL:
    return op(expr.is<Label>());
  case CONST:
    return op(expr.is<Const>());
  default:
    break;
  }
  error(expr, "unimplemented MIR expression Expr");
  return {};
}

Op Assembler::op(Const const_) {
  Op ret;
  const Value val = const_.val();
  const MIR_type_t mkind = to_mir_kind(val.kind());
  switch (mkind) {
  case MIR_T_I64:
    ret.impl = MIR_new_int_op(mctx_, val.int64());
    break;
  case MIR_T_F:
    ret.impl = MIR_new_float_op(mctx_, val.float32());
    break;
  case MIR_T_D:
    ret.impl = MIR_new_double_op(mctx_, val.float64());
    break;
  default:
    error(const_, "unimplemented MIR expression Const");
    break;
  }
  return ret;
}

Op Assembler::op(Label label) {
  // supports only LOCAL labels,
  // i.e. does NOT support labels representing other functions
  MIR_label_t mlabel = mlabels_[label.index()];
  Op ret;
  if (mlabel) {
    ret.impl = MIR_new_label_op(mctx_, mlabel);
  } else {
    error(label, "missing MIR label");
  }
  return ret;
}

Op Assembler::op(Var var) {
  Op ret;
  auto iter = mvars_.find(var.id());
  if (iter != mvars_.end()) {
    ret.impl = MIR_new_reg_op(mctx_, iter->second);
  } else {
    error(var, "missing MIR local variable");
  }
  return ret;
}

Op Assembler::op(Unary expr) {
  error(expr, "unimplemented MIR expression Unary");
  return {};
}
Op Assembler::op(Binary binary) {
  error(binary, "unimplemented MIR expression Binary");
  return {};
}

Op Assembler::op(Tuple tuple) {
  error(tuple, "unimplemented MIR expression Tuple");
  return {};
}

#else // !HAVE_MIR

Assembler::Assembler() : good_{true} {
  error(Node{}, "MIR support not compiled");
}

Assembler::~Assembler() {
}

void *Assembler::assemble(const Func &func) {
  (void)func;
  (void)func_;
  (void)mctx_;
  (void)mmod_;
  (void)mfunc_;
  (void)mlabels_;
  (void)param_names_;
  return nullptr;
}

#endif // HAVE_MIR

Assembler &Assembler::error(Node where, Chars msg) noexcept {
  good_ = good_ && error_.append(Error{where, msg});
  return *this;
}

Assembler &Assembler::out_of_memory(Node where) noexcept {
  // always set good_ to false
  good_ = good_ && error_.append(Error{where, "out of memory"}) && false;
  return *this;
}

} // namespace mir
} // namespace onejit
