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
 * rex.hpp
 *
 *  Created on Jan 29, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X86_REX_HPP
#define ONEJIT_X86_REX_HPP

#include <onejit/local.hpp>
#include <onejit/x86/reg.hpp>

namespace onejit {
namespace x86 {

constexpr inline Reg to_reg(Local l) noexcept {
  return Reg(l.id().val());
}

constexpr inline uint8_t rlo(Local l) noexcept {
  return rlo(to_reg(l));
}

inline uint8_t rhi(Local l) noexcept {
  return rhi(to_reg(l));
}

// REX byte if default width is 32
inline uint8_t rex_byte_32_64(Local l) noexcept {
  uint8_t byte = rhi(l);
  if (byte || l.kind().bits().val() >= 64) {
    byte |= 0x40;
  }
  return byte;
}

// REX byte if default width is 64
inline uint8_t rex_byte_64(Local l) noexcept {
  uint8_t byte = rhi(l);
  if (byte) {
    byte |= 0x40;
  }
  return byte;
}

} // namespace x86
} // namespace onejit

#endif // ONEJIT_X86_REG_HPP
