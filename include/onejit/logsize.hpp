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
 * logsize.hpp
 *
 *  Created on Jan 08, 2021
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
  constexpr LogSize() noexcept : log2_{uint8_t(-1)} {
  }

  constexpr explicit LogSize(size_t n) noexcept : log2_(uintlog2(n, -1)) {
  }

  static constexpr LogSize fromlog2(uint8_t log2) noexcept {
    return LogSize(log2, Log2Tag{});
  }

  // 255 means val() == 0
  constexpr uint8_t log2() const noexcept {
    return log2_;
  }

  constexpr bool is_zero() const noexcept {
    return log2_ == uint8_t(-1);
  }

  constexpr size_t val() const noexcept {
    return is_zero() ? 0 : size_t(1) << log2_;
  }

  // String string() const;

private:
  struct Log2Tag {};
  constexpr LogSize(uint8_t log2, Log2Tag) noexcept : log2_{log2} {
  }

  static constexpr uint8_t uintlog2(size_t n, uint8_t accum) noexcept {
    return n == 0 ? accum : uintlog2(n >> 1, accum + 1);
  }

  uint8_t log2_;
};

constexpr inline bool operator==(LogSize a, LogSize b) noexcept {
  return a.log2() == b.log2();
}

constexpr inline bool operator==(LogSize a, size_t b) noexcept {
  return a.val() == b;
}

constexpr inline bool operator==(size_t a, LogSize b) noexcept {
  return a == b.val();
}

constexpr inline bool operator!=(LogSize a, LogSize b) noexcept {
  return a.log2() != b.log2();
}

constexpr inline bool operator!=(LogSize a, size_t b) noexcept {
  return a.val() != b;
}

constexpr inline bool operator!=(size_t a, LogSize b) noexcept {
  return a != b.val();
}

constexpr inline bool operator<(LogSize a, LogSize b) noexcept {
  // use overflow to convert log2() == 255 to 0
  return uint8_t(1 + a.log2()) < uint8_t(1 + b.log2());
}

constexpr inline bool operator<=(LogSize a, LogSize b) noexcept {
  return !(b < a);
}

constexpr inline bool operator>(LogSize a, LogSize b) noexcept {
  return b < a;
}

constexpr inline bool operator>=(LogSize a, LogSize b) noexcept {
  return !(a < b);
}

constexpr inline LogSize operator*(LogSize a, LogSize b) noexcept {
  return a.is_zero() || b.is_zero() ? LogSize() : LogSize::fromlog2(a.log2() + b.log2());
}

constexpr inline LogSize operator/(LogSize a, LogSize b) noexcept {
  return a.is_zero() || a < b ? LogSize() : LogSize::fromlog2(a.log2() - b.log2());
}

typedef LogSize Bits;
typedef LogSize SimdN;

std::ostream &operator<<(std::ostream &out, LogSize size);

} // namespace onejit

#endif // ONEJIT_SIZE_HPP
