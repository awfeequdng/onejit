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
#include <onejit/size.hpp>

#include <cstdint> // uint8_t
#include <iosfwd>  // std::ostream
#include <string>

namespace onejit {

class Kind {
public:
  constexpr explicit Kind(uint8_t kind) : val_(kind) {
  }

  constexpr uint8_t val() const {
    return val_;
  }

  Size size() const;

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
    return is(gBool /*remove?*/, gUint);
  }

  bool is_ordered() const {
    return is(gInt, gUint, gFloat, gPtr);
  }

  const std::string &string() const;

private:
  uint8_t val_;
};

std::ostream &operator<<(std::ostream &out, Kind kind);

// intentionally matches Go reflect.Kind values
constexpr const Kind Void(0);
constexpr const Kind Bool(1);
// Int
constexpr const Kind Int8(3);
constexpr const Kind Int16(4);
constexpr const Kind Int32(5);
constexpr const Kind Int64(6);
// Uint
constexpr const Kind Uint8(8);
constexpr const Kind Uint16(9);
constexpr const Kind Uint32(10);
constexpr const Kind Uint64(11);
// Uintptr
constexpr const Kind Float32(13);
constexpr const Kind Float64(14);
constexpr const Kind Complex64(15);
constexpr const Kind Complex128(16);
// Array
// Chan
// Func
// Interface
// Map
constexpr const Kind Ptr(22);
// Slice
// String
// Struct
// UnsafePointer
constexpr const Kind ArchFlags(27);

} // namespace onejit

#endif // ONEJIT_KIND_HPP