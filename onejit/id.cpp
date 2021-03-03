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
 * id.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/archid.hpp>
#include <onejit/arm64/regid.hpp>
#include <onejit/fmt.hpp>
#include <onejit/id.hpp>
#include <onejit/x64/regid.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// ----------------------------------- noarch -----------------------------------

static const Fmt &format_reg_noarch(const Fmt &fmt, Id id, Kind kind) {
  return fmt << "var" << Hex(id.val()) //
             << '_' << kind.stringsuffix();
}

// ----------------------------------- x64 -------------------------------------

static const char string_reg_x64[4 * 4 * 16] =         //
    "al\0\0cl\0\0dl\0\0bl\0\0spl\0bpl\0sil\0dil\0"     //
    "r8b\0r9b\0r10br11br12br13br14br15b"               //
    "ax\0\0cx\0\0dx\0\0bx\0\0sp\0\0bp\0\0si\0\0di\0\0" //
    "r8w\0r9w\0r10wr11wr12wr13wr14wr15w"               //
    "eax\0ecx\0edx\0ebx\0esp\0ebp\0esi\0edi\0"         //
    "r8d\0r9d\0r10dr11dr12dr13dr14dr15d"               //
    "rax\0rcx\0rdx\0rbx\0rsp\0rbp\0rsi\0rdi\0"         //
    "r8\0\0r9\0\0r10\0r11\0r12\0r13\0r14\0r15";

static const Fmt &format_reg_x64(const Fmt &fmt, Id id, Kind kind) {
  eBits ebits = kind.bits().ebits();
  uint32_t x = id.val();
  if (x >= x64::RAX && x <= x64::R15) {
    if (ebits < eBits8 || ebits > eBits64) {
      ebits = eBits64;
    }
    const char *addr = string_reg_x64 + (id.val() & 0xF) * 4 + (ebits - eBits8) * 64;
    size_t len = addr[3] ? 4 : addr[2] ? 3 : 2;
    return fmt << Chars{addr, len};

  } else if (x == x64::RIP) {
    return fmt << Chars{ebits <= eBits32 ? "eip" : "rip", 3};

  } else if (x >= x64::XMM0 && x <= x64::XMM31) {
    return fmt << Chars{ebits <= eBits128 ? "xmm" : ebits == eBits256 ? "ymm" : "zmm", 3}
               << (x - x64::XMM0);
  } else {
    return format_reg_noarch(fmt, id, kind);
  }
}

// ----------------------------------- arm64 -----------------------------------

static const Fmt &format_reg_arm64(const Fmt &fmt, Id id, Kind kind) {
  eBits ebits = kind.bits().ebits();
  uint32_t x = id.val();
  if (x >= arm64::X0 && x <= arm64::X31) {
    return fmt << ((ebits <= eBits32) ? 'w' : 'x') << (x - arm64::X0);
  } else if (x >= arm64::V0 && x <= arm64::V31) {
    size_t i = (ebits <= eBits8) ? 0 : (ebits >= eBits128) ? 4 : ebits - eBits8;
    return fmt << "bhsdq"[i] << (x - arm64::V0);
  } else {
    return format_reg_noarch(fmt, id, kind);
  }
}

// -----------------------------------------------------------------------------

const Fmt &Id::format(const Fmt &fmt, Id id, Kind kind) {
  switch (ArchId(id.val() >> 8)) {
  case ArchId::X64:
    return format_reg_x64(fmt, id, kind);
  case ArchId::ARM64:
    return format_reg_arm64(fmt, id, kind);
  default:
    return format_reg_noarch(fmt, id, kind);
  }
}

} // namespace onejit
