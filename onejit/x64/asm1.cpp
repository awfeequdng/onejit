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
 * asm1.cpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/assembler.hpp>
#include <onejit/bits.hpp> // Bits
#include <onejit/const.hpp>
#include <onejit/label.hpp>
#include <onejit/stmt1.hpp>
#include <onejit/x64/asm.hpp>
#include <onejit/x64/inst.hpp>
#include <onejit/x64/mem.hpp>
#include <onejit/x64/reg.hpp>
#include <onejit/x64/rex_byte.hpp>
#include <onejit/x64/util.hpp>

namespace onejit {
namespace x64 {

using onestl::Bytes;

using namespace onejit;

static const Inst1 inst1_vec[] = {
    /*     reg/mem       imm8       imm32                                         */ /*-------- */
    Inst1{"\x0f\x0b", "\x0f\x0b", "\x0f\x0b", Arg1::None, B0}, /*               bad instruction */
    Inst1{"\x0f\xc8", "", "", Arg1::Reg, B32 | B64},           /*                       bswap   */
    Inst1{"\xff\x10", "", "\xe8", Arg1::Reg | Arg1::Mem | Arg1::Val, B64, B32}, /*      call    */
    /* TODO */ Inst1{"", "", "", Arg1::Rax, B16 | B32 | B64},                   /*      cbw     */
    Inst1{"\xfe\x08", "", "", Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite}, /* dec */
    Inst1{"\xfe\x00", "", "", Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite}, /* inc */
    Inst1{"", "\xcd", "", Arg1::Val, B0, B8},                                        /* int     */
    /* ---------------------------------------------------------------------------*/ /*-------- */
    /*          imm8      imm32                                                   */ /*-------- */
    Inst1{"", "\x77", "\x0f\x87", Arg1::Val, B0, B8 | B32, EFread},                  /* ja      */
    Inst1{"", "\x73", "\x0f\x83", Arg1::Val, B0, B8 | B32, EFread},                  /* jae     */
    Inst1{"", "\x72", "\x0f\x82", Arg1::Val, B0, B8 | B32, EFread},                  /* jb      */
    Inst1{"", "\x76", "\x0f\x86", Arg1::Val, B0, B8 | B32, EFread},                  /* jbe     */
    Inst1{"", "\x74", "\x0f\x84", Arg1::Val, B0, B8 | B32, EFread},                  /* je      */
    Inst1{"", "\x7f", "\x0f\x8f", Arg1::Val, B0, B8 | B32, EFread},                  /* jg      */
    Inst1{"", "\x7d", "\x0f\x8d", Arg1::Val, B0, B8 | B32, EFread},                  /* jge     */
    Inst1{"", "\x7c", "\x0f\x8c", Arg1::Val, B0, B8 | B32, EFread},                  /* jl      */
    Inst1{"", "\x7e", "\x0f\x8e", Arg1::Val, B0, B8 | B32, EFread},                  /* jle     */
    Inst1{"", "\x75", "\x0f\x85", Arg1::Val, B0, B8 | B32, EFread},                  /* jne     */
    Inst1{"", "\x71", "\x0f\x81", Arg1::Val, B0, B8 | B32, EFread},                  /* jno     */
    Inst1{"", "\x7b", "\x0f\x8b", Arg1::Val, B0, B8 | B32, EFread},                  /* jnp     */
    Inst1{"", "\x79", "\x0f\x89", Arg1::Val, B0, B8 | B32, EFread},                  /* jns     */
    Inst1{"", "\x70", "\x0f\x80", Arg1::Val, B0, B8 | B32, EFread},                  /* jo      */
    Inst1{"", "\x7a", "\x0f\x8a", Arg1::Val, B0, B8 | B32, EFread},                  /* jp      */
    Inst1{"", "\x78", "\x0f\x88", Arg1::Val, B0, B8 | B32, EFread},                  /* js      */
    /* ---------------------------------------------------------------------------*/ /*-------- */
    /*     reg/mem     imm8    imm32                                              */ /*-------- */
    Inst1{"\xff\x20", "\xeb", "\xe9", Arg1::Reg | Arg1::Mem | Arg1::Val, B64, B8 | B32}, /* jmp */
    Inst1{"\xf6\x18", "", "", Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite}, /* neg */
    Inst1{"\xf6\x10", "", "", Arg1::Reg | Arg1::Mem, B8 | B16 | B32 | B64, B0, EFwrite}, /* not */
    /* ---------------------------------------------------------------------------*/ /*-------- */
    Inst1{"\x8f\x00", "", "", Arg1::Reg | Arg1::Mem, B16 | B64},                     /* pop     */
    /*        mem      imm8   imm32                                              */  /* push    */
    Inst1{"\xff\x30", "\x6a", "\x68", Arg1::Reg | Arg1::Mem | Arg1::Val, B16 | B64, B8 | B32},
    Inst1{"\x0f\x31", "", "", Arg1::Rax, B128}, /* B64 on 32bit                         rdtsc   */
    /*      reg/mem                                                               */ /*-------- */
    Inst1{"\x0f\x97", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            seta    */
    Inst1{"\x0f\x93", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            setae   */
    Inst1{"\x0f\x92", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            setb    */
    Inst1{"\x0f\x96", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            setbe   */
    Inst1{"\x0f\x94", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            sete    */
    Inst1{"\x0f\x9f", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            setg    */
    Inst1{"\x0f\x9d", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            setge   */
    Inst1{"\x0f\x9c", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            setl    */
    Inst1{"\x0f\x9e", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            setle   */
    Inst1{"\x0f\x95", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            setne   */
    Inst1{"\x0f\x91", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            setno   */
    Inst1{"\x0f\x9b", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            setnp   */
    Inst1{"\x0f\x99", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            setns   */
    Inst1{"\x0f\x90", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            seto    */
    Inst1{"\x0f\x9a", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*            setp    */
    Inst1{"\x0f\x98", "", "", Arg1::Reg | Arg1::Mem, B8, B0, EFread}, /*                sets    */
    ONEJIT_COMMENT()                  /* [CPUID CLFSH] is required by the following instructions */
    Inst1{"", "", "", Arg1::Mem, B8}, /*                           clflush              */
    ONEJIT_COMMENT() /* [CPUID CLFLUSHOPT] is required by the following instructions */
    Inst1{"", "", "", Arg1::Mem, B8}, /*                           clflushopt           */
    ONEJIT_COMMENT()                  /* [CPUID CLWB] is required by the following instructions */
    Inst1{"", "", "", Arg1::Mem, B8}, /*                           clwb                 */
    ONEJIT_COMMENT()                  /* [CPUID RTM] is required by the following instructions */
    Inst1{"", "", "", Arg1::Val, B0, B8}, /*                       xabort               */
};

const Inst1 &Asm1::find(OpStmt1 op) noexcept {
  size_t i = 0;
  if (op >= X86_BSWAP && op <= X86_XABORT) {
    i = size_t(op) - X86_BSWAP + 1;
  }
  return inst1_vec[i];
}

static size_t asm1_insert_prefixes(uint8_t buf[], size_t len, Kind kind, Bits default_size,
                                   Reg base, Reg index = Reg{}) {
  if (kind.bits() == Bits16) {
    buf[len++] = 0x66;
  }
  if ((buf[len] = rex_byte(default_size, kind.bits(), base, index)) != 0) {
    len++;
  }
  return len;
}

static ONEJIT_NOINLINE Assembler &asm1_emit_addr(Assembler &dst, //
                                                 OpStmt1 op,
                                                 const uint8_t prefix[2], //
                                                 Bits default_size,       //
                                                 const Mem &mem) noexcept {
  Reg base{mem.base()};
  Reg index{mem.index()};
  Scale scale = mem.scale();
  if (scale == Scale0 || !index) {
    // if scale or index is not set, clear both.
    index = Reg{};
    scale = Scale0;
  }
  if (!base && !index) {
    // use RSP as index, it is interpreted as zero
    index = Reg{Uint64, RSP};
    scale = Scale1;
  }
  uint8_t buf[10] = {};
  size_t len = asm1_insert_prefixes(buf, 0, mem.kind(), default_size, base, index);
  buf[len++] = prefix[0];
  buf[len] = prefix[1];

  if (op >= X86_SETA && op <= X86_SETS) {
    /// TODO: implement
    return dst.error(mem, "x64::Asm1::emit: unimplemented SETcc with memory reference");
  }

  size_t offset_bytes = Util::get_offset_minbytes(mem, base, index);
  len = Util::insert_modrm_sib(buf, len, offset_bytes, base, index, scale);

  if (offset_bytes != 0) {
    len = Util::insert_offset_or_imm(buf, len, offset_bytes, mem.offset());
  }
  dst.add(onestl::Bytes{buf, len});
  if (auto label = mem.label()) {
    dst.add_relocation(label);
  }
  return dst;
}

static constexpr Bits asm1_default_size(OpStmt1 op) noexcept {
  return (op == X86_CALL || op == X86_JMP || op == X86_POP || op == X86_PUSH) ? Bits64 : Bits32;
}

static ONEJIT_NOINLINE Assembler &asm1_emit_mem(Assembler &dst, const Inst1 &inst, OpStmt1 op,
                                                Mem mem) noexcept {
  if (!Util::validate_mem(dst, mem)) {
    return dst;
  }
  const Bytes bytes = inst.bytes();
  uint8_t prefix[2] = {bytes[0], bytes[1]};
  if ((inst.arg_size() & B8) != 0 && mem.kind().bits() != Bits8) {
    // instruction also supports 8-bit memory access, but requested access is wider.
    prefix[0] |= 1;
  }
  Bits default_size = asm1_default_size(op);

  return asm1_emit_addr(dst, op, prefix, default_size, mem);
}

static ONEJIT_NOINLINE Assembler &asm1_emit_imm(Assembler &dst, const Inst1 &inst, OpStmt1 op,
                                                Imm imm) noexcept {
  const int32_t imm_val = imm.int32();
  size_t imm_len;
  if (imm_val == int32_t(int8_t(imm_val)) && (inst.imm_size() & B8) != B0) {
    imm_len = 1;
  } else if (imm_val == int32_t(int16_t(imm_val)) && (inst.imm_size() & B16) != B0) {
    imm_len = 2;
  } else {
    imm_len = 4;
  }

  uint8_t buf[6] = {};
  Bytes bytes = imm_len == 1 ? inst.imm8_bytes() : inst.imm32_bytes();
  size_t len = 0;
  if (imm_len == 2) {
    buf[len++] = 0x66;
  }

  std::memcpy(buf + len, bytes.data(), bytes.size());
  len += bytes.size();
  (void)op;
  len = Util::insert_offset_or_imm(buf, len, imm_len, imm_val);

  return dst.add(Bytes{buf, len});
}

static ONEJIT_NOINLINE Assembler &asm1_emit_label(Assembler &dst, const Inst1 &inst, OpStmt1 op,
                                                  Label l) noexcept {
  uint8_t buf[6] = {};
  Bytes bytes = inst.imm32_bytes();
  size_t len = bytes.size();

  std::memcpy(buf, bytes.data(), len);
  (void)op;
  dst.add(Bytes{buf, len + 4});
  return dst.add_relocation(l);
}

static Assembler &asm1_emit_pop_or_push_reg(Assembler &dst, OpStmt1 op, Reg reg) noexcept {
  uint8_t buf[4] = {};
  size_t len = asm1_insert_prefixes(buf, 0, reg.kind(), asm1_default_size(op), reg);

  buf[len++] = 0x50 | rlo(reg) | (op == X86_POP ? 0x8 : 0x0);
  return dst.add(onestl::Bytes{buf, len});
}

static Assembler &asm1_emit_rdtsc(Assembler &dst, const Inst1 &inst) noexcept {
  return dst.add(inst.bytes());
}

static Assembler &asm1_emit_setcc_reg(Assembler &dst, const Inst1 &inst, OpStmt1 op,
                                      Reg reg) noexcept {
  uint8_t buf[6] = {};
  size_t len = asm1_insert_prefixes(buf, 0, reg.kind(), asm1_default_size(op), reg);
  const Bytes bytes = inst.bytes();

  buf[len++] = bytes[0];
  buf[len++] = bytes[1];
  buf[len++] = 0xc0 | rlo(reg);
  return dst.add(onestl::Bytes{buf, len});
}

static ONEJIT_NOINLINE Assembler &asm1_emit_reg(Assembler &dst, const Inst1 &inst, OpStmt1 op,
                                                Reg reg) noexcept {
  if (op == X86_POP || op == X86_PUSH) {
    return asm1_emit_pop_or_push_reg(dst, op, reg);
  } else if (op == X86_RDTSC) {
    return asm1_emit_rdtsc(dst, inst);
  } else if (op >= X86_SETA && op <= X86_SETS) {
    return asm1_emit_setcc_reg(dst, inst, op, reg);
  }
  uint8_t buf[4] = {};
  size_t len = asm1_insert_prefixes(buf, 0, reg.kind(), asm1_default_size(op), reg);
  const Bytes bytes = inst.bytes();

  buf[len] = bytes[0];
  buf[len + 1] = bytes[1] | 0xc0 | rlo(reg);
  if ((inst.arg_size() & B8) != 0 && reg.kind().bits() != Bits8) {
    // instruction also supports 8-bit memory access, but requested access is wider.
    buf[len] |= 1;
  }
  return dst.add(onestl::Bytes{buf, len + 2});
}

Assembler &Asm1::emit(Assembler &dst, const Stmt1 &st, const Inst1 &inst) noexcept {
  Node arg = st.child(0);
  if (!is_compatible(arg, inst.arg())) {
    return dst.error(st, "x64::Asm1::emit: instruction does not support specified argument type");
  }

  switch (arg.type()) {
  case VAR:
    if (Var v = arg.is<Var>()) {
      Reg reg{v.local()};
      if (!is_compatible(reg.kind().bits(), inst.arg_size())) {
        return dst.error(st,
                         "x64::Asm1::emit: instruction does not support specified register width");
      }
      return asm1_emit_reg(dst, inst, st.op(), reg);
    }
    break;
  case MEM:
    if (Mem mem = arg.is<Mem>()) {
      if (!is_compatible(mem.kind().bits(), inst.arg_size())) {
        return dst.error(st,
                         "x64::Asm1::emit: instruction does not support specified memory width");
      }
      return asm1_emit_mem(dst, inst, st.op(), mem);
    }
    break;
  case LABEL:
    if (Label l = arg.is<Label>()) {
      if (!is_compatible(Bits32, inst.imm_size())) {
        return dst.error(arg, "x64::Asm1::emit: instruction does not support labels");
      }
      return asm1_emit_label(dst, inst, st.op(), l);
    }
    break;
  case CONST:
    if (Const c = arg.is<Const>()) {
      if (!is_compatible(c.kind().bits(), inst.imm_size())) {
        return dst.error(st,
                         "x64::Asm1::emit: instruction does not support specified immediate width");
      }
      return asm1_emit_imm(dst, inst, st.op(), c.imm());
    }
    break;
  default:
    break;
  }
  return dst.error(
      st, "x64::Asm1::emit: unexpected argument type, expecting Const, Label, Var or x64::Mem");
}

Assembler &Asm1::emit(Assembler &dst, const Stmt1 &st) noexcept {
  return emit(dst, st, find(st.op()));
}

} // namespace x64
} // namespace onejit
