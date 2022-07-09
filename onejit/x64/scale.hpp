/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * scale.hpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_SCALE_HPP
#define ONEJIT_X64_SCALE_HPP

#include <onejit/op.hpp>

namespace onejit {
namespace x64 {

enum eScale : uint8_t {
  eScale0 = 0,
  eScale1 = 1,
  eScale2 = 2,
  eScale4 = 3,
  eScale8 = 4, // x86_64 only
};

class Scale {
public:
  constexpr Scale() noexcept : val_{eScale0} {
  }

  constexpr /*implicit*/ Scale(eScale val) noexcept : val_{val} {
  }

  constexpr explicit Scale(uint8_t val) noexcept
      : val_{val <= 2 ? eScale(val) : val == 4 ? eScale4 : val == 8 ? eScale8 : eScale0} {
  }

  static constexpr Scale fromlog2p1(uint8_t log2p1) noexcept {
    return Scale{eScale(log2p1)};
  }

  constexpr explicit operator bool() const noexcept {
    return val_ != eScale0;
  }

  constexpr eScale log2p1() const noexcept {
    return val_;
  }

  constexpr uint8_t val() const noexcept {
    return val_ ? 1 << (val_ - 1) : 0;
  }

  constexpr uint8_t bits() const noexcept {
    return val_ == eScale0 ? 0 : val_ - 1;
  }

  Scale &operator>>=(uint8_t shift) noexcept {
    val_ = eScale(val_ - shift);
    return *this;
  }

  Scale &operator<<=(uint8_t shift) noexcept {
    val_ = eScale(val_ + shift);
    return *this;
  }

  const Chars string() const noexcept;

private:
  eScale val_;
};

inline constexpr bool operator==(Scale a, Scale b) noexcept {
  return a.log2p1() == b.log2p1();
}
inline constexpr bool operator!=(Scale a, Scale b) noexcept {
  return a.log2p1() != b.log2p1();
}
inline constexpr bool operator<(Scale a, Scale b) noexcept {
  return a.log2p1() < b.log2p1();
}
inline constexpr bool operator<=(Scale a, Scale b) noexcept {
  return a.log2p1() <= b.log2p1();
}
inline constexpr bool operator>(Scale a, Scale b) noexcept {
  return a.log2p1() > b.log2p1();
}
inline constexpr bool operator>=(Scale a, Scale b) noexcept {
  return a.log2p1() >= b.log2p1();
}

constexpr const Scale Scale0{eScale0};
constexpr const Scale Scale1{eScale1};
constexpr const Scale Scale2{eScale2};
constexpr const Scale Scale4{eScale4};
constexpr const Scale Scale8{eScale8}; // x86_64 only

const Fmt &operator<<(const Fmt &fmt, eScale escale);
const Fmt &operator<<(const Fmt &fmt, Scale scale);

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_SCALE_HPP
