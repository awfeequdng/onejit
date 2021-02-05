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
 * kind.hpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_KIND_HPP
#define ONEJIT_KIND_HPP

#include <onejit/bits.hpp>
#include <onejit/group.hpp>

#include <cstdint> // uint8_t

namespace onejit {

enum eKind : uint8_t {
  eBad = 0,
  eVoid = 1,
  eBool = 2,
  eInt8 = 3,
  eInt16 = 4,
  eInt32 = 5,
  eInt64 = 6,
  eUint8 = 7,
  eUint16 = 8,
  eUint32 = 9,
  eUint64 = 10,
  eFloat16 = 11,
  eFloat32 = 12,
  eFloat64 = 13,
  ePtr = 14,
  eArchFlags = 15,

  eKindHi = 255,
};

constexpr inline eKind operator+(eKind a, int b) noexcept {
  return eKind(int(a) + b);
}

constexpr inline eKind operator-(eKind a, int b) noexcept {
  return eKind(int(a) - b);
}

class Kind {
public:
  // construct an invalid Kind
  constexpr Kind() noexcept : val_{eBad} {
  }

  constexpr Kind(eKind ekind) noexcept : val_{ekind} {
  }

  constexpr explicit Kind(uint8_t val) noexcept : val_{eKind(val)} {
  }

  constexpr Kind(eKind kind, SimdN simd_n) noexcept //
      : val_(eKind((kind & 0xF) | (simd_n.log2() << 4))) {
  }

  constexpr eKind val() const noexcept {
    return val_;
  }

  constexpr explicit operator bool() const noexcept {
    return val_ != eBad;
  }

  constexpr bool operator!() const noexcept {
    return val_ == eBad;
  }

  Bits bits() const noexcept;

  size_t bitsize() const noexcept {
    return bits().val();
  }

  constexpr SimdN simdn() const noexcept {
    return SimdN::fromlog2(val_ >> 4);
  }

  // return a new Kind with specified SimdN repetitions
  constexpr Kind simdn(size_t simd_n) const noexcept {
    return Kind{val_, SimdN{simd_n}};
  }

  // return a new Kind with specified SimdN repetitions
  constexpr Kind simdn(SimdN simd_n) const noexcept {
    return Kind{val_, simd_n};
  }

  // return a new Kind without SimdN repetitions
  constexpr Kind nosimd() const noexcept {
    return Kind{uint8_t(val_ & 0xF)};
  }

  Group group() const noexcept;

  bool is(Group g) const noexcept {
    return group() == g;
  }

  bool is(Group g1, Group g2) const noexcept {
    const Group g = group();
    return g == g1 || g == g2;
  }

  bool is(Group g1, Group g2, Group g3) const noexcept {
    const Group g = group();
    return g == g1 || g == g2 || g == g3;
  }

  bool is(Group g1, Group g2, Group g3, Group g4) const noexcept {
    const Group g = group();
    return g == g1 || g == g2 || g == g3 || g == g4;
  }

  bool is_integer() const noexcept {
    return is(gInt, gUint);
  }

  bool is_integer_or_ptr() const noexcept {
    return is(gInt, gUint, gPtr);
  }

  bool is_unsigned() const noexcept {
    return is(gBool /*remove?*/, gUint, gPtr);
  }

  bool is_ordered() const noexcept {
    return is(gInt, gUint, gFloat, gPtr);
  }

  const Chars string() const noexcept;
  const Chars stringsuffix() const noexcept;

private:
  eKind val_;
};

constexpr inline bool operator==(Kind a, Kind b) noexcept {
  return a.val() == b.val();
}

constexpr inline bool operator!=(Kind a, Kind b) noexcept {
  return a.val() != b.val();
}

constexpr inline bool operator<(Kind a, Kind b) noexcept {
  return a.val() < b.val();
}

constexpr inline bool operator<=(Kind a, Kind b) noexcept {
  return a.val() <= b.val();
}

constexpr inline bool operator>(Kind a, Kind b) noexcept {
  return a.val() > b.val();
}

constexpr inline bool operator>=(Kind a, Kind b) noexcept {
  return a.val() >= b.val();
}

const Fmt &operator<<(const Fmt &out, Kind kind);
inline const Fmt &operator<<(const Fmt &out, eKind ekind) {
  return out << Kind{ekind};
}

constexpr const Kind Bad(eBad);
constexpr const Kind Void(eVoid);
constexpr const Kind Bool(eBool);
constexpr const Kind Int8(eInt8);
constexpr const Kind Int16(eInt16);
constexpr const Kind Int32(eInt32);
constexpr const Kind Int64(eInt64);
constexpr const Kind Uint8(eUint8);
constexpr const Kind Uint16(eUint16);
constexpr const Kind Uint32(eUint32);
constexpr const Kind Uint64(eUint64);
constexpr const Kind Float16(eFloat16);
constexpr const Kind Float32(eFloat32);
constexpr const Kind Float64(eFloat64);
constexpr const Kind Ptr(ePtr);
constexpr const Kind ArchFlags(eArchFlags);

} // namespace onejit

#endif // ONEJIT_KIND_HPP
