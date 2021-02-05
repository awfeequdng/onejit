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
 * local.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/fmt.hpp>
#include <onejit/local.hpp>
#include <onejit/x86/regid.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// ----------------------------------- noarch -----------------------------------

static const Fmt &format_reg_noarch(const Fmt &out, Id id, Kind kind) {
  return out << "var" << Hex(id.val()) //
             << '_' << kind.stringsuffix();
}

// ----------------------------------- x86 -------------------------------------

static const char string_reg_x86[4 * 4 * 16] =         //
    "al\0\0cl\0\0dl\0\0bl\0\0spl\0bpl\0sil\0dil\0"     //
    "r8b\0r9b\0r10br11br12br13br14br15b"               //
    "ax\0\0cx\0\0dx\0\0bx\0\0sp\0\0bp\0\0si\0\0di\0\0" //
    "r8w\0r9w\0r10wr11wr12wr13wr14wr15w"               //
    "eax\0ecx\0edx\0ebx\0esp\0ebp\0esi\0edi\0"         //
    "r8l\0r9l\0r10lr11lr12lr13lr14lr15l"               //
    "rax\0rcx\0rdx\0rbx\0rsp\0rbp\0rsi\0rdi\0"         //
    "r8\0\0r9\0\0r10\0r11\0r12\0r13\0r14\0r15";

static const Fmt &format_reg_x86(const Fmt &out, Id id, Kind kind) {
  eBits ebits = kind.bits().ebits();
  uint32_t x = id.val();
  if (x >= x86::RAX && x <= x86::R15) {
    if (ebits < eBits8 || ebits > eBits64) {
      ebits = eBits64;
    }
    const char *addr = string_reg_x86 + (id.val() & 0xF) * 4 + (ebits - eBits8) * 64;
    size_t len = addr[3] ? 4 : addr[2] ? 3 : 2;
    return out << Chars{addr, len};

  } else if (x == x86::RIP) {
    return out << Chars{ebits <= eBits32 ? "eip" : "rip", 3};

  } else if (x >= x86::XMM0 && x <= x86::XMM31) {
    return out << Chars{ebits <= eBits128 ? "xmm" : ebits == eBits256 ? "ymm" : "zmm", 3}
               << (x - x86::XMM0);
  } else {
    return format_reg_noarch(out, id, kind);
  }
}

// -----------------------------------------------------------------------------

const Fmt &operator<<(const Fmt &out, Id id) {
  return out << "id" << Hex(id.val());
}

const Fmt &operator<<(const Fmt &out, Local local) {
  Id id = local.id();
  Kind kind = local.kind();
  if (id.val() >= x86::RAX && id.val() <= x86::XMM31) {
    return format_reg_x86(out, id, kind);
  } else {
    return format_reg_noarch(out, id, kind);
  }
}

} // namespace onejit
