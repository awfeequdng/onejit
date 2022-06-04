/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * assembler.hpp
 *
 *  Created on Jan 23, 2022
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_MIR_ASSEMBLER_HPP
#define ONEJIT_MIR_ASSEMBLER_HPP

#include <onejit/error.hpp>
#include <onejit/fwd.hpp>
#include <onejit/id_hash.hpp>
#include <onestl/crange.hpp>
#include <onestl/string.hpp>

#include <initializer_list>
#include <unordered_map>
#include <vector>

struct MIR_context;
struct MIR_insn;
struct MIR_item;
struct MIR_module;
typedef struct MIR_context *MIR_context_t;
typedef struct MIR_insn *MIR_insn_t;
typedef struct MIR_item *MIR_item_t;
typedef struct MIR_insn *MIR_label_t;
typedef struct MIR_module *MIR_module_t;
typedef uint32_t MIR_reg_t;

namespace onejit {
namespace mir {

struct Op; // wraps MIR_op_t
typedef View<Op> Ops;

class Assembler {

public:
  Assembler();

  ~Assembler() noexcept;

  // assemble a function. must be already compiled to MIR instructions.
  // temporary: also link it and return address of jit-compiled function
  void *assemble(const Func &func);

  /// @return current assembler errors
  constexpr CRange<Error> errors() const noexcept {
    return CRange<Error>{&error_};
  }

  /// @return false if some error occurred
  constexpr explicit operator bool() const noexcept {
    return good_;
  }

  // add an assembler error
  Assembler &error(Node where, Chars msg) noexcept;

  // add an out of memory error
  Assembler &out_of_memory(Node where) noexcept;

private:
  void create_mir_func(const Func &func);
  void declare_mir_labels();
  void declare_mir_vars();

  // assemble a MIR instruction.
  void add_node(Node node);
  void add_stmt0(Stmt0 stmt);
  void add_stmt1(Stmt1 stmt);
  void add_stmt2(Stmt2 stmt);
  void add_stmt3(Stmt3 stmt);
  void add_stmt4(Stmt4 stmt);
  void add_stmtn(StmtN stmt);
  void add_block(Block stmt);
  void add_return(StmtN stmt);
  void add_expr(Expr expr);
  void add_label(Label label);

  void add_mir_insn(Node node, int mir_opcode, Ops operands);

  void add_mir_insn(Node node, int mir_opcode, std::initializer_list<Op> operands) {
    add_mir_insn(node, mir_opcode, Ops{operands.begin(), operands.size()});
  }

  // assemble a MIR expression
  Op op(Expr expr);
  Op op(Binary binary);
  Op op(Const const_);
  Op op(Label label);
  Op op(Tuple tuple);
  Op op(Unary unary);
  Op op(Var var);

private:
  MIR_context_t mctx_;
  MIR_module_t mmod_;
  MIR_item_t mfunc_;
  Array<MIR_label_t> mlabels_;
  std::unordered_map<Id, MIR_reg_t> mvars_;
  const Func *func_;
  std::vector<String> param_names_;
  Array<Error> error_;
  bool good_;

}; // class Assembler

} // namespace mir
} // namespace onejit

#endif // ONEJIT_MIR_ASSEMBLER_HPP
