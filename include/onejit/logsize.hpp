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
 * bits.hpp
 *
 *  Created on Jan 08, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_SIZE_HPP
#define ONEJIT_SIZE_HPP

#include <cstddef> // size_t
#include <cstdint> // uint8_t
#include <iosfwd>  // std::ostream

namespace onejit {

class LogSize {
public:
  constexpr LogSize() : log2_(-1) {
  }

  constexpr explicit LogSize(size_t n) : log2_(uintlog2(n, -1)) {
  }

  static constexpr LogSize fromlog2(uint8_t log2) {
    return LogSize(log2, Log2Tag{});
  }

  // 255 means val() == 0
  constexpr uint8_t log2() const {
    return log2_;
  }

  constexpr bool is_zero() const {
    return log2_ == uint8_t(-1);
  }

  constexpr size_t val() const {
    return is_zero() ? 0 : size_t(1) << log2_;
  }

  // String string() const;

private:
  struct Log2Tag {};
  constexpr LogSize(uint8_t log2, Log2Tag) : log2_(log2) {
  }

  constexpr static uint8_t uintlog2(size_t n, uint8_t accum) {
    return n == 0 ? accum : uintlog2(n >> 1, accum + 1);
  }

  uint8_t log2_;
};

constexpr inline bool operator==(LogSize a, LogSize b) {
  return a.log2() == b.log2();
}

constexpr inline bool operator==(LogSize a, size_t b) {
  return a.val() == b;
}

constexpr inline bool operator==(size_t a, LogSize b) {
  return a == b.val();
}

constexpr inline bool operator!=(LogSize a, LogSize b) {
  return a.log2() != b.log2();
}

constexpr inline bool operator!=(LogSize a, size_t b) {
  return a.val() != b;
}

constexpr inline bool operator!=(size_t a, LogSize b) {
  return a != b.val();
}

constexpr inline bool operator<(LogSize a, LogSize b) {
  // use overflow to convert log2() == 255 to 0
  return uint8_t(1 + a.log2()) < uint8_t(1 + b.log2());
}

constexpr inline LogSize operator*(LogSize a, LogSize b) {
  return a.is_zero() || b.is_zero() ? LogSize() : LogSize::fromlog2(a.log2() + b.log2());
}

constexpr inline LogSize operator/(LogSize a, LogSize b) {
  return a.is_zero() || a < b ? LogSize() : LogSize::fromlog2(a.log2() - b.log2());
}

typedef LogSize Bits;
typedef LogSize SimdN;

std::ostream &operator<<(std::ostream &out, LogSize size);

} // namespace onejit

#endif // ONEJIT_SIZE_HPP
