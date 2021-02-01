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
#include <onejit/bits.hpp> // Bits
#include <onejit/const.hpp>
#include <onejit/label.hpp>
#include <onejit/stmt1.hpp>
#include <onejit/x64/inst.hpp>
#include <onejit/x64/mem.hpp>
#include <onejit/x64/reg.hpp>
#include <onejit/x64/rex_byte.hpp>

namespace onejit {
namespace x64 {

using onestl::Bytes;

using namespace onejit;

static const Inst1 inst1[] = {
    Inst1{"\0", Arg1(0), B0},                /*                             bad instruction */
    Inst1{"\x0f\xc8", Arg1::Reg, B32 | B64}, /*                                     bswap   */
    Inst1{"\xff\x10", Arg1::Reg | Arg1::Mem | Arg1::Val, B64, B32},              /* call    */
    Inst1{"\0", Arg1::Rax, B16 | B32 | B64},                                     /* cbw     */
    Inst1{"\xfe\x08", Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite}, /* dec     */
    Inst1{"\xfe\x00", Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite}, /* inc     */
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
    Inst1{"\xf6\x18", Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite}, /* neg     */
    Inst1{"\xf6\x10", Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite}, /* not     */
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
    Inst1{"\0", Arg1::Val, B0, B8}, /*                       xabort               */
};

const Inst1 &Inst1::find(OpStmt1 op) noexcept {
  size_t i = 0;
  if (op >= X86_BSWAP && op <= X86_XABORT) {
    i = size_t(op) - X86_BSWAP + 1;
  }
  return inst1[i];
}

static size_t immediate_minbytes(x64::Addr address, Reg base, Reg index) noexcept {
  int32_t offset = address.offset();
  if (offset != int32_t(int8_t(offset)) || address.label() || (index && !base)) {
    return 4;
  }
  return offset != 0 || rlo(base) == 5 ? 1 : 0;
}

static constexpr inline bool is_quirk24(Reg base, Reg index) noexcept {
  return !index && rlo(base) == 4;
}

static size_t inst1_insert_prefixes(uint8_t buf[], size_t len, Kind kind, Bits default_size,
                                    Reg base, Reg index = Reg{}) {
  if (kind.bits() == Bits16) {
    buf[len++] = 0x66;
  }
  if ((buf[len] = rex_byte(default_size, base, index)) != 0) {
    len++;
  }
  return len;
}

static size_t inst1_insert_modrm_sib(uint8_t buf[], size_t len, size_t immediate_bytes, //
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

static size_t inst1_insert_offset(uint8_t buf[], size_t len, size_t immediate_bytes,
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

static ONEJIT_NOINLINE Assembler &inst1_emit_addr(Assembler &dst,           //
                                                  const x64::Addr &address, //
                                                  const uint8_t prefix[2],  //
                                                  Bits default_size) noexcept {
  Reg base{address.base()};
  Reg index{address.index()}; // index cannot be %rsp
  Scale scale = address.scale();
  if (!base && !index) {
    // use RSP as index, it is interpreted as zero
    index = Reg{Uint64, RSP};
    scale = Scale1;
  }
  uint8_t buf[10] = {};
  size_t len = inst1_insert_prefixes(buf, 0, address.kind(), default_size, base, index);
  buf[len++] = prefix[0];
  buf[len] = prefix[1];

  size_t immediate_bytes = immediate_minbytes(address, base, index);
  len = inst1_insert_modrm_sib(buf, len, immediate_bytes, base, index, scale);

  if (immediate_bytes != 0) {
    len = inst1_insert_offset(buf, len, immediate_bytes, address.offset());
  }
  dst.add(onestl::Bytes{buf, len});
  if (auto label = address.label()) {
    dst.add_relocation(label);
  }
  return dst;
}

static constexpr Bits inst1_default_size(OpStmt1 op) noexcept {
  return (op == X86_CALL || op == X86_JMP || op == X86_POP || op == X86_PUSH) ? Bits64 : Bits32;
}

ONEJIT_NOINLINE Assembler &Inst1::emit_mem(Assembler &dst, OpStmt1 op, x64::Mem mem) const
    noexcept {
  uint8_t prefix[2] = {bytes_[0], bytes_[1]};
  if ((arg_size_ & B8) != 0 && mem.kind().bits() != Bits8) {
    // instruction also supports 8-bit memory access, but requested access is wider.
    prefix[1] |= 1;
  }
  Bits default_size = inst1_default_size(op);

  // tested for X86_CALL, X86_DEC, X86_INC, X86_NEG, X86_NOT
  return inst1_emit_addr(dst, mem.address(), prefix, default_size);
}

ONEJIT_NOINLINE Assembler &Inst1::emit_imm(Assembler &dst, OpStmt1 op, Imm imm) const noexcept {
  /// TODO: implement
  (void)op;
  (void)imm;
  return dst;
}

ONEJIT_NOINLINE Assembler &Inst1::emit_label(Assembler &dst, OpStmt1 op, Label l) const noexcept {
  /// TODO: implement
  (void)l;
  (void)op;
  return dst;
}

ONEJIT_NOINLINE Assembler &Inst1::emit_reg(Assembler &dst, OpStmt1 op, Reg reg) const noexcept {
  Bits default_size = inst1_default_size(op);
  uint8_t buf[4] = {};
  size_t len = inst1_insert_prefixes(buf, 0, reg.kind(), default_size, reg);

  buf[len] = bytes_[0];
  buf[len + 1] = bytes_[1] | 0xc0 | rlo(reg);
  if ((arg_size_ & B8) != 0 && reg.kind().bits() != Bits8) {
    // instruction also supports 8-bit memory access, but requested access is wider.
    buf[len] |= 1;
  }
  return dst.add(onestl::Bytes{buf, len + 2});
}

Assembler &Inst1::emit(Assembler &dst, Stmt1 st) const noexcept {
  Node arg = st.child(0);
  switch (arg.type()) {
  case VAR:
    if (Var v = arg.is<Var>()) {
      return emit_reg(dst, st.op(), Reg{v.local()});
    }
    break;
  case MEM:
    if (Mem mem = arg.is<Mem>()) {
      return emit_mem(dst, st.op(), mem);
    }
    break;
  case LABEL:
    if (Label l = arg.is<Label>()) {
      return emit_label(dst, st.op(), l);
    }
    break;
  case CONST:
    if (Const c = arg.is<Const>()) {
      return emit_imm(dst, st.op(), c.imm());
    }
    break;
  default:
    break;
  }
  return dst.error(
      arg, "unexpected node type in x64::Inst1::emit, expecting Const, Label, Var or x64::Mem");
}

} // namespace x64
} // namespace onejit
