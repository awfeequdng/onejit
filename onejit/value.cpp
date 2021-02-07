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
 * value.cpp
 *
 *  Created on Feb 07, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/fmt.hpp>
#include <onejit/value.hpp>
#include <onestl/chars.hpp>

namespace onejit {

static ONEJIT_NOINLINE Value extend_or_truncate(Kind kind, uint64_t bits) noexcept {
  size_t bitsize = kind.bitsize();
  if (bitsize == 0) {
    bits = 0;
  } else {
    uint64_t lomask = ~(uint64_t)0 >> (64 - bitsize);
    bits &= lomask;
    if (kind.is(gInt) && (bits & (lomask >> 1)) != bits) {
      bits |= ~lomask;
    }
  }
  return Value{kind, bits};
}

Value Value::bitcopy(Kind to) noexcept {
  return extend_or_truncate(to, bits_);
}

Value operator-(Value a) noexcept {
  return extend_or_truncate(a.kind(), -a.bits());
}

Value operator~(Value a) noexcept {
  return extend_or_truncate(a.kind(), ~a.bits());
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

Value and_not(Value a, Value b) noexcept {
  return a & ~b;
}

bool operator<(Value a, Value b) noexcept {
  Kind kind = a.kind();
  if (kind != b.kind()) {
    return false;
  } else if (kind == Float32) {
    return a.float32() < b.float32();
  } else if (kind == Float64) {
    return a.float64() < b.float64();
  } else if (kind.is(gInt)) {
    return a.int64() < b.int64();
  } else {
    return a.uint64() < b.uint64();
  }
}

////////////////////////////////////////////////////////////////////////////////

const Fmt &operator<<(const Fmt &out, const Value &value) {
  const Kind kind = value.kind();
  switch (kind.nosimd().val()) {
  case eVoid:
    out << "void";
    break;
  case eBool:
    out << (value.boolean() ? Chars("true") : Chars("false"));
    break;
  case eInt8:
  case eInt16:
  case eInt32:
  case eInt64:
    out << value.int64();
    break;
  case eUint8:
  case eUint16:
  case eUint32:
  case eUint64:
  case eArchFlags:
    out << value.uint64();
    break;
  case eFloat32:
    out << value.float32();
    break;
  case eFloat64:
    out << value.float64();
    break;
  case ePtr:
    out << value.ptr();
    break;
  default:
    out << '?';
    break;
  }
  const size_t n = kind.simdn().val();
  if (n > 1) {
    out << '_' << kind.stringsuffix() << 'x' << n;
  }
  return out;
}

} // namespace onejit
