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
 * compiler.hpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_COMPILER_HPP
#define ONEJIT_COMPILER_HPP

#include <onejit/error.hpp>
#include <onejit/flowgraph.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/node.hpp>
#include <onejit/optimizer.hpp>
#include <onejit/reg/allocator.hpp>
#include <onestl/array.hpp>
#include <onestl/crange.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////

class Compiler {
public:
  enum Flags : uint8_t;

  Compiler() noexcept;
  Compiler(Compiler &&other) noexcept = default;
  Compiler &operator=(Compiler &&other) noexcept = default;

  ~Compiler() noexcept;

  // return false if out of memory
  explicit operator bool() const noexcept;

  constexpr Func *func() const noexcept {
    return func_;
  }

  // configure the checks that compiled code must perform at runtime.
  // default is CheckNone
  Compiler &configure(Check check) noexcept {
    optimizer_.configure(check);
    return *this;
  }

  // compile function to portable IR (intermediate representation)
  Compiler &compile(Func &func, Opt flags = OptAll) noexcept;

  // compile function to x86_64 assembly
  // defined in onejit/x64/compiler.cpp
  Compiler &compile_x64(Func &func, Opt flags = OptAll) noexcept;

  // return the configured checks that compiled code must perform at runtime.
  constexpr Check check() const noexcept {
    return optimizer_.check();
  }

  // return current compile errors
  constexpr CRange<Error> errors() const noexcept {
    return CRange<Error>{&error_};
  }

private:
  Node compile(Assign stmt, Flags flags) noexcept;
  Node compile(AssignCall stmt, Flags flags) noexcept;
  Expr compile(Binary expr, Flags flags) noexcept;
  Node compile(Block stmt, Flags flags) noexcept;
  Expr compile(Call expr, Flags flags) noexcept;
  Node compile(Cond stmt, Flags flags) noexcept;
  Expr compile(Expr expr, Flags flags) noexcept;
  Node compile(For stmt, Flags flags) noexcept;
  Node compile(If stmt, Flags flags) noexcept;
  Node compile(JumpIf stmt, Flags flags) noexcept;
  Expr compile(Mem expr, Flags flags) noexcept;
  Node compile(Node node, Flags flags) noexcept;
  Node compile(Return stmt, Flags flags) noexcept;
  Node compile(Stmt0 stmt, Flags flags) noexcept;
  Node compile(Stmt1 stmt, Flags flags) noexcept;
  Node compile(Stmt2 stmt, Flags flags) noexcept;
  Node compile(Stmt3 stmt, Flags flags) noexcept;
  Node compile(Stmt4 stmt, Flags flags) noexcept;
  Node compile(StmtN stmt, Flags flags) noexcept;
  Node compile(Switch stmt, Flags flags) noexcept;
  Expr compile(Unary expr, Flags flags) noexcept;
  Expr compile(Tuple expr, Flags flags) noexcept;

  Expr simplify_boolean(Op2 op, Expr x, Expr y) noexcept;
  Expr simplify_land(Expr x, Expr y) noexcept;
  Expr simplify_lor(Expr x, Expr y) noexcept;

private:
  // get current destination for "break"
  Label label_break() const noexcept;

  // get current destination for "continue"
  Label label_continue() const noexcept;

  // get current destination for "fallthrough"
  Label label_fallthrough() const noexcept;

  Compiler &enter_case(Label l_break, Label l_fallthrough) noexcept;
  Compiler &exit_case() noexcept;

  Compiler &enter_loop(Label l_break, Label l_continue) noexcept;
  Compiler &exit_loop() noexcept;

  // copy expression result to a new local variable.
  // if node is already a Var, does nothing and returns it
  Var to_var(Node node) noexcept;

  // copy node.child(start ... end-1) to new local variables,
  // and append such variables to vars.
  Compiler &to_vars(Node node, uint32_t start, uint32_t end, //
                    Array<Expr> &vars) noexcept;

  Compiler &add_prologue(Func &func) noexcept;

  // add an already compiled node to compiled list
  Compiler &add(const Node &node) noexcept;

  // compile a node, then add it to compiled list
  Compiler &compile_add(const Node &node, Flags flags) noexcept {
    return add(compile(node, flags));
  }

  // store compiled code into function.compiled()
  // invoked by compile(Func)
  Compiler &finish() noexcept;

  // add a compile error
  Compiler &error(Node where, Chars msg) noexcept;

  // add an out of memory error
  Compiler &out_of_memory(Node where) noexcept;

private:
  Optimizer optimizer_;
  reg::Allocator allocator_;
  Func *func_;

  Array<Label> break_;       // stack of 'break' destination labels
  Array<Label> continue_;    // stack of 'continue' destination labels
  Array<Label> fallthrough_; // stack of 'fallthrough' destination labels
  Array<Node> node_;
  FlowGraph flowgraph_;
  Array<Error> error_;
  bool good_; // !good_ means out of memory
};

} // namespace onejit

#endif // ONEJIT_COMPILER_HPP
