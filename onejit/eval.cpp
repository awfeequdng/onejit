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

#include <onejit/binary.hpp>
#include <onejit/const.hpp>
#include <onejit/eval.hpp>
#include <onejit/unary.hpp>

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
    x = Value{!x};
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

Value Value::cast(Kind to) noexcept {
  Kind from = kind();
  uint64_t bits = 0;
  if (from == to) {
    return *this;
  } else if (to == Float32) {
    float f = 0.0f;
    switch (from.val()) {
    case eVoid:
      break;
    case eBool:
      f = float(boolean());
      break;
    case eInt8:
      f = float(int8());
      break;
    case eInt16:
      f = float(int16());
      break;
    case eInt32:
      f = float(int32());
      break;
    case eInt64:
      f = float(int64());
      break;
    case eUint8:
      f = float(uint8());
      break;
    case eUint16:
      f = float(uint16());
      break;
    case eUint32:
      f = float(uint32());
      break;
    case eUint64:
    case ePtr:
    case eArchFlags:
      f = float(uint64());
      break;
    case eFloat64:
      f = float(float64());
      break;
    default:
      return Value{};
    }
    return Value{f};
  } else if (to == Float64) {
    double f = 0.0f;
    switch (from.val()) {
    case eVoid:
      break;
    case eBool:
      f = double(boolean());
      break;
    case eInt8:
      f = double(int8());
      break;
    case eInt16:
      f = double(int16());
      break;
    case eInt32:
      f = double(int32());
      break;
    case eInt64:
      f = double(int64());
      break;
    case eUint8:
      f = double(uint8());
      break;
    case eUint16:
      f = double(uint16());
      break;
    case eUint32:
      f = double(uint32());
      break;
    case eUint64:
    case ePtr:
    case eArchFlags:
      f = double(uint64());
      break;
    case eFloat32:
      f = double(float32());
      break;
    default:
      return Value{};
    }
    return Value{f};
  } else if (from == Float32) {
    float f = float32();
    switch (to.val()) {
    case eVoid:
      break;
    case eBool:
      bits = uint64_t(f != 0.0f);
      break;
    case eInt8:
      bits = int8_t(f);
      break;
    case eInt16:
      bits = int16_t(f);
      break;
    case eInt32:
      bits = int32_t(f);
      break;
    case eInt64:
      bits = int64_t(f);
      break;
    case eUint8:
      bits = uint8_t(f);
      break;
    case eUint16:
      bits = uint16_t(f);
      break;
    case eUint32:
      bits = uint32_t(f);
      break;
    case eUint64:
    case ePtr:
    case eArchFlags:
      bits = uint64_t(f);
      break;
    case eFloat64:
      bits = Float64Value{double(f)}.bits();
      break;
    default:
      return Value{};
    }
  } else if (from == Float64) {
    double f = float64();
    switch (to.val()) {
    case eVoid:
      break;
    case eBool:
      bits = uint64_t(f != 0.0f);
      break;
    case eInt8:
      bits = int8_t(f);
      break;
    case eInt16:
      bits = int16_t(f);
      break;
    case eInt32:
      bits = int32_t(f);
      break;
    case eInt64:
      bits = int64_t(f);
      break;
    case eUint8:
      bits = uint8_t(f);
      break;
    case eUint16:
      bits = uint16_t(f);
      break;
    case eUint32:
      bits = uint32_t(f);
      break;
    case eUint64:
    case ePtr:
    case eArchFlags:
      bits = uint64_t(f);
      break;
    case eFloat32:
      bits = Float32Value{float(f)}.bits();
      break;
    default:
      return Value{};
    }
  } else {
    bits = bits_;
  }
  return Value{to, bits};
}

} // namespace onejit
