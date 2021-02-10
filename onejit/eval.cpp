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
 * eval.cpp
 *
 *  Created on Feb 07, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/node/const.hpp>
#include <onejit/eval.hpp>
#include <onejit/node/binary.hpp>
#include <onejit/node/unary.hpp>

namespace onejit {

bool is_const(Expr expr) noexcept {
  for (;;) {
    switch (expr.type()) {
    case UNARY:
      expr = expr.is<Unary>().x();
      continue;
    case BINARY: {
      Binary be = expr.is<Binary>();
      if (!is_const(be.x())) {
        break;
      }
      expr = be.y();
      continue;
    }
    case CONST:
      return true;
    default:
      break;
    }
    break;
  }
  return false;
}

Value eval(Expr expr) noexcept {
  switch (expr.type()) {
  case UNARY: {
    Unary ue = expr.is<Unary>();
    Value x = eval(ue.x());
    return eval_unary(ue.kind(), ue.op(), x);
  }
  case BINARY: {
    Binary be = expr.is<Binary>();
    Value x = eval(be.x());
    Value y = eval(be.y());
    return eval_binary(be.op(), x, y);
  }
  case CONST:
    return expr.is<Const>().imm();
  default:
    return Value{};
  }
}

Value eval_unary(Kind to, Op1 op, Value x) noexcept {
  switch (op) {
  case XOR1:
    x = ~x;
    break;
  case NOT1:
    x = !x;
    break;
  case NEG1:
    x = -x;
    break;
  case CAST:
    x = x.cast(to);
    break;
  case BITCOPY:
    x = x.bitcopy(to);
    break;
  default:
    x = Value{};
    break;
  }
  return x;
}

Value eval_binary(Op2 op, Value x, Value y) noexcept {
  switch (op) {
  case ADD:
    x = x + y;
    break;
  case SUB:
    x = x - y;
    break;
  case MUL:
    x = x * y;
    break;
  case QUO:
    x = x / y;
    break;
  case REM:
    x = x % y;
    break;
  case AND:
    x = x & y;
    break;
  case OR:
    x = x | y;
    break;
  case XOR:
    x = x ^ y;
    break;
  case SHL:
    x = x << y;
    break;
  case SHR:
    x = x >> y;
    break;
  case AND_NOT:
    x = and_not(x, y);
    break;
  case LAND:
    x = and_(x, y);
    break;
  case LOR:
    x = or_(x, y);
    break;
  case LSS:
    x = x < y;
    break;
  case LEQ:
    x = x <= y;
    break;
  case NEQ:
    x = x != y;
    break;
  case EQL:
    x = x == y;
    break;
  case GTR:
    x = x > y;
    break;
  case GEQ:
    x = x >= y;
    break;
  default:
    x = Value{};
    break;
  }
  return x;
}

} // namespace onejit
