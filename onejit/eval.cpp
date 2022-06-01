/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * eval.cpp
 *
 *  Created on Feb 07, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/eval.hpp>
#include <onejit/ir/binary.hpp>
#include <onejit/ir/const.hpp>
#include <onejit/ir/tuple.hpp>
#include <onejit/ir/unary.hpp>
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

Value eval_const(Expr expr) noexcept {
  switch (expr.type()) {
  case UNARY: {
    Unary ue = expr.is<Unary>();
    Value x = eval_const(ue.x());
    return eval_unary_op(ue.kind(), ue.op(), x);
  }
  case BINARY: {
    Binary be = expr.is<Binary>();
    Value x = eval_const(be.x());
    Value y = eval_const(be.y());
    return eval_binary_op(be.op(), x, y);
  }
  case TUPLE: {
    Tuple te = expr.is<Tuple>();
    if (te.type() != TUPLE || !is_arithmetic(te.op())) {
      break;
    }
    Value v = Value::identity(te.kind(), te.op());
    for (size_t i = 0, n = te.children(); v.is_valid() && i < n; i++) {
      Value vi = eval_const(te.arg(i));
      v = eval_tuple_op(te.kind(), te.op(), {v, vi});
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

Value eval_unary_op(Kind kind, Op1 op, Value x) noexcept {
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

Value eval_binary_op(Op2 op, Value x, Value y) noexcept {
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

Value eval_tuple_op(Kind kind, OpN op, std::initializer_list<Value> vs) noexcept {
  return eval_tuple_op(kind, op, Values{vs.begin(), vs.size()});
}

Value eval_tuple_op(Kind kind, OpN op, Values vs) noexcept {
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
