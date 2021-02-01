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
 * rex_byte.hpp
 *
 *  Created on Jan 29, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_REX_HPP
#define ONEJIT_X64_REX_HPP

#include <onejit/x64/reg.hpp>

namespace onejit {
namespace x64 {

constexpr inline uint8_t rlo(Reg reg) noexcept {
  return rlo(reg.reg_id());
}

inline uint8_t rhi(Reg reg) noexcept {
  return rhi(reg.reg_id());
}

uint8_t rex_byte(Bits default_size, Reg base, Reg index = Reg{}) noexcept;

// REX byte if default width is 32
inline uint8_t rex_byte_default32(Reg base, Reg index = Reg{}) noexcept {
  return rex_byte(Bits(32), base, index);
}

// REX byte if default width is 32
inline uint8_t rex_byte_default64(Reg base, Reg index = Reg{}) noexcept {
  return rex_byte(Bits(64), base, index);
}

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_REG_HPP
