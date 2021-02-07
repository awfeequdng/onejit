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
 * value.hpp
 *
 *  Created on Feb 07, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_VALUE_HPP
#define ONEJIT_VALUE_HPP

#include <onejit/kind.hpp>

namespace onejit {

union Float32Value {
public:
  constexpr Float32Value() noexcept : float_{0.0f} {
  }
  constexpr explicit Float32Value(float val) noexcept : float_{val} {
  }
  constexpr explicit Float32Value(uint32_t bits) noexcept : bits_{bits} {
  }

  constexpr float val() const noexcept {
    return float_;
  }
  constexpr uint32_t bits() const noexcept {
    return bits_;
  }

private:
  float float_;
  uint32_t bits_;
};

////////////////////////////////////////////////////////////////////////////////

union Float64Value {
public:
  constexpr Float64Value() noexcept : float_{0.0} {
  }
  constexpr explicit Float64Value(double val) noexcept : float_{val} {
  }
  constexpr explicit Float64Value(uint64_t bits) noexcept : bits_{bits} {
  }

  constexpr double val() const noexcept {
    return float_;
  }
  constexpr uint64_t bits() const noexcept {
    return bits_;
  }

private:
  double float_;
  uint64_t bits_;
};

////////////////////////////////////////////////////////////////////////////////

class Value {

public:
  // construct an invalid Value.
  constexpr Value() noexcept //
      : bits_{}, ekind_{eBad} {
  }
  constexpr explicit Value(bool val) noexcept //
      : bits_{uint64_t(val)}, ekind_{eBool} {
  }
  constexpr explicit Value(int8_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{eInt8} {
  }
  constexpr explicit Value(int16_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{eInt16} {
  }
  constexpr explicit Value(int32_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{eInt32} {
  }
  constexpr explicit Value(int64_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{eInt64} {
  }
  constexpr explicit Value(uint8_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{eUint8} {
  }
  constexpr explicit Value(uint16_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{eUint16} {
  }
  constexpr explicit Value(uint32_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{eUint32} {
  }
  constexpr explicit Value(uint64_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{eUint64} {
  }
  constexpr explicit Value(float val) noexcept //
      : bits_{Float32Value{val}.bits()}, ekind_{eFloat32} {
  }
  constexpr explicit Value(double val) noexcept //
      : bits_{Float64Value{val}.bits()}, ekind_{eFloat64} {
  }
  constexpr Value(Kind kind, uint64_t bits) noexcept //
      : bits_{bits}, ekind_{kind.val()} {
  }

  constexpr Kind kind() const noexcept {
    return Kind{ekind_};
  }

  constexpr eKind ekind() const noexcept {
    return ekind_;
  }

  constexpr explicit operator bool() const noexcept {
    return ekind_ != eBad;
  }

  constexpr bool boolean() const noexcept {
    return bool(bits_);
  }

  constexpr int8_t int8() const noexcept {
    return int8_t(bits_);
  }
  constexpr int16_t int16() const noexcept {
    return int16_t(bits_);
  }
  constexpr int32_t int32() const noexcept {
    return int32_t(bits_);
  }
  constexpr int64_t int64() const noexcept {
    return int64_t(bits_);
  }

  constexpr uint8_t uint8() const noexcept {
    return uint8_t(bits_);
  }
  constexpr uint16_t uint16() const noexcept {
    return uint16_t(bits_);
  }
  constexpr uint32_t uint32() const noexcept {
    return uint32_t(bits_);
  }
  constexpr uint64_t uint64() const noexcept {
    return uint64_t(bits_);
  }

  constexpr float float32() const noexcept {
    return Float32Value{uint32_t(bits_)}.val();
  }
  constexpr double float64() const noexcept {
    return Float64Value{bits_}.val();
  }

  void *ptr() const noexcept {
    return reinterpret_cast<void *>(size_t(bits_));
  }

  constexpr uint64_t bits() const noexcept {
    return bits_;
  }

  // change value's Kind, keeping the same bits
  Value bitcopy(Kind to) noexcept;

  // convert between kinds, for example between float and int
  Value cast(Kind to) noexcept;

private:
  uint64_t bits_;
  eKind ekind_;
};

constexpr Value operator+(Value a) noexcept {
  return a;
}
Value operator-(Value a) noexcept;
Value operator~(Value a) noexcept;
Value operator+(Value a, Value b) noexcept;
Value operator-(Value a, Value b) noexcept;
Value operator*(Value a, Value b) noexcept;
Value operator/(Value a, Value b) noexcept;
Value operator%(Value a, Value b) noexcept;
constexpr Value operator&(Value a, Value b) noexcept {
  return Value{a.kind(), a.bits() & b.bits()};
}
constexpr Value operator|(Value a, Value b) noexcept {
  return Value{a.kind(), a.bits() | b.bits()};
}
constexpr Value operator^(Value a, Value b) noexcept {
  return Value{a.kind(), a.bits() ^ b.bits()};
}
Value operator<<(Value a, Value b) noexcept;
Value operator>>(Value a, Value b) noexcept;
Value and_not(Value a, Value b) noexcept;

bool operator<(Value a, Value b) noexcept;
inline bool operator<=(Value a, Value b) noexcept {
  return !(b < a);
}
inline bool operator>(Value a, Value b) noexcept {
  return b < a;
}
inline bool operator>=(Value a, Value b) noexcept {
  return !(a < b);
}

constexpr inline bool operator==(Value a, Value b) noexcept {
  return a.kind() == b.kind() && a.bits() == b.bits();
}

constexpr inline bool operator!=(Value a, Value b) noexcept {
  return a.kind() != b.kind() || a.bits() != b.bits();
}

const Fmt &operator<<(const Fmt &out, const Value &value);

} // namespace onejit

#endif // ONEJIT_VALUE_HPP
