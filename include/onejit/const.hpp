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
 * const.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_CONST_HPP
#define ONEJIT_CONST_HPP

#include <onejit/node.hpp>

#include <iosfwd>

namespace onejit {

union ConstFloat32 {
public:
  constexpr ConstFloat32() : float_{0.0f} {
  }
  constexpr explicit ConstFloat32(float val) : float_{val} {
  }
  constexpr explicit ConstFloat32(uint32_t bits) : bits_{bits} {
  }

  constexpr float val() const {
    return float_;
  }
  constexpr uint32_t bits() const {
    return bits_;
  }

private:
  float float_;
  uint32_t bits_;
}; // namespace onejit

////////////////////////////////////////////////////////////////////////////////

union ConstFloat64 {
public:
  constexpr ConstFloat64() : float_{0.0} {
  }
  constexpr explicit ConstFloat64(double val) : float_{val} {
  }
  constexpr explicit ConstFloat64(uint64_t bits) : bits_{bits} {
  }

  constexpr double val() const {
    return float_;
  }
  constexpr uint64_t bits() const {
    return bits_;
  }

private:
  double float_;
  uint64_t bits_;
}; // namespace onejit

////////////////////////////////////////////////////////////////////////////////

class Const {
  friend class Func;
  friend class Node;

public:
  constexpr Const() : kind_{Void}, bits_{} {
  }
  constexpr explicit Const(bool val) : kind_{Bool}, bits_{uint64_t(val)} {
  }
  constexpr explicit Const(int8_t val) : kind_{Int8}, bits_{uint64_t(val)} {
  }
  constexpr explicit Const(int16_t val) : kind_{Int16}, bits_{uint64_t(val)} {
  }
  constexpr explicit Const(int32_t val) : kind_{Int32}, bits_{uint64_t(val)} {
  }
  constexpr explicit Const(int64_t val) : kind_{Int64}, bits_{uint64_t(val)} {
  }
  constexpr explicit Const(uint8_t val) : kind_{Uint8}, bits_{uint64_t(val)} {
  }
  constexpr explicit Const(uint16_t val) : kind_{Uint16}, bits_{uint64_t(val)} {
  }
  constexpr explicit Const(uint32_t val) : kind_{Uint32}, bits_{uint64_t(val)} {
  }
  constexpr explicit Const(uint64_t val) : kind_{Uint64}, bits_{uint64_t(val)} {
  }
  constexpr explicit Const(float val) : kind_{Float32}, bits_{ConstFloat32{val}.bits()} {
  }
  constexpr explicit Const(double val) : kind_{Float64}, bits_{ConstFloat64{val}.bits()} {
  }
  constexpr Const(Kind kind, uint64_t bits) : kind_{kind}, bits_{bits} {
  }

  constexpr Type type() const {
    return CONST;
  }

  constexpr Kind kind() const {
    return kind_;
  }

  constexpr uint16_t children() const {
    return 0;
  }

  constexpr explicit operator bool() const {
    return kind_ != Void;
  }

  constexpr int8_t int8() const {
    return int8_t(bits_);
  }
  constexpr int16_t int16() const {
    return int16_t(bits_);
  }
  constexpr int32_t int32() const {
    return int32_t(bits_);
  }
  constexpr int64_t int64() const {
    return int64_t(bits_);
  }

  constexpr uint8_t uint8() const {
    return uint8_t(bits_);
  }
  constexpr uint16_t uint16() const {
    return uint16_t(bits_);
  }
  constexpr uint32_t uint32() const {
    return uint32_t(bits_);
  }
  constexpr uint64_t uint64() const {
    return uint64_t(bits_);
  }

  constexpr float float32() const {
    return ConstFloat32{uint32_t(bits_)}.val();
  }
  constexpr double float64() const {
    return ConstFloat64{bits_}.val();
  }

private:
  constexpr bool is_direct() const {
    // true if highest 41 bits are either 0 or 0x1FFFFFFFFFF
    return (kind_.val() & 0xF0) == 0 && (bits_ < 0x800000 || int64_t(bits_) >= -0x800000);
  }
  // usable only if is_direct() returns true
  constexpr uint32_t direct() const {
    return 0x1 | uint32_t(kind_.val() & 0xF) << 1 | uint32_t(bits_) << 5;
  }
  constexpr static Kind kind_from_direct(uint32_t data) {
    return Kind((data >> 1) & 0xF);
  }
  constexpr static Const from_direct(uint32_t data) {
    // signed right shift is implementation-dependent: use a division
    return Const{Kind((data >> 1) & 0x7F), uint64_t(int32_t(data & ~0xFF) / 256)};
  }

  Kind kind_;
  uint64_t bits_;
};

constexpr bool operator==(Const a, Const b) {
  return a.kind() == b.kind() && a.uint64() == b.uint64();
}

constexpr bool operator!=(Const a, Const b) {
  return a.kind() != b.kind() || a.uint64() != b.uint64();
}

std::ostream &operator<<(std::ostream &out, Const const);

} // namespace onejit

#endif // ONEJIT_CONST_HPP
