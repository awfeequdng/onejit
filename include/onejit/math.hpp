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
 * math.hpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_MATH_HPP
#define ONEJIT_MATH_HPP

#include <cstdint> // uint*_t

namespace onejit {

// truncate to uint16_t with saturation
constexpr inline uint16_t saturate_uint16(uint64_t a) noexcept {
  return uint16_t(a) == a ? uint16_t(a) : uint16_t(-1);
}

// truncate to uint32_t with saturation
constexpr inline uint32_t saturate_uint32(uint64_t a) noexcept {
  return uint32_t(a) == a ? uint32_t(a) : uint32_t(-1);
}

// subtract a - b with saturation to uint16_t
constexpr inline uint16_t sub_uint16(uint64_t a, uint64_t b) noexcept {
  return a >= b ? saturate_uint16(a - b) : 0;
}

// add uint32_t a + b with saturation
constexpr inline uint32_t add_uint32(uint32_t a, uint32_t b) noexcept {
  return saturate_uint32(uint64_t(a) + uint64_t(b));
}

// multiply uint32_t a * b with saturation
constexpr inline uint32_t mul_uint32(uint32_t a, uint32_t b) noexcept {
  return saturate_uint32(uint64_t(a) * uint64_t(b));
}

// subtract a - b with saturation to uint32_t
constexpr inline uint16_t sub_uint32(uint64_t a, uint64_t b) noexcept {
  return a >= b ? saturate_uint32(a - b) : 0;
}

} // namespace onejit

#endif // ONEJIT_MATH_HPP
