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

class Chars;

class Kind {
public:
  constexpr Kind() : val_(0 /*Void*/) {
  }

  constexpr explicit Kind(uint8_t kind) : val_(kind) {
  }

  constexpr Kind(uint8_t kind, SimdN simd_n) //
      : val_((kind & 0xF) | (simd_n.log2() << 4)) {
  }

  constexpr uint8_t val() const {
    return val_;
  }

  Bits bits() const;

  constexpr SimdN simdn() const {
    return SimdN::fromlog2(val_ >> 4);
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
  uint8_t val_;
};

constexpr bool operator==(Kind a, Kind b) {
  return a.val() == b.val();
}

constexpr bool operator!=(Kind a, Kind b) {
  return a.val() != b.val();
}

std::ostream &operator<<(std::ostream &out, Kind kind);

constexpr const Kind Void(0);
constexpr const Kind Bool(1);
constexpr const Kind Int8(2);
constexpr const Kind Int16(3);
constexpr const Kind Int32(4);
constexpr const Kind Int64(5);
constexpr const Kind Uint8(6);
constexpr const Kind Uint16(7);
constexpr const Kind Uint32(8);
constexpr const Kind Uint64(9);
constexpr const Kind Float32(10);
constexpr const Kind Float64(11);
constexpr const Kind Complex64(12);
constexpr const Kind Complex128(13);
constexpr const Kind Ptr(14);
constexpr const Kind ArchFlags(15);

} // namespace onejit

#endif // ONEJIT_KIND_HPP
