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

// convert float32 to its bit representation and vice versa
union Float32Bits {
public:
  constexpr Float32Bits() noexcept : float_{0.0f} {
  }
  constexpr explicit Float32Bits(float val) noexcept : float_{val} {
  }
  constexpr explicit Float32Bits(uint32_t bits) noexcept : bits_{bits} {
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

// convert float64 to its bit representation and vice versa
union Float64Bits {
public:
  constexpr Float64Bits() noexcept : float_{0.0} {
  }
  constexpr explicit Float64Bits(double val) noexcept : float_{val} {
  }
  constexpr explicit Float64Bits(uint64_t bits) noexcept : bits_{bits} {
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
      : bits_{Float32Bits{val}.bits()}, ekind_{eFloat32} {
  }
  constexpr explicit Value(double val) noexcept //
      : bits_{Float64Bits{val}.bits()}, ekind_{eFloat64} {
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

  constexpr bool is_valid() const noexcept {
    return ekind_ != eBad;
  }

  // return true if Value != zero,
  // as C operator bool does on POD types
  explicit operator bool() const noexcept;

  // return true if Value == zero,
  // as C operator ! does on POD types
  Value operator!() const noexcept;

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
    return Float32Bits{uint32_t(bits_)}.val();
  }
  constexpr double float64() const noexcept {
    return Float64Bits{bits_}.val();
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

// return a Value = 0 with specified kind
constexpr Value ZeroValue(Kind kind) noexcept {
  return Value{kind, uint64_t(0)};
}

// ----------------------------- formatting ------------------------------------

const Fmt &operator<<(const Fmt &out, const Value &value);

// ----------------------------- binary operators ------------------------------

Value operator+(Value a, Value b) noexcept;
Value operator-(Value a, Value b) noexcept;
Value operator*(Value a, Value b) noexcept;
Value operator/(Value a, Value b) noexcept;
Value operator%(Value a, Value b) noexcept;
Value operator&(Value a, Value b) noexcept;
Value operator|(Value a, Value b) noexcept;
Value operator^(Value a, Value b) noexcept;
Value operator<<(Value a, Value b) noexcept;
Value operator>>(Value a, Value b) noexcept;

/**
 * relational operators return Value{} if a and b have different kind.
 * they also use floating point comparison when a and b are floating point,
 * which has non-trivial behavior for not-a-number (NaN) and +0 vs -0
 */
Value operator==(Value a, Value b) noexcept;
Value operator!=(Value a, Value b) noexcept;
Value operator<(Value a, Value b) noexcept;
Value operator<=(Value a, Value b) noexcept;
inline Value operator>(Value a, Value b) noexcept {
  return b < a;
}
inline Value operator>=(Value a, Value b) noexcept {
  return b <= a;
}

// return true if Values a and b have the same kind and bits
// differs from operator== when kind is Bad, or floating point +/- 0,
// or floating point not-a-number
constexpr bool identical(Value a, Value b) noexcept {
  return a.kind() == b.kind() && a.bits() == b.bits();
}

// if a and b are boolean, return their &&
// otherwise return Value{}
constexpr Value and_(Value a, Value b) noexcept {
  return (a.kind() == Bool && b.kind() == Bool) //
             ? Value{a.boolean() && b.boolean()}
             : Value{};
}

// if a and b are boolean, return their &&
// otherwise return Value{}
constexpr Value or_(Value a, Value b) noexcept {
  return (a.kind() == Bool && b.kind() == Bool) //
             ? Value{a.boolean() || b.boolean()}
             : Value{};
}

// ----------------------------- assignment operators --------------------------

inline Value &operator+=(Value &a, Value b) noexcept {
  return a = a + b;
}
inline Value &operator-=(Value &a, Value b) noexcept {
  return a = a - b;
}
inline Value &operator*=(Value &a, Value b) noexcept {
  return a = a * b;
}
inline Value &operator/=(Value &a, Value b) noexcept {
  return a = a / b;
}
inline Value &operator%=(Value &a, Value b) noexcept {
  return a = a % b;
}
inline Value &operator&=(Value &a, Value b) noexcept {
  return a = a & b;
}
inline Value &operator|=(Value &a, Value b) noexcept {
  return a = a | b;
}
inline Value &operator^=(Value &a, Value b) noexcept {
  return a = a ^ b;
}
inline Value &operator<<=(Value &a, Value b) noexcept {
  return a = a << b;
}
inline Value &operator>>=(Value &a, Value b) noexcept {
  return a = a >> b;
}

// ----------------------------- unary operators -------------------------------

inline Value::operator bool() const noexcept {
  return (*this != ZeroValue(kind())).boolean();
}

inline Value Value::operator!() const noexcept {
  return *this == ZeroValue(kind());
}

Value operator~(Value a) noexcept;
Value operator-(Value a) noexcept;
constexpr Value operator+(Value a) noexcept {
  return a;
}

} // namespace onejit

#endif // ONEJIT_VALUE_HPP
