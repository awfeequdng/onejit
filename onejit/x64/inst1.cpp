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
 * inst1.cpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/assembler.hpp>
#include <onejit/logsize.hpp> // Bits
#include <onejit/opstmt.hpp>
#include <onejit/x64/inst.hpp>
#include <onejit/x64/mem.hpp>
#include <onejit/x64/reg.hpp>
#include <onejit/x64/rex.hpp>

namespace onejit {
namespace x64 {

using onestl::Bytes;

using namespace onejit;

static const Inst1 inst1[] = {
    Inst1{"\0", Arg1(0), B0},          /*                                   bad instruction */
    Inst1{"\0", Arg1::Reg, B32 | B64}, /*                                           bswap   */
    Inst1{"\xff\x10", Arg1::Reg | Arg1::Mem | Arg1::Val, B64, B32},              /* call    */
    Inst1{"\0", Arg1::Rax, B16 | B32 | B64},                                     /* cbw     */
    Inst1{"\xff\x08", Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite}, /* dec     */
    Inst1{"\xff\x00", Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite}, /* inc     */
    Inst1{"\0", Arg1::Val, B0, B8},                                              /* int     */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* ja      */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* jae     */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* jb      */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* jbe     */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* je      */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* jge     */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* jl      */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* jle     */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* jne     */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* jno     */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* jnp     */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* jns     */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* jo      */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* jp      */
    Inst1{"\0", Arg1::Val, B0, B8 | B32, EFread},                                /* js      */
    Inst1{"\0", Arg1::Reg | Arg1::Mem | Arg1::Val, B64, B8 | B32},               /* jmp     */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite},       /* neg     */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite},       /* not     */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B16 | B64},      /* B32 on 32bit       pop     */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B16 | B64},      /* B32 on 32bit       push    */
    Inst1{"\0", Arg1::Rax, B128},                       /* B64 on 32bit       rdtsc   */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       seta                 */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setae                */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setb                 */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setbe                */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       sete                 */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setg                 */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setge                */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setl                 */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setle                */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setne                */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setno                */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setnp                */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setns                */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       seto                 */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       setp                 */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       sets                 */
    Inst1{"\0", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*       sets                 */
    ONEJIT_COMMENT()            /* [CPUID CLFSH] is required by the following instructions */
    Inst1{"\0", Arg1::Mem, B8}, /*                           clflush              */
    ONEJIT_COMMENT()            /* [CPUID CLFLUSHOPT] is required by the following instructions */
    Inst1{"\0", Arg1::Mem, B8}, /*                           clflushopt           */
    ONEJIT_COMMENT()            /* [CPUID CLWB] is required by the following instructions */
    Inst1{"\0", Arg1::Mem, B8}, /*                           clwb                 */
    ONEJIT_COMMENT()            /* [CPUID RTM] is required by the following instructions */
    Inst1{"\0", Arg1::Val, B0, B8}, /*                           xabort               */
};

const Inst1 &Inst1::find(OpStmt1 op) noexcept {
  size_t i = 0;
  if (op >= X86_BSWAP && op <= X86_XABORT) {
    i = size_t(op) - X86_BSWAP + 1;
  }
  return inst1[i];
}

ONEJIT_NOINLINE Assembler &emit_bswap(Assembler &dst, Local l) noexcept {
  uint8_t buf[3] = {rex_byte(l), 0x0f, uint8_t(0xc8 | rlo(l))};

  onestl::Bytes bytes{buf, 3};
  if (buf[0] == 0) {
    bytes = Bytes{buf + 1, 2};
  }
  return dst.add(bytes);
}

ONEJIT_NOINLINE Assembler &emit_call(Assembler &dst, Local l) noexcept {
  uint8_t buf[3] = {rex_byte_default64(l), 0xff, uint8_t(0xd0 | rlo(l))};

  onestl::Bytes bytes{buf, 3};
  if (buf[0] == 0) {
    bytes = Bytes{buf + 1, 2};
  }
  return dst.add(bytes);
}

