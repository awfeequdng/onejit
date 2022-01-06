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
 *  Created on Jan 04, 2022
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_MIR_COMPILER_HPP
#define ONEJIT_MIR_COMPILER_HPP

#include <onejit/error.hpp>
#include <onejit/reg/fwd.hpp>
#include <onestl/array.hpp>

namespace onejit {
namespace mir {

////////////////////////////////////////////////////////////////////////////////
// compiles code from portable intermediate representation
// (produced by onejit::Compiler::compile()) to libmir MIR representation,
// then uses libmir JIT to generate native machine code
//
// usually not invoked directly - the public API is onejit::Compiler::mir()
class Compiler {
  friend class ::onejit::Compiler;
  friend class Address;
  friend class Mem;

  enum mKind : uint8_t { mInt32, mUint32, mInt64, mUint64, mFloat32, mFloat64, mFloat128 };
  enum Mask : uint8_t {
    toVar = 0, // toVar is always assumed
    toConst = 1,
    toMem = 2,
    toVarOrConst = toVar | toConst,
    toVarOrMem = toVar | toMem,
    toAny = toVar | toConst | toMem
  };

  friend constexpr Mask operator&(Mask left, Mask right) noexcept {
    return Mask(uint8_t(left) & uint8_t(right));
  }

  friend constexpr Mask operator|(Mask left, Mask right) noexcept {
    return Mask(uint8_t(left) | uint8_t(right));
  }

public:
  constexpr Compiler() noexcept //
      : func_{}, node_{}, error_{}, flags_{}, good_{true} {
  }

  Compiler(Compiler &&other) noexcept = default;
  Compiler &operator=(Compiler &&other) noexcept = default;

  ~Compiler() noexcept = default;

  // return false if out of memory
  explicit operator bool() const noexcept;

private:
  // private, use onejit::Compiler::compile_mir() instead
  Compiler &compile(Func &func, reg::Allocator &allocator, Array<Node> &node,      //
                    FlowGraph &flowgraph, Array<Error> &error, Opt flags, Abi abi) //
      noexcept;

  Compiler &compile(Assign stmt) noexcept;
  Compiler &compile(AssignCall stmt) noexcept;
  Compiler &compile(Block stmt) noexcept;
  Compiler &compile(Node node) noexcept;
  Compiler &compile(Return stmt) noexcept;
  Compiler &compile(Stmt1 stmt) noexcept;
  Compiler &compile(Stmt2 stmt) noexcept;
  Compiler &compile(Stmt3 stmt) noexcept;
  Compiler &compile(StmtN stmt) noexcept;

  Expr simplify(Expr expr, Mask mask = toAny, Expr opt_dst = Expr{}) noexcept;
  Expr simplify(Const expr, Mask mask = toAny, Expr opt_dst = Expr{}) noexcept;
  Expr simplify(onejit::Mem expr, Mask mask = toAny, Expr opt_dst = Expr{}) noexcept;
  Expr simplify(Unary expr, Expr opt_dst = Expr{}) noexcept;
  Expr simplify(Binary expr, Expr opt_dst = Expr{}) noexcept;
  Expr simplify(Tuple expr, Expr opt_dst = Expr{}) noexcept;
  Expr simplify(Call expr, Expr opt_dst = Expr{}) noexcept;

  constexpr Func *func() const noexcept {
    return func_;
  }

  Var to_var(Expr simplified_expr, Expr opt_dst = Expr{}) noexcept;

  // add an already compiled node to compiled list
  Compiler &add(Node node) noexcept;

  // store compiled code into function.set_compiled(X64)
  // invoked by compile(Func)
  Compiler &finish() noexcept;

  // add a compile error
  Compiler &error(Node where, Chars msg) noexcept;

  // add an out of memory error
  Compiler &out_of_memory(Node where) noexcept;

  Func *func_;
  Array<Node> *node_;
  Array<Error> *error_;
  Opt flags_;
  bool good_; // !good_ means out of memory
};

} // namespace mir
} // namespace onejit

#endif // ONEJIT_X64_COMPILER_HPP
