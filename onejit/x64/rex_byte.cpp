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
 * rex_byte.cpp
 *
 *  Created on Feb 01, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/x64/rex_byte.hpp>

namespace onejit {
namespace x64 {

uint8_t rex_byte(Bits default_size, Reg base, Reg index) noexcept {
  uint8_t byte = rhi(base) | rhi(index) << 1;
  if (default_size < Bits64 && (base.kind().bits() >= Bits64 || //
                                index.kind().bits() >= Bits64)) {
    // REX byte is needed to use 64-bit registers when default size is 32 bits
    byte |= 0x48;
  } else if (byte || (base && !index                 //
                      && base.kind().bits() == Bits8 //
                      && base.reg_id() >= RSP)) {
    // and also to use the 8-bit registers %spl %bpl %sil %dil
    byte |= 0x40;
  }
  return byte;
}

} // namespace x64
} // namespace onejit
