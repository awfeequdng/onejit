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
#include <onejit/label.hpp>
#include <onejit/node.hpp>
#include <onestl/vector.hpp>

namespace onejit {

class Compiler {
public:
  Compiler(Func &func) noexcept;

  Compiler(Compiler &&other) noexcept = delete;
  Compiler &operator=(Compiler &&other) noexcept = delete;

  ~Compiler() noexcept;

  explicit operator bool() const noexcept;

  constexpr Func &func() const noexcept {
    return func_;
  }

  Node finish() noexcept;

  //////////////////////////////////////////////////////////////////////////////

  // get current destination for "break"
  Label label_break() const noexcept;

  // get current destination for "continue"
  Label label_continue() const noexcept;

  Compiler &enter_case(Label l_break) noexcept;
  Compiler &exit_case() noexcept;

  Compiler &enter_loop(Label l_break, Label l_continue) noexcept;
  Compiler &exit_loop() noexcept;

  // copy expression result to a new local variable.
  // if node is already a VarExpr, does nothing and returns it
  VarExpr to_var(const Node &node) noexcept;

  // copy node.child(start ... end-1) to new local variables,
  // and append such variables to vars.
  Compiler &to_vars(const Node &node, uint32_t start, uint32_t end, //
                    Vector<Expr> &vars) noexcept;

  // add an already compiled node to compiled list
  Compiler &add(const Node &node) noexcept;

  // compile a node, then add it to compiled list
  Compiler &compile_add(const Node &node, bool parent_is_expr) noexcept {
    return add(node.compile(*this, parent_is_expr));
  }

  // add a compile error
  Compiler &error(const Node &where, Chars msg) noexcept;

  // add an out-of-memory error
  Compiler &out_of_memory(const Node &where) noexcept;

private:
  Func &func_;

  Vector<Label> break_;    // stack of 'break' destination labels
  Vector<Label> continue_; // stack of 'continue' destination labels
  Vector<Node> node_;
  Vector<Error> error_;
  bool good_; // good_ = false means out-of-memory
};

} // namespace onejit

#endif // ONEJIT_COMPILER_HPP
