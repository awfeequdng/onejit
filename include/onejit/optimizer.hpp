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
 * optimizer.hpp
 *
 *  Created on Feb 08, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPTIMIZER_HPP
#define ONEJIT_OPTIMIZER_HPP

#include <onejit/node/node.hpp>
#include <onestl/buffer.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////

class Optimizer {
public:
  enum Result : uint8_t;

  enum Flag : uint16_t {
    None = 0,
    ConstantFolding = 1 << 0,
    ExprSimplification = 1 << 1,
    DeadCodeElimination = 1 << 2,
    // treat floating point + and * as associative. requires ExprSimplification
    FastMath = 1 << 3,
    All = 0xffff,
  };

  Optimizer() noexcept;
  Optimizer(Optimizer &&other) noexcept = default;
  Optimizer &operator=(Optimizer &&other) noexcept = default;

  ~Optimizer() noexcept;

  Node optimize(Func &func, Node node, Flag flags = All) noexcept;

  // false if out-of-memory
  constexpr explicit operator bool() const noexcept {
    return bool(nodes_);
  }

private:
  Node optimize(Node node, Result &in_out) noexcept;
  Node optimize(Unary expr, Nodes children, Result result) noexcept;
  Node optimize(Binary expr, Nodes children, Result result) noexcept;
  static bool optimize_leaf(Type t, size_t n_children, Result &in_out) noexcept;

  Expr simplify_unary(Kind kind, Op1 op, Expr x) noexcept;
  Expr simplify_binary(Op2 op, Expr x, Expr y) noexcept;
  Binary partial_eval_binary(Op2 op, Expr x, Expr y) noexcept;

  Expr simplify_add(Expr x, Expr y) noexcept;
  Expr simplify_sub(Expr x, Expr y) noexcept;
  Expr simplify_mul(Expr x, Expr y) noexcept;
  Expr simplify_quo(Expr x, Expr y) noexcept;
  Expr simplify_rem(Expr x, Expr y) noexcept;
  Expr simplify_bitwise(Op2 op, Expr x, Expr y) noexcept;
  Expr simplify_shift(Op2 op, Expr x, Expr y) noexcept;
  Expr simplify_boolean(Op2 op, Expr x, Expr y) noexcept;
  Expr simplify_comparison(Op2 op, Expr x, Expr y) noexcept;

  static Node finish(Node node, Node new_node, Result result, Result &in_out) noexcept;

private:
  Func *func_;
  Buffer<Node> nodes_;
  Flag flags_;
};

////////////////////////////////////////////////////////////////////////////////

constexpr inline Optimizer::Flag operator~(Optimizer::Flag a) noexcept {
  return Optimizer::Flag(~unsigned(a));
}

constexpr inline Optimizer::Flag operator&(Optimizer::Flag a, Optimizer::Flag b) noexcept {
  return Optimizer::Flag(unsigned(a) & unsigned(b));
}

constexpr inline Optimizer::Flag operator|(Optimizer::Flag a, Optimizer::Flag b) noexcept {
  return Optimizer::Flag(unsigned(a) | unsigned(b));
}

constexpr inline Optimizer::Flag operator^(Optimizer::Flag a, Optimizer::Flag b) noexcept {
  return Optimizer::Flag(unsigned(a) ^ unsigned(b));
}

inline Optimizer::Flag operator&=(Optimizer::Flag &a, Optimizer::Flag b) noexcept {
  return a = a & b;
}

inline Optimizer::Flag operator|=(Optimizer::Flag &a, Optimizer::Flag b) noexcept {
  return a = a | b;
}

inline Optimizer::Flag operator^=(Optimizer::Flag &a, Optimizer::Flag b) noexcept {
  return a = a ^ b;
}

} // namespace onejit

#endif // ONEJIT_OPTIMIZER_HPP
