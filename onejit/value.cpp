/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * value.cpp
 *
 *  Created on Feb 07, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/op.hpp>
#include <onejit/value.hpp>

namespace onejit {

static ONEJIT_NOINLINE Value extend_or_truncate(Kind kind, uint64_t bits) noexcept {
  size_t bitsize = kind.bitsize();
  if (bitsize == 0) {
    bits = 0;
  } else {
    uint64_t lomask = ~(uint64_t)0 >> (64 - bitsize);
    bits &= lomask;
    if (kind.is(gInt) && (bits & (lomask >> 1)) != bits) {
      // sign-extend to 64 bits
      bits |= ~lomask;
    }
  }
  return Value{kind, bits};
}

Value Value::bitcopy(Kind to) noexcept {
  return extend_or_truncate(to, bits_);
}

Value Value::cast(Kind to) noexcept {
  Kind from = kind();
  if (from == to) {
    return *this;
  }
  if (to == Float32) {
    float f = 0.0f;
    switch (from.val()) {
    case eVoid:
      break;
    case eBool:
      f = float(boolean());
      break;
    case eInt8:
    case eInt16:
    case eInt32:
    case eInt64:
      f = float(int64());
      break;
    case eUint8:
    case eUint16:
    case eUint32:
    case eUint64:
    case ePtr:
      f = float(uint64());
      break;
    case eFloat64:
    case eFloat128:
      f = float(float64());
      break;
    default:
      return Value{};
    }
    return Value{f};
  }
  if (to == Float64) {
    double f = 0.0f;
    switch (from.val()) {
    case eVoid:
      break;
    case eBool:
      f = double(boolean());
      break;
    case eInt8:
    case eInt16:
    case eInt32:
    case eInt64:
      f = double(int64());
      break;
    case eUint8:
    case eUint16:
    case eUint32:
    case eUint64:
    case ePtr:
      f = double(uint64());
      break;
    case eFloat32:
      f = double(float32());
      break;
    case eFloat128:
      f = double(float64());
      break;
    default:
      return Value{};
    }
    return Value{f};
  }
  uint64_t bits = 0;
  if (from == Float32) {
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
      bits = uint64_t(f);
      break;
    case eFloat64:
    case eFloat128:
      bits = Float64Bits{double(f)}.bits();
      break;
    default:
      return Value{};
    }
    return Value{to, bits};
  }
  if (from == Float64) {
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
      bits = uint64_t(f);
      break;
    case eFloat32:
      bits = Float32Bits{float(f)}.bits();
      break;
    case eFloat128:
      bits = Float64Bits{f}.bits();
      break;
    default:
      return Value{};
    }
    return Value{to, bits};
  }
  return extend_or_truncate(to, bits_);
}

Value Value::max(Kind kind) noexcept {
  if (kind == Float32) {
    return Value{float(1.0f / 0.0f)};
  } else if (kind == Float64) {
    return Value{double(1.0 / 0.0)};
  }
  const size_t bitsize = kind.bitsize();
  uint64_t bits = bitsize == 0 ? uint64_t(0) : uint64_t(-1) >> (64 - bitsize);
  if (kind.is_signed()) {
    bits >>= 1;
  }
  return Value{kind, bits};
}

Value Value::min(Kind kind) noexcept {
  if (kind.is_unsigned()) {
    return zero(kind);
  } else if (kind == Float32) {
    return Value{float(-1.0f / 0.0f)};
  } else if (kind == Float64) {
    return Value{double(-1.0 / 0.0)};
  }
  return Value{kind, uint64_t(1) << (kind.bitsize() - 1)};
}

/// @return the identity element for specified operation and kind
Value Value::identity(Kind kind, Op2 op) noexcept {
  uint64_t i;
  switch (op) {
  case SUB:
  case SHL:
  case SHR:
    i = 0;
    break;
  case QUO:
    i = 1;
    break;
  default:
    return Value{};
  }
  return Value{i}.cast(kind);
}

/// @return the identity element for specified operation and kind
Value Value::identity(Kind kind, OpN op) noexcept {
  if (is_bitwise(op) && kind.is_float()) {
    return Value{};
  }
  uint64_t i;
  switch (op) {
  case ADD:
  case OR:
  case XOR:
    i = 0;
    break;
  case MUL:
    i = 1;
    break;
  case AND:
    i = uint64_t(-1);
    break;
  case MAX:
    return min(kind);
  case MIN:
    return max(kind);
  case COMMA:
    i = 0;
    break;
  default:
    return Value{};
  }
  return Value{i}.cast(kind);
}

/// @return the absorbing element for specified operation and kind
Value Value::absorbing(Kind kind, OpN op) noexcept {
  if (is_bitwise(op) && kind.is_float()) {
    return Value{};
  }
  uint64_t i;
  switch (op) {
  case OR:
    i = uint64_t(-1);
    break;
  case MUL:
  case AND:
    i = 0;
    break;
  case MAX:
    return max(kind);
  case MIN:
    return min(kind);
  default:
    return Value{};
  }
  return Value{i}.cast(kind);
}

////////////////////////////////////////////////////////////////////////////////

Value operator~(Value a) noexcept {
  Kind kind = a.kind();
  if (kind == Float32 || kind == Float64) {
    return Value{};
  } else {
    return extend_or_truncate(a.kind(), ~a.bits());
  }
}

