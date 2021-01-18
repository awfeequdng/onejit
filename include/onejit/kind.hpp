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
 *  Created on Jan 08, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_KIND_HPP
#define ONEJIT_KIND_HPP

#include <onejit/group.hpp>
#include <onejit/logsize.hpp>

#include <cstdint> // uint8_t
#include <iosfwd>  // std::ostream

namespace onejit {

enum eKind : uint8_t {
  kBad = 0,
  kVoid = 1,
  kBool = 2,
  kInt8 = 3,
  kInt16 = 4,
  kInt32 = 5,
  kInt64 = 6,
  kUint8 = 7,
  kUint16 = 8,
  kUint32 = 9,
  kUint64 = 10,
  kFloat32 = 11,
  kFloat64 = 12,
  // kComplex*
  kPtr = 14,
  kArchFlags = 15,

  kHi = 255,
};

class Kind {
public:
  constexpr Kind() : val_{kVoid} {
  }

  constexpr explicit Kind(eKind ekind) : val_{ekind} {
  }

  constexpr explicit Kind(uint8_t val) : val_{eKind(val)} {
  }

  constexpr Kind(eKind kind, SimdN simd_n) //
      : val_(eKind((kind & 0xF) | (simd_n.log2() << 4))) {
  }

  constexpr eKind val() const {
    return val_;
  }

  Bits bits() const;

  constexpr SimdN simdn() const {
    return SimdN::fromlog2(val_ >> 4);
  }

  // return a new Kind with specified SimdN repetitions
  constexpr Kind simdn(SimdN simd_n) const {
    return Kind{val_, simd_n};
  }

  // return a new Kind without SimdN repetitions
  constexpr Kind nosimd() const {
    return Kind{uint8_t(val_ & 0xF)};
  }

  Group group() const;

  bool is(Group g) const {
    return group() == g;
  }

  bool is(Group g1, Group g2) const {
    const Group g = group();
    return g == g1 || g == g2;
  }

  bool is(Group g1, Group g2, Group g3) const {
    const Group g = group();
    return g == g1 || g == g2 || g == g3;
  }

  bool is(Group g1, Group g2, Group g3, Group g4) const {
    const Group g = group();
    return g == g1 || g == g2 || g == g3 || g == g4;
  }

  bool is_integer() const {
    return is(gInt, gUint);
  }

  bool is_integer_or_ptr() const {
    return is(gInt, gUint, gPtr);
  }

  bool is_unsigned() const {
    return is(gBool /*remove?*/, gUint, gPtr);
  }

  bool is_ordered() const {
    return is(gInt, gUint, gFloat, gPtr);
  }

  const Chars &string() const;
  const Chars &stringsuffix() const;

private:
  eKind val_;
};

constexpr inline bool operator==(Kind a, Kind b) {
  return a.val() == b.val();
}

constexpr inline bool operator!=(Kind a, Kind b) {
  return a.val() != b.val();
}

constexpr inline bool operator<(Kind a, Kind b) {
  return a.val() < b.val();
}

constexpr inline bool operator<=(Kind a, Kind b) {
  return a.val() <= b.val();
}

constexpr inline bool operator>(Kind a, Kind b) {
  return a.val() > b.val();
}

constexpr inline bool operator>=(Kind a, Kind b) {
  return a.val() >= b.val();
}

std::ostream &operator<<(std::ostream &out, Kind kind);
inline std::ostream &operator<<(std::ostream &out, eKind ekind) {
  return out << Kind{ekind};
}

constexpr const Kind BadKind(kBad);
constexpr const Kind Void(kVoid);
constexpr const Kind Bool(kBool);
constexpr const Kind Int8(kInt8);
constexpr const Kind Int16(kInt16);
constexpr const Kind Int32(kInt32);
constexpr const Kind Int64(kInt64);
constexpr const Kind Uint8(kUint8);
constexpr const Kind Uint16(kUint16);
constexpr const Kind Uint32(kUint32);
constexpr const Kind Uint64(kUint64);
constexpr const Kind Float32(kFloat32);
constexpr const Kind Float64(kFloat64);
// Complex64
// Complex128
constexpr const Kind Ptr(kPtr);
constexpr const Kind ArchFlags(kArchFlags);

} // namespace onejit

#endif // ONEJIT_KIND_HPP
