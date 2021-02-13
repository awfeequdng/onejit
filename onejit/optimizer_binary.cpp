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
#include <onejit/optimizer.hpp>

namespace onejit {

Expr Optimizer::simplify_binary(Op2 op, Expr x, Expr y) noexcept {
  Binary expr = partial_eval_binary(op, x, y);
  if (expr) {
    op = expr.op();
    x = expr.x();
    y = expr.y();
  }
  Expr expr2;
  switch (op) {
  case ADD:
    expr2 = simplify_add(x, y);
    break;
  case SUB:
    expr2 = simplify_sub(x, y);
    break;
  case MUL:
    expr2 = simplify_mul(x, y);
    break;
  case QUO:
    expr2 = simplify_quo(x, y);
    break;
  case REM:
    expr2 = simplify_rem(x, y);
    break;
  case AND:
  case OR:
  case XOR:
  case AND_NOT:
    expr2 = simplify_bitwise(op, x, y);
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

Binary Optimizer::partial_eval_binary(Op2 op, Expr x, Expr y) noexcept {
  bool changed = false;
  if (x.type() > y.type()) {
    if (is_commutative(op)) {
      // put constants to the right
      mem::swap(x, y);
      changed = true;
    } else if (is_comparison(op)) {
      // put constants to the right
      op = swap_comparison(op);
      mem::swap(x, y);
      changed = true;
    }
  }
  if (op == SUB && y.type() == CONST && y.kind().is_signed()) {
    // optimize (x - const) to (x + (-const))
    // because + is easier to optimize further
    Value v = -y.is<Const>().val();
    if (v.is_valid()) {
      op = ADD;
      y = Const{*func_, v};
      changed = true;
    }
  }
  // floating point operations are never exactly associative.
  // treat them as associative only if (flags & FastMath)
  if (is_associative(op) && x.type() == BINARY && Op2(x.op()) == op //
      && ((flags_ & FastMath) || !x.kind().is_float())) {

    if (Expr z = x.child_is<Expr>(0)) {
      if (Const c1 = x.child_is<Const>(1)) {
        if (Const c2 = y.is<Const>()) {
          /**
           * optimize   op    =>     op
           *           /  \         /  \
           *         op   c2       z  eval_binary(op, c1, c2)
           *        /  \
           *       z   c1
           */
          Value v = eval_binary(op, c1.val(), c2.val());
          if (v.is_valid()) {
            x = z;
            y = Const{*func_, v};
            changed = true;
          }
        } else if (is_commutative(op) && y.type() == BINARY && Op2(y.op()) == op) {
          if (Expr w = y.child_is<Expr>(0)) {
            if ((c2 = y.child_is<Const>(1))) {
              /**
               * optimize    op       =>      op
               *           /    \            /  \
               *        op       op        op   eval_binary(op, c1, c2)
               *       /  \     /  \      /  \
               *      z   c1   w   c2    z    w
               */
              Value v = eval_binary(op, c1.val(), c2.val());
              if (v.is_valid()) {
                if (z.type() > w.type()) {
                  mem::swap(z, w);
                }
                x = Binary{*func_, op, z, w};
                y = Const{*func_, v};
                changed = true;
              }
            }
          }
        }
      }
    }
  }
  return changed && x && y ? Binary{*func_, op, x, y} : Binary{};
}

Expr Optimizer::simplify_add(Expr x, Expr y) noexcept {
  if (Const yc = y.is<Const>()) {
    if (!yc.val()) {
      // optimize x+0 to x
      return x;
    }
  }
  if (Var xv = x.is<Var>()) {
    if (Var yv = y.is<Var>()) {
      if (xv.local() == yv.local()) {
        // optimize x+x to x<<1
        return Binary{*func_, SHL, x, One(*func_, x.kind())};
      }
    }
  }
  return Expr{};
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

Expr Optimizer::simplify_mul(Expr x, Expr y) noexcept {
  if (Const yc = y.is<Const>()) {
    Value yval = yc.val();
    if (!yval) {
      if (Var xv = x.is<Var>()) {
        // optimize x*0 to 0
        return Zero(x.kind());
      }
    } else if (yval == Value{1}.cast(y.kind())) {
      // optimize x*1 to 1
      return x;
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

Expr Optimizer::simplify_bitwise(Op2 op, Expr x, Expr y) noexcept {
  (void)op;
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
