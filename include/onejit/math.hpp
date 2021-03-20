/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
