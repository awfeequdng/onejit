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
 * optimizer_binary.cpp
 *
 *  Created on Feb 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/eval.hpp>
#include <onejit/func.hpp>
#include <onejit/mem.hpp>
#include <onejit/node/binary.hpp>
#include <onejit/node/const.hpp>
#include <onejit/node/tuple.hpp>
#include <onejit/optimizer.hpp>

namespace onejit {

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
  if (op == SUB && y.type() == CONST && y.kind().is_signed()) {
    // optimize (x - const) to (x + (-const))
    // because + is easier to optimize further
    Value v = -y.is<Const>().val();
    if (v.is_valid()) {
      if (Const c = Const{*func_, v}) {
        return flatten_tuple(Tuple{*func_, ADD, x, c});
      }
    }
  }
  return changed && x && y ? Binary{*func_, op, x, y} : Binary{};
}

Expr Optimizer::simplify_sub(Expr x, Expr y) noexcept {
  if (Const yc = y.is<Const>()) {
    if (!yc.val()) {
      // optimize x-0 to x
      return x;
    }
  }
  if (Var xv = x.is<Var>()) {
    if (Var yv = y.is<Var>()) {
      if (xv.local() == yv.local()) {
        // optimize x-x to 0
        return Zero(x.kind());
      }
    }
  }
  return Expr{};
}

Expr Optimizer::simplify_quo(Expr x, Expr y) noexcept {
  (void)x;
  (void)y;
  return Expr{};
}

Expr Optimizer::simplify_rem(Expr x, Expr y) noexcept {
  (void)x;
  (void)y;
  return Expr{};
}

Expr Optimizer::simplify_shift(Op2 op, Expr x, Expr y) noexcept {
  (void)op;
  (void)x;
  (void)y;
  return Expr{};
}

Expr Optimizer::simplify_boolean(Op2 op, Expr x, Expr y) noexcept {
  (void)op;
  (void)x;
  (void)y;
  return Expr{};
}

Expr Optimizer::simplify_comparison(Op2 op, Expr x, Expr y) noexcept {
  (void)op;
  (void)x;
  (void)y;
  return Expr{};
}

} // namespace onejit
