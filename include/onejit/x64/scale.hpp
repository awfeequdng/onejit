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

#include <cstdint> // uint*_t

namespace onejit {
namespace x64 {

enum class Scale : uint8_t {
  None = 0,
  S0 = 0,
  S1 = 1,
  S2 = 2,
  S4 = 4,
  S8 = 8,
};

constexpr inline uint8_t scale_bits(Scale scale) noexcept {
  return scale == Scale::S2 ? 1 : scale == Scale::S4 ? 2 : scale == Scale::S8 ? 3 : 0;
}

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_SCALE_HPP
