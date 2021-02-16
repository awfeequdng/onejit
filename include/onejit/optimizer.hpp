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
  enum OptFlags : uint16_t {
    OptNone = 0,
    OptFoldConstant = 1 << 0,
    OptSimplifyExpr = 1 << 1,
    OptRemoveDeadCode = 1 << 2,
    // treat floating point + and * as associative. requires OptSimplifyExpr
    OptFastMath = 1 << 3,
    OptAll = 0xffff,
  };

  Optimizer() noexcept;
  Optimizer(Optimizer &&other) noexcept = default;
  Optimizer &operator=(Optimizer &&other) noexcept = default;

  ~Optimizer() noexcept;

  Node optimize(Func &func, Node node, OptFlags flags = OptAll) noexcept;

  // false if out-of-memory
  constexpr explicit operator bool() const noexcept {
    return bool(nodes_);
  }

private:
  Node optimize(Node node) noexcept;
  Expr try_optimize(Unary expr, Nodes children) noexcept;
  Expr try_optimize(Binary expr, Nodes children) noexcept;
  Expr optimize(Tuple expr, bool optimize_children) noexcept;

  bool optimize_children(Node node, Span<Node> &children_out) noexcept;
  // recursively append (optionally) optimized children of node to this->nodes_
  bool flatten_children_tobuf(Node node, bool optimize_children) noexcept;

  static bool same_children(Node node, Nodes children) noexcept;

  Expr simplify_unary(Kind kind, Op1 op, Expr x) noexcept;
  Expr simplify_binary(Op2 op, Expr x, Expr y) noexcept;
  Expr partial_eval_binary(Op2 op, Expr x, Expr y) noexcept;
  Expr partial_eval_tuple(Tuple expr, Span<Node> children) noexcept;

  Expr simplify_sub(Expr x, Expr y) noexcept;
  Expr simplify_quo(Expr x, Expr y) noexcept;
  Expr simplify_rem(Expr x, Expr y) noexcept;
  Expr simplify_shift(Op2 op, Expr x, Expr y) noexcept;
  Expr simplify_boolean(Op2 op, Expr x, Expr y) noexcept;
  Expr simplify_comparison(Op2 op, Expr x, Expr y) noexcept;

private:
  Func *func_;
  Buffer<Node> nodes_;
  OptFlags flags_;
};

////////////////////////////////////////////////////////////////////////////////

constexpr inline Optimizer::OptFlags operator~(Optimizer::OptFlags a) noexcept {
  return Optimizer::OptFlags(~unsigned(a));
}

constexpr inline Optimizer::OptFlags operator&(Optimizer::OptFlags a,
                                               Optimizer::OptFlags b) noexcept {
  return Optimizer::OptFlags(unsigned(a) & unsigned(b));
}

constexpr inline Optimizer::OptFlags operator|(Optimizer::OptFlags a,
                                               Optimizer::OptFlags b) noexcept {
  return Optimizer::OptFlags(unsigned(a) | unsigned(b));
}

constexpr inline Optimizer::OptFlags operator^(Optimizer::OptFlags a,
                                               Optimizer::OptFlags b) noexcept {
  return Optimizer::OptFlags(unsigned(a) ^ unsigned(b));
}

inline Optimizer::OptFlags operator&=(Optimizer::OptFlags &a, Optimizer::OptFlags b) noexcept {
  return a = a & b;
}

inline Optimizer::OptFlags operator|=(Optimizer::OptFlags &a, Optimizer::OptFlags b) noexcept {
  return a = a | b;
}

inline Optimizer::OptFlags operator^=(Optimizer::OptFlags &a, Optimizer::OptFlags b) noexcept {
  return a = a ^ b;
}

} // namespace onejit

#endif // ONEJIT_OPTIMIZER_HPP
