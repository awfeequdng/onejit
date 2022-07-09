/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * bits.hpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_BITS_HPP
#define ONEJIT_BITS_HPP

#include <onejit/fwd.hpp>

#include <cstddef> // size_t
#include <cstdint> // uint8_t

namespace onejit {

enum eBits : uint8_t {
  eBits0 = 255,
  eBits1 = 0,
  eBits2 = 1,
  eBits4 = 2,
  eBits8 = 3,
  eBits16 = 4,
  eBits32 = 5,
  eBits64 = 6,
  eBits128 = 7,
  eBits256 = 8,
  eBits512 = 9,
};

class Bits {
public:
  constexpr Bits() noexcept : val_{eBits0} {
  }

  constexpr explicit Bits(size_t n) noexcept : val_{eBits(uintlog2(n, -1))} {
  }

  constexpr /*implicit*/ Bits(eBits val) noexcept : val_{val} {
  }

  static constexpr Bits fromlog2(uint8_t log2) noexcept {
    return Bits{eBits(log2)};
  }

  constexpr eBits ebits() const noexcept {
    return val_;
  }

  // 255 means val() == 0
  constexpr uint8_t log2() const noexcept {
    return uint8_t(val_);
  }

  constexpr bool is_zero() const noexcept {
    return val_ == eBits0;
  }

  constexpr size_t val() const noexcept {
    return is_zero() ? 0 : size_t(1) << val_;
  }

  // String string() const;

private:
  static constexpr uint8_t uintlog2(size_t n, uint8_t accum) noexcept {
    return n == 0 ? accum : uintlog2(n >> 1, accum + 1);
  }

  eBits val_;
};

constexpr inline bool operator==(Bits a, Bits b) noexcept {
  return a.log2() == b.log2();
}

constexpr inline bool operator==(Bits a, size_t b) noexcept {
  return a.val() == b;
}

constexpr inline bool operator==(size_t a, Bits b) noexcept {
  return a == b.val();
}

constexpr inline bool operator!=(Bits a, Bits b) noexcept {
  return a.log2() != b.log2();
}

constexpr inline bool operator!=(Bits a, size_t b) noexcept {
  return a.val() != b;
}

constexpr inline bool operator!=(size_t a, Bits b) noexcept {
  return a != b.val();
}

constexpr inline bool operator<(Bits a, Bits b) noexcept {
  // use overflow to convert log2() == 255 to 0
  return uint8_t(1 + a.log2()) < uint8_t(1 + b.log2());
}

constexpr inline bool operator<=(Bits a, Bits b) noexcept {
  return !(b < a);
}

constexpr inline bool operator>(Bits a, Bits b) noexcept {
  return b < a;
}

constexpr inline bool operator>=(Bits a, Bits b) noexcept {
  return !(a < b);
}

constexpr inline Bits operator*(Bits a, Bits b) noexcept {
  return a.is_zero() || b.is_zero() ? Bits() : Bits::fromlog2(a.log2() + b.log2());
}

constexpr inline Bits operator/(Bits a, Bits b) noexcept {
  return a.is_zero() || a < b ? Bits() : Bits::fromlog2(a.log2() - b.log2());
}

using SimdN = Bits;

constexpr const Bits Bits0{};
constexpr const Bits Bits1{eBits1};
constexpr const Bits Bits8{eBits8};
constexpr const Bits Bits16{eBits16};
constexpr const Bits Bits32{eBits32};
constexpr const Bits Bits64{eBits64};
constexpr const Bits Bits128{eBits128};

const Fmt &operator<<(const Fmt &fmt, Bits bits);

} // namespace onejit

#endif // ONEJIT_BITS_HPP
