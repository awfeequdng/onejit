/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * util.cpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/assembler.hpp>
#include <onejit/x64/inst.hpp>
#include <onejit/x64/mem.hpp>
#include <onejit/x64/reg.hpp>
#include <onejit/x64/scale.hpp>
#include <onejit/x64/util.hpp>

namespace onejit {
namespace x64 {

size_t Util::get_offset_minbytes(Mem mem, Reg base, Reg index) noexcept {
  int32_t offset = mem.offset();
  if (offset != int32_t(int8_t(offset)) || mem.label() || base.reg_id() == RIP ||
      (index && !base)) {
    return 4;
  }
  return offset != 0 || rlo(base) == 5 ? 1 : 0;
}

static constexpr inline bool is_quirk24(Reg base, Reg index) noexcept {
  return !index && rlo(base) == 4;
}

size_t Util::insert_modrm_sib(uint8_t buf[], size_t len, size_t immediate_bytes, //
                              Reg base, Reg index, Scale scale) {
  if (!base && index) {
    // nothing to do
  } else if (immediate_bytes == 1) {
    buf[len] |= 0x40;
  } else if (immediate_bytes == 4 && base.reg_id() != RIP) {
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

size_t Util::insert_offset_or_imm(uint8_t buf[], size_t len, size_t immediate_bytes,
                                  int32_t offset) {
  if (immediate_bytes == 1) {
    buf[len++] = uint8_t(offset);
  } else if (immediate_bytes == 2) {
    uint16_t offset_u = uint16_t(offset);
    buf[len++] = offset_u;
    buf[len++] = offset_u >> 8;
  } else if (immediate_bytes == 4) {
    uint32_t offset_u = uint32_t(offset);
    buf[len++] = offset_u;
    buf[len++] = offset_u >> 8;
    buf[len++] = offset_u >> 16;
    buf[len++] = offset_u >> 24;
  }
  return len;
}

bool Util::validate_mem(Assembler &dst, Mem mem) {
  Reg base{mem.base()};
  Reg index{mem.index()};
  Scale scale = mem.scale();
  bool ok = true;
  if (base) {
    RegId id = base.reg_id();
    if ((id < RAX || id > R15) && id != RIP) {
      dst.error(mem, "x64::Asm memory reference: invalid base register");
      ok = false;
    }
  }
  if (index && scale != eScale0) {
    RegId id = index.reg_id();
    if ((id < RAX || id > R15) && id != RIP) {
      dst.error(mem, "x64::Asm memory reference: invalid index register");
      ok = false;
    } else if (id == RSP) {
      dst.error(mem, "x64::Asm memory reference: cannot use %rsp as index register");
      ok = false;
    } else if (id == RIP) {
      dst.error(mem, "x64::Asm memory reference: cannot use %rip as index register");
      ok = false;
    } else if (base.reg_id() == RIP) {
      dst.error(
          mem, "x64::Asm memory reference: %rip-relative addressing does not allow index register");
      ok = false;
    }
  }
  return ok;
}

} // namespace x64
} // namespace onejit
