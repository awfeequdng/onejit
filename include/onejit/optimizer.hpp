/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * optimizer.hpp
 *
 *  Created on Feb 08, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPTIMIZER_HPP
#define ONEJIT_OPTIMIZER_HPP

#include <onejit/check.hpp>
#include <onejit/ir/node.hpp>
#include <onestl/buffer.hpp>
#include <onestl/crange.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////

// optimizations
enum Opt : uint16_t {
  OptNone = 0,
  OptFoldConstant = 1 << 0,
  OptSimplifyExpr = 1 << 1,
  OptRemoveDeadCode = 1 << 2,
  // treat floating point + and * as associative. requires OptSimplifyExpr
  OptFastMath = 1 << 3,
  OptAll = 0xffff,
};

////////////////////////////////////////////////////////////////////////////////

class Optimizer {
public:
  Optimizer() noexcept;
  Optimizer(Optimizer &&other) noexcept = default;
  Optimizer &operator=(Optimizer &&other) noexcept = default;

  ~Optimizer() noexcept;

  // configure the checks that compiled code must perform at runtime.
  // default is CheckNone
  Optimizer &configure(Check check) noexcept {
    check_ = check;
    return *this;
  }

  Node optimize(Func &func, Node node, Opt flags = OptAll) noexcept;

  // false if out of memory
  constexpr explicit operator bool() const noexcept {
    return bool(nodes_);
  }

  /// @return the configured checks that compiled code must perform at runtime.
  constexpr Check check() const noexcept {
    return check_;
  }

private:
  Node optimize(Node node) noexcept;
  Expr optimize(Tuple expr, bool optimize_children) noexcept;

  Node try_optimize(Assign st, const Range<Node> &children) noexcept;
  Node try_optimize(Binary expr, const Range<Node> &children) noexcept;
  Node try_optimize(Unary expr, const Range<Node> &children) noexcept;
  // called by try_optimize(Assign) above
  Node try_optimize(OpStmt2 assign_op, Expr dst, Expr src) noexcept;

  // recursively call optimize() on node's children and return them
  Range<Node> optimize_children(Node node) noexcept;
  // recursively append (optionally) optimized children of node to this->nodes_
  bool flatten_children_tobuf(Node node, bool optimize_children) noexcept;

  static bool same_children(Node node, Nodes children) noexcept;

  Node make_assign(OpStmt2 assign_op, Expr dst, Expr src) noexcept;
  Node simplify_assign(OpStmt2 assign_op, Expr dst, Value src) noexcept;
  Expr simplify_binary(Op2 op, Expr x, Expr y) noexcept;
  Expr simplify_unary(Kind kind, Op1 op, Expr x) noexcept;
  Expr partial_eval_binary(Op2 op, Expr x, Expr y) noexcept;
  Expr partial_eval_tuple(Tuple expr, Range<Node> &children) noexcept;

  Expr simplify_sub(Expr x, Expr y) noexcept;
  Expr simplify_quo(Expr x, Expr y) noexcept;
  Expr simplify_rem(Expr x, Expr y) noexcept;
  Expr simplify_shift(Op2 op, Expr x, Expr y) noexcept;
  Expr simplify_boolean(Op2 op, Expr x, Expr y) noexcept;
  Expr simplify_comparison(Op2 op, Expr x, Expr y) noexcept;

  Expr simplify_comma(Span<Expr> args) noexcept;

  // convert configured Check:s to an Allow mask
  // that ignores expressions with side effects
  constexpr Allow allow_mask_pure() const noexcept {
    return Allow(~check()) & ~AllowCall;
  }

private:
  Func *func_;
  Buffer<Node> nodes_;
  Check check_;
  Opt flags_;
};

////////////////////////////////////////////////////////////////////////////////

constexpr inline Opt operator~(Opt a) noexcept {
  return Opt(~unsigned(a));
}

constexpr inline Opt operator&(Opt a, Opt b) noexcept {
  return Opt(unsigned(a) & unsigned(b));
}

constexpr inline Opt operator|(Opt a, Opt b) noexcept {
  return Opt(unsigned(a) | unsigned(b));
}

constexpr inline Opt operator^(Opt a, Opt b) noexcept {
  return Opt(unsigned(a) ^ unsigned(b));
}

inline Opt operator&=(Opt &a, Opt b) noexcept {
  return a = a & b;
}

inline Opt operator|=(Opt &a, Opt b) noexcept {
  return a = a | b;
}

inline Opt operator^=(Opt &a, Opt b) noexcept {
  return a = a ^ b;
}

} // namespace onejit

#endif // ONEJIT_OPTIMIZER_HPP