constexpr inline size_t immediate_minbytes(Local base) noexcept {
  return rlo(base) == 5 ? 1 : 0;
}

size_t immediate_minbytes(x64::Addr address, Local base, Local index) noexcept {
  int32_t offset = address.offset();
  if (offset != int32_t(int8_t(offset)) || address.label() || (index && !base)) {
    return 4;
  }
  return offset != 0 || rlo(base) == 5 ? 1 : 0;
}

constexpr inline bool is_quirk24(Local base, Local index) noexcept {
  return !index && rlo(base) == 4;
}

ONEJIT_NOINLINE Assembler &emit_inst1_addr(Assembler &dst,          //
                                           x64::Addr address,       //
                                           const uint8_t prefix[2], //
                                           Bits default_size) noexcept {
  Local base = address.base();
  Local index = address.index(); // index cannot be %rsp
  Scale scale = address.scale();
  if (!base && !index) {
    // use RSP as index, it is interpreted as zero
    index = Reg{Uint64, RSP};
    scale = Scale1;
  }
  uint8_t buf[10] = {};
  uint8_t *addr = buf;
  size_t immediate_bytes = immediate_minbytes(address, base, index);
  size_t len = 0;
  uint8_t rexbyte;

  if (address.kind().bits() == Bits(16)) {
    addr[len++] = 0x66;
  }

  if (default_size == Bits(64)) {
    addr[len] = rexbyte = rex_byte_default64(base, index);
  } else {
    addr[len] = rexbyte = rex_byte(base, index);
  }
  if (rexbyte != 0) {
    len++;
  }
  addr[len++] = prefix[0];
  addr[len] = prefix[1];
  if (!base && index) {
    // nothing to do
  } else if (immediate_bytes == 1) {
    addr[len] |= 0x40;
  } else if (immediate_bytes == 4) {
    addr[len] |= 0x80;
  }
  if (index) {
    addr[len++] |= 0x4;
    addr[len++] = (base ? rlo(base) : 0x5) | rlo(index) << 3 | scale.bits() << 6;
  } else if (base) {
    addr[len++] |= rlo(base);
  }

  if (is_quirk24(base, index)) {
    addr[len++] = 0x24;
  }
  if (immediate_bytes == 1) {
    addr[len++] = uint8_t(address.offset());
  } else if (immediate_bytes == 4) {
    uint32_t offset = uint32_t(address.offset());
    addr[len++] = offset;
    addr[len++] = offset >> 8;
    addr[len++] = offset >> 16;
    addr[len++] = offset >> 24;
  }
  dst.add(onestl::Bytes{addr, len});
  if (auto label = address.label()) {
    dst.add_label(label);
  }
  return dst;
}

ONEJIT_NOINLINE Assembler &emit_call(Assembler &dst, x64::Addr address) noexcept {
  const uint8_t *bytes = Inst1::find(X86_CALL).bytes();
  return emit_inst1_addr(dst, address, bytes, Bits(64));
}

ONEJIT_NOINLINE Assembler &emit_dec(Assembler &dst, x64::Addr address) noexcept {
  const uint8_t *bytes = Inst1::find(X86_DEC).bytes();
  uint8_t prefix[2] = {bytes[0], bytes[1]};
  prefix[1] |= (address.kind().bits() != Bits(8));
  return emit_inst1_addr(dst, address, prefix, Bits(32));
}

ONEJIT_NOINLINE Assembler &emit_inc(Assembler &dst, x64::Addr address) noexcept {
  const uint8_t *bytes = Inst1::find(X86_INC).bytes();
  uint8_t prefix[2] = {bytes[0], bytes[1]};
  prefix[1] |= (address.kind().bits() != Bits(8));
  return emit_inst1_addr(dst, address, prefix, Bits(32));
}

Assembler &Inst1::emit(Assembler &dst, Node arg) const noexcept {
  if (Var var = arg.is<Var>()) {
    return emit_call(dst, var.local());
  } else if (Mem mem = arg.is<Mem>()) {
    return emit_call(dst, mem.addr());
  } else {
    return dst;
  }
}

} // namespace x64
} // namespace onejit
