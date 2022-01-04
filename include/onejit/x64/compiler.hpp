/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * compiler.hpp
 *
 *  Created on Feb 19, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_COMPILER_HPP
#define ONEJIT_X64_COMPILER_HPP

#include <onejit/error.hpp>
#include <onejit/reg/fwd.hpp>
#include <onestl/array.hpp>

namespace onejit {
namespace x64 {

////////////////////////////////////////////////////////////////////////////////
// compiles code from portable intermediate representation
// (produced by onejit::Compiler::compile()) to x86_64 assembler
//
// usually not invoked directly - the public API is onejit::Compiler::x64()
class Compiler {
  friend class ::onejit::Compiler;
  friend class Address;
  friend class Mem;

public:
  constexpr Compiler() noexcept //
      : func_{}, allocator_{}, node_{}, flowgraph_{}, error_{}, flags_{}, good_{true} {
  }

  Compiler(Compiler &&other) noexcept = default;
  Compiler &operator=(Compiler &&other) noexcept = default;

  ~Compiler() noexcept = default;

  // return false if out of memory
  explicit operator bool() const noexcept;

private:
  // private, use onejit::Compiler::compile_x64() instead
  Compiler &compile(Func &func, reg::Allocator &allocator, Array<Node> &node,      //
                    FlowGraph &flowgraph, Array<Error> &error, Opt flags, Abi abi) //
      noexcept;

  Compiler &compile(Assign stmt) noexcept;
  Compiler &compile(AssignCall stmt) noexcept;
  Compiler &compile(Block stmt) noexcept;
  Compiler &compile(Expr expr) noexcept;
  Compiler &compile(Node node) noexcept;
  Compiler &compile(Return stmt) noexcept;
  Compiler &compile(Stmt1 stmt) noexcept;
  Compiler &compile(Stmt2 stmt) noexcept;
  Compiler &compile(Stmt3 stmt) noexcept;
  Compiler &compile(StmtN stmt) noexcept;

  Expr simplify(Binary expr) noexcept;
  Expr simplify(Expr expr) noexcept;
  Expr simplify(onejit::Mem expr) noexcept;
  Expr simplify(Tuple expr) noexcept;
  Expr simplify(Unary expr) noexcept;

  Node simplify_assign(Assign st, Expr dst, Expr src) noexcept;
  Node simplify_assign(Assign st, Expr dst, Unary src) noexcept;
  Node simplify_assign(Assign st, Expr dst, Binary src) noexcept;
  Node simplify_assign(Assign st, Expr dst, Tuple src) noexcept;

  void simplify_binary(Expr &x, Expr &y) noexcept;

  constexpr Func *func() const noexcept {
    return func_;
  }

  // if expr is a Var, does nothing and returns it.
  // otherwise copies expression result to a new local variable and returns it.
  Var to_var(Expr expr) noexcept;

  // if expr is a Var or Const, does nothing and returns it
  // otherwise copies expression result to a new local variable and returns it.
  Expr to_var_const(Expr expr) noexcept;

  // if expr is a Var, Mem or Const, does nothing and returns it
  // otherwise copies expression result to a new local variable and returns it.
  Expr to_var_mem_const(Expr expr) noexcept;

  // add an already compiled node to compiled list
  Compiler &add(Node node) noexcept;

  // perform register allocation
  Compiler &allocate_regs(Abi abi) noexcept;

  Compiler &fill_interference_graph() noexcept;

  static void update_live_regs(BitSet &live, Node node) noexcept;

  // set ABI register hints for function params and results
  Compiler &set_reg_hints(Abi abi) noexcept;

  // store compiled code into function.set_compiled(X64)
  // invoked by compile(Func)
  Compiler &finish() noexcept;

  // add a compile error
  Compiler &error(Node where, Chars msg) noexcept;

  // add an out of memory error
  Compiler &out_of_memory(Node where) noexcept;

  Func *func_;
  reg::Allocator *allocator_;
  Array<Node> *node_;
  FlowGraph *flowgraph_;
  Array<Error> *error_;
  Opt flags_;
  bool good_; // !good_ means out of memory
};

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_COMPILER_HPP