Value operator-(Value a) noexcept {
  Kind kind = a.kind();
  if (kind == Float32) {
    return Value{-a.float32()};
  } else if (kind == Float64) {
    return Value{-a.float64()};
  } else {
    return extend_or_truncate(a.kind(), -a.bits());
  }
}

Value operator+(Value a, Value b) noexcept {
  Kind kind = a.kind();
  if (kind != b.kind()) {
    return Value{};
  } else if (kind == Float32) {
    return Value{a.float32() + b.float32()};
  } else if (kind == Float64) {
    return Value{a.float64() + b.float64()};
  } else {
    return extend_or_truncate(a.kind(), a.uint64() + b.uint64());
  }
}

Value operator-(Value a, Value b) noexcept {
  Kind kind = a.kind();
  if (kind != b.kind()) {
    return Value{};
  } else if (kind == Float32) {
    return Value{a.float32() - b.float32()};
  } else if (kind == Float64) {
    return Value{a.float64() - b.float64()};
  } else {
    return extend_or_truncate(a.kind(), a.uint64() - b.uint64());
  }
}

Value operator*(Value a, Value b) noexcept {
  Kind kind = a.kind();
  if (kind != b.kind()) {
    return Value{};
  } else if (kind == Float32) {
    return Value{a.float32() * b.float32()};
  } else if (kind == Float64) {
    return Value{a.float64() * b.float64()};
  } else {
    return extend_or_truncate(a.kind(), a.uint64() * b.uint64());
  }
}

Value operator/(Value a, Value b) noexcept {
  Kind kind = a.kind();
  if (kind != b.kind()) {
    return Value{};
  } else if (kind == Float32) {
    return Value{a.float32() / b.float32()};
  } else if (kind == Float64) {
    return Value{a.float64() / b.float64()};
  } else if (kind.is(gInt)) {
    return extend_or_truncate(a.kind(), a.int64() / b.int64());
  } else {
    return extend_or_truncate(a.kind(), a.uint64() / b.uint64());
  }
}

Value operator%(Value a, Value b) noexcept {
  Kind kind = a.kind();
  if (kind != b.kind() || kind == Float32 || kind == Float64) {
    return Value{};
  } else if (kind.is(gInt)) {
    return extend_or_truncate(a.kind(), a.int64() % b.int64());
  } else {
    return extend_or_truncate(a.kind(), a.uint64() % b.uint64());
  }
}

Value operator&(Value a, Value b) noexcept {
  Kind kind = a.kind();
  if (kind != b.kind() || kind == Float32 || kind == Float64) {
    return Value{};
  } else {
    return Value{kind, a.bits() & b.bits()};
  }
}

Value operator|(Value a, Value b) noexcept {
  Kind kind = a.kind();
  if (kind != b.kind() || kind == Float32 || kind == Float64) {
    return Value{};
  } else {
    return Value{kind, a.bits() | b.bits()};
  }
}

Value operator^(Value a, Value b) noexcept {
  Kind kind = a.kind();
  if (kind != b.kind() || kind == Float32 || kind == Float64) {
    return Value{};
  } else {
    return Value{kind, a.bits() ^ b.bits()};
  }
}

Value operator<<(Value a, Value b) noexcept {
  Kind kind = a.kind();
  if (kind != b.kind() || kind == Float32 || kind == Float64) {
    return Value{};
  } else {
    return extend_or_truncate(a.kind(), a.uint64() << b.uint64());
  }
}

Value operator>>(Value a, Value b) noexcept {
  Kind kind = a.kind();
  if (kind != b.kind() || kind == Float32 || kind == Float64) {
    return Value{};
  } else if (kind.is(gInt)) {
    return extend_or_truncate(a.kind(), a.int64() >> b.int64());
  } else {
    return extend_or_truncate(a.kind(), a.uint64() >> b.uint64());
  }
}

Value operator==(Value a, Value b) noexcept {
  Kind kind = a.kind();
  bool ret;
  if (kind != b.kind()) {
    return Value{};
  } else if (kind == Float32) {
    ret = a.float32() == b.float32();
  } else if (kind == Float64) {
    ret = a.float64() == b.float64();
  } else {
    ret = a.bits() == b.bits();
  }
  return Value{ret};
}

Value operator!=(Value a, Value b) noexcept {
  Kind kind = a.kind();
  bool ret;
  if (kind != b.kind()) {
    return Value{};
  } else if (kind == Float32) {
    ret = a.float32() != b.float32();
  } else if (kind == Float64) {
    ret = a.float64() != b.float64();
  } else {
    ret = a.bits() != b.bits();
  }
  return Value{ret};
}

Value operator<(Value a, Value b) noexcept {
  Kind kind = a.kind();
  bool ret;
  if (kind != b.kind()) {
    return Value{};
  } else if (kind == Float32) {
    ret = a.float32() < b.float32();
  } else if (kind == Float64) {
    ret = a.float64() < b.float64();
  } else if (kind.is(gInt)) {
    ret = a.int64() < b.int64();
  } else {
    ret = a.uint64() < b.uint64();
  }
  return Value{ret};
}

Value operator<=(Value a, Value b) noexcept {
  Kind kind = a.kind();
  bool ret;
  if (kind != b.kind()) {
    return Value{};
  } else if (kind == Float32) {
    ret = a.float32() <= b.float32();
  } else if (kind == Float64) {
    ret = a.float64() <= b.float64();
  } else if (kind.is(gInt)) {
    ret = a.int64() <= b.int64();
  } else {
    ret = a.uint64() <= b.uint64();
  }
  return Value{ret};
}

} // namespace onejit
