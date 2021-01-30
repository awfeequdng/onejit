/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
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

class Scale {
  struct log2tag {};

public:
  constexpr Scale() noexcept : log2p1_{0} {
  }

  constexpr explicit Scale(uint8_t val) noexcept
      : log2p1_{val <= 2 ? val : val == 4 ? uint8_t(3) : val == 8 ? uint8_t(4) : uint8_t(0)} {
  }

  static constexpr Scale fromlog2p1(uint8_t log2p1) noexcept {
    return Scale{log2p1, log2tag{}};
  }

  constexpr operator bool() const noexcept {
    return log2p1_ != 0;
  }

  // log2(val) + 1
  constexpr uint8_t log2p1() const noexcept {
    return log2p1_;
  }

  constexpr uint8_t val() const noexcept {
    return log2p1_ ? 1 << (log2p1_ - 1) : 0;
  }

  constexpr uint8_t bits() const noexcept {
    return log2p1_ == 0 ? 0 : log2p1_ - 1;
  }

  constexpr OpN opn() const noexcept {
    return OpN(X64_ADDR + log2p1_);
  }

  Scale &operator>>=(uint8_t shift) noexcept {
    log2p1_ -= shift;
    return *this;
  }

  Scale &operator<<=(uint8_t shift) noexcept {
    log2p1_ += shift;
    return *this;
  }

private:
  constexpr Scale(uint8_t log2p1, log2tag) noexcept //
      : log2p1_{log2p1} {
  }

  uint8_t log2p1_;
};

constexpr const Scale Scale0{0};
constexpr const Scale Scale1{1};
constexpr const Scale Scale2{2};
constexpr const Scale Scale4{4};
constexpr const Scale Scale8{8};

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_SCALE_HPP
