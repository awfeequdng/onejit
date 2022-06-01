/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * optimizer_binary.cpp
 *
 *  Created on Feb 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/eval.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/binary.hpp>
#include <onejit/ir/comma.hpp>
#include <onejit/ir/const.hpp>
#include <onejit/ir/tuple.hpp>
#include <onejit/mem.hpp>
#include <onejit/optimizer.hpp>
#include <onestl/range.hpp>

namespace onejit {

Node Optimizer::try_optimize(Binary expr, const Range<Node> &children) noexcept {
  Expr x, y;
  if (expr && children.size() == 2 //
      && (x = children[0].is<Expr>()) && (y = children[1].is<Expr>())) {

    Op2 op = expr.op();
    if ((flags_ & OptFoldConstant) && x.type() == CONST && y.type() == CONST) {
      Value v0 = x.is<Const>().val();
      Value v1 = y.is<Const>().val();
      if (v0.is_valid() && v1.is_valid()) {
        Value ve = eval_binary_op(op, v0, v1);
        if (ve.is_valid()) {
          return Const{*func_, ve};
        }
      }
    }
    if (flags_ & OptSimplifyExpr) {
      return simplify_binary(op, x, y);
    }
  }
  return Node{};
}

Expr Optimizer::simplify_binary(Op2 op, Expr x, Expr y) noexcept {
  Expr expr = partial_eval_binary(op, x, y);
  if (expr) {
    if (Binary be = expr.is<Binary>()) {
      op = be.op();
      x = be.x();
      y = be.y();
    } else {
      return expr;
    }
  }
  Expr expr2;
  switch (op) {
    break;
  case SUB:
    expr2 = simplify_sub(x, y);
    break;
  case QUO:
    expr2 = simplify_quo(x, y);
    break;
  case REM:
    expr2 = simplify_rem(x, y);
    break;
  case SHL:
  case SHR:
    expr2 = simplify_shift(op, x, y);
    break;
  case LAND:
  case LOR:
    expr2 = simplify_boolean(op, x, y);
    break;
  case LSS:
  case LEQ:
  case NEQ:
  case EQL:
  case GTR:
  case GEQ:
    expr2 = simplify_comparison(op, x, y);
    break;
  default:
    break;
  }
  return expr2 ? expr2 : expr;
}

Expr Optimizer::partial_eval_binary(Op2 op, Expr x, Expr y) noexcept {
  bool changed = false;
  if (x.type() > y.type() && is_comparison(op)) {
    // put constants to the right
    op = swap_comparison(op);
    mem::swap(x, y);
    changed = true;
  }
  if (Const c = y.is<Const>()) {
    Value identity = Value::identity(x.kind(), op);
    if (identity.is_valid() && identity == c.val()) {
      // optimize (op x identity) to x
      return x;
    }
#if 0
    if (op == SUB) {
      // optimize (x - const) to (x + (-const))
      // because + is easier to optimize further on x86
      Value v = -c.val();
      if (v.is_valid() && (c = Const{*func_, v})) {
        // optimize() may resize nodes_ and change its data()
        // => it invalidates spans and views on it!
        // only Range<Node> on nodes_ remains valid.
        return optimize(Tuple{*func_, ADD, x, c}, false);
      }
    }
#endif // 0
  }
  return changed && x && y ? Binary{*func_, op, x, y} : Binary{};
}

Expr Optimizer::simplify_sub(Expr x, Expr y) noexcept {
  if (x.deep_equal(y, allow_mask_pure())) {
    // optimize (- x x) to 0
    return Zero(x.kind());
  }
  return Expr{};
}

Expr Optimizer::simplify_quo(Expr x, Expr y) noexcept {
  if (x.deep_equal(y, allow_mask_pure())) {
    // optimize (/ x x) to 1
    return One(*func_, x.kind());
  }
  return Expr{};
}

Expr Optimizer::simplify_rem(Expr x, Expr y) noexcept {
  if (x.deep_equal(y, allow_mask_pure())) {
    // optimize (% x x) to 0
    return Zero(x.kind());
  }
  return Expr{};
}

Expr Optimizer::simplify_shift(Op2 op, Expr x, Expr y) noexcept {
  // TODO
  (void)op;
  (void)x;
  (void)y;
  return Expr{};
}

Expr Optimizer::simplify_boolean(Op2 op, Expr x, Expr y) noexcept {
  Const xc = x.is<Const>();
  Const yc = y.is<Const>();
  switch (op) {
  case LAND:
    if (xc) {
      // optimize true && y to y
      // optimize false && y to false
      return xc.val() ? y : x;
    } else if (yc) {
      if (yc.val()) {
        // optimize x && true to x
        return x;
      } else {
        // optimize x && false to x, false
        Expr args[] = {x, y};
        return simplify_comma(Span<Expr>{args, 2});
      }
    }
    break;
  case LOR:
    if (xc) {
      // optimize true || y to true
      // optimize false || y to y
      return xc.val() ? x : y;
    } else if (yc) {
      if (yc.val()) {
        // optimize x || true to x, true
        Expr args[] = {x, y};
        return simplify_comma(Span<Expr>{args, 2});
      } else {
        // optimize x || false to x
        return x;
      }
    }
    break;
  default:
    break;
  }
  return Expr{};
}

Expr Optimizer::simplify_comparison(Op2 op, Expr x, Expr y) noexcept {
  // partial_eval_binary() above put constants to the right
  if (Const cy = y.is<Const>()) {
    if (y.kind().is_unsigned() && !cy.val()) {
      // comparing unsigned expression with zero
      switch (op) {
      case LSS: {
        // simplify expr_u < 0 to (expr_u, false)
        Expr args[] = {x, FalseExpr};
        return simplify_comma(Span<Expr>{args, 2});
      }
      case LEQ:
        // simplify expr_u <= 0 to expr_u == 0
        return Binary{*func_, EQL, x, y};
      case GEQ: {
        // simplify expr_u >= 0 to (expr_u, true)
        Expr args[] = {x, TrueExpr};
        return simplify_comma(Span<Expr>{args, 2});
      }
      case GTR:
        // simplify expr_u > 0 to expr_u != 0
        return Binary{*func_, NEQ, x, y};
      default:
        break;
      }
    }
  } else if (x.deep_equal(y, allow_mask_pure())) {
    // comparing an expression with itself
    switch (op) {
    case LSS:
    case NEQ:
    case GTR:
      return FalseExpr;
    case LEQ:
    case EQL:
    case GEQ:
      return TrueExpr;
    default:
      break;
    }
  }
  return Expr{};
}

Expr Optimizer::simplify_comma(Span<Expr> argspan) noexcept {
  const size_t n = argspan.size();
  if (n == 0) {
    return VoidExpr;
  }
  Expr *args = argspan.data();
  size_t src, dst;
  for (src = dst = 0; src + 1 < n; src++) {
    if (!args[src].deep_pure(allow_mask_pure())) {
      args[dst++] = args[src];
    }
  }
  Expr last = args[src];
  if (dst == 0) {
    return last;
  }
  args[dst++] = last;
  return Comma{*func_, Exprs{args, dst}};
}

} // namespace onejit
