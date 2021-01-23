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
  kFloat16 = 11,
  kFloat32 = 12,
  kFloat64 = 13,
  kPtr = 14,
  kArchFlags = 15,

  kHi = 255,
};

class Kind {
public:
  constexpr Kind() noexcept : val_{kVoid} {
  }

  constexpr explicit Kind(eKind ekind) noexcept : val_{ekind} {
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
    return val_ != kBad;
  }

  constexpr bool operator!() const noexcept {
    return val_ == kBad;
  }

  Bits bits() const noexcept;

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

  const Chars &string() const noexcept;
  const Chars &stringsuffix() const noexcept;

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

std::ostream &operator<<(std::ostream &out, Kind kind);
inline std::ostream &operator<<(std::ostream &out, eKind ekind) {
  return out << Kind{ekind};
}

constexpr const Kind Bad(kBad);
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
constexpr const Kind Float16(kFloat16);
constexpr const Kind Float32(kFloat32);
constexpr const Kind Float64(kFloat64);
constexpr const Kind Ptr(kPtr);
constexpr const Kind ArchFlags(kArchFlags);

} // namespace onejit

#endif // ONEJIT_KIND_HPP
