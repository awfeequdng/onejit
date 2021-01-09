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
 * size.hpp
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

class Size {
public:
  constexpr Size() : log2_(-1) {
  }

  constexpr explicit Size(size_t n) : log2_(uintlog2(n, -1)) {
  }

  // 255 means Val() == 0
  constexpr uint8_t log2() const {
    return log2_;
  }
  constexpr size_t val() const {
    return log2_ == uint8_t(-1) ? 0 : size_t(1) << log2_;
  }
  // std::string string() const;

private:
  constexpr static uint8_t uintlog2(size_t n, uint8_t accum) {
    return n == 0 ? accum : uintlog2(n >> 1, accum + 1);
  }

  uint8_t log2_;
};

std::ostream &operator<<(std::ostream &out, Size size);

} // namespace onejit

#endif // ONEJIT_SIZE_HPP
