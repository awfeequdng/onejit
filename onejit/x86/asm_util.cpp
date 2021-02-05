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
 * inst1_x64.cpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/bits.hpp> // Bits
#include <onejit/x86/asm_util.hpp>
#include <onejit/x86/inst.hpp>
#include <onejit/x86/mem.hpp>
#include <onejit/x86/reg.hpp>
#include <onejit/x86/scale.hpp>

namespace onejit {
namespace x86 {

size_t AsmUtil::get_offset_minbytes(Mem mem, Reg base, Reg index) noexcept {
  int32_t offset = mem.offset();
  if (offset != int32_t(int8_t(offset)) || mem.label() || (index && !base)) {
    return 4;
  }
  return offset != 0 || rlo(base) == 5 ? 1 : 0;
}

static constexpr inline bool is_quirk24(Reg base, Reg index) noexcept {
  return !index && rlo(base) == 4;
}

size_t AsmUtil::insert_modrm_sib(uint8_t buf[], size_t len, size_t immediate_bytes, //
                                 Reg base, Reg index, Scale scale) {
  if (!base && index) {
    // nothing to do
  } else if (immediate_bytes == 1) {
    buf[len] |= 0x40;
  } else if (immediate_bytes == 4) {
    buf[len] |= 0x80;
  }
  if (index) {
    buf[len++] |= 0x4;
    buf[len++] = (base ? rlo(base) : 0x5) | rlo(index) << 3 | scale.bits() << 6;
  } else if (base) {
    buf[len++] |= rlo(base);
  }
  if (is_quirk24(base, index)) {
    buf[len++] = 0x24;
  }
  return len;
}

size_t AsmUtil::insert_offset_or_imm(uint8_t buf[], size_t len, size_t immediate_bytes,
                                     int32_t offset) {
  if (immediate_bytes == 1) {
    buf[len++] = uint8_t(offset);
  } else if (immediate_bytes == 4) {
    uint32_t offset_u = uint32_t(offset);
    buf[len++] = offset_u;
    buf[len++] = offset_u >> 8;
    buf[len++] = offset_u >> 16;
    buf[len++] = offset_u >> 24;
  }
  return len;
}

} // namespace x86
} // namespace onejit
