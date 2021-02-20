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

#include <onejit/eval.hpp>
#include <onejit/node/binary.hpp>
#include <onejit/node/const.hpp>
#include <onejit/node/tuple.hpp>
#include <onejit/node/unary.hpp>
#include <onestl/view.hpp>

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
    case TUPLE: {
      Tuple te = expr.is<Tuple>();
      if (te.type() != TUPLE || !is_arithmetic(te.op())) {
        break;
      }
      for (size_t i = 0, n = te.children(); i < n; i++) {
        if (!is_const(te.arg(i))) {
          break;
        }
      }
      return true;
    }
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
  case TUPLE: {
    Tuple te = expr.is<Tuple>();
    if (te.type() != TUPLE || !is_arithmetic(te.op())) {
      break;
    }
    Value v = Value::identity(te.kind(), te.op());
    for (size_t i = 0, n = te.children(); v.is_valid() && i < n; i++) {
      Value vi = eval(te.arg(i));
      v = eval_tuple(te.kind(), te.op(), {v, vi});
    }
    return v;
  }
  case CONST:
    return expr.is<Const>().val();
  default:
    break;
  }
  return Value{};
}

Value eval_unary(Kind kind, Op1 op, Value x) noexcept {
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
    x = x.cast(kind);
    break;
  case BITCOPY:
    x = x.bitcopy(kind);
    break;
  default:
    x = Value{};
    break;
  }
  return x;
}

Value eval_binary(Op2 op, Value x, Value y) noexcept {
  switch (op) {
  case SUB:
    x = x - y;
    break;
  case QUO:
    x = x / y;
    break;
  case REM:
    x = x % y;
    break;
  case SHL:
    x = x << y;
    break;
  case SHR:
    x = x >> y;
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

Value eval_tuple(Kind kind, OpN op, std::initializer_list<Value> vs) noexcept {
  return eval_tuple(kind, op, Values{vs.begin(), vs.size()});
}

Value eval_tuple(Kind kind, OpN op, Values vs) noexcept {
  Value x = Value::identity(kind, op);
  switch (op) {
  case ADD:
    for (const Value &v : vs) {
      x += v;
    }
    break;
  case MUL:
    for (const Value &v : vs) {
      x *= v;
    }
    break;
  case AND:
    for (const Value &v : vs) {
      x &= v;
    }
    break;
  case OR:
    for (const Value &v : vs) {
      x |= v;
    }
    break;
  case XOR:
    for (const Value &v : vs) {
      x ^= v;
    }
    break;
  case MAX:
    for (const Value &v : vs) {
      Value cmp = x < v;
      if (cmp.is_valid()) {
        x = cmp ? v : x;
      } else {
        return Value{};
      }
    }
    break;
  case MIN:
    for (const Value &v : vs) {
      Value cmp = x < v;
      if (cmp.is_valid()) {
        x = cmp ? x : v;
      } else {
        return Value{};
      }
    }
    break;
  case COMMA:
    if (size_t n = vs.size()) {
      for (const Value &v : vs) {
        if (!v.is_valid()) {
          return Value{};
        }
      }
      x = vs[n - 1];
    }
    break;
  default:
    break;
  }
  return x;
}

} // namespace onejit
