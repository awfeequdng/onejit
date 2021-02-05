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
 * test_disasm.cpp
 *
 *  Created on Feb 04, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/fmt.hpp>

#include "test_disasm.hpp"

#ifdef HAVE_LIBCAPSTONE
#include <arm64.h>
#include <capstone.h>
#include <x86.h>
#endif

namespace onejit {

#ifdef HAVE_LIBCAPSTONE

TestDisasm::TestDisasm() noexcept : handle_{}, err_{cs_open(CS_ARCH_X86, CS_MODE_64, &handle_)} {
  if (err_ != CS_ERR_OK) {
    return;
  }
  cs_option(handle_, CS_OPT_DETAIL, CS_OPT_ON);
}

TestDisasm::~TestDisasm() noexcept {
  if (err_ != CS_ERR_OK) {
    return;
  }
  cs_close(&handle_);
}

const Fmt &TestDisasm::disasm(const Fmt &out, Bytes bytes) {
  if (err_ != CS_ERR_OK) {
    return out << "capstone initialization error";
  }
  cs_insn *insn = nullptr;
  size_t count = cs_disasm(handle_, bytes.data(), bytes.size(), 0x1000, 0, &insn);
  if (count <= 0) {
    return out << "capstone disassemble error";
  }
  for (size_t i = 0; i < count; i++) {
    show(out, insn + i);
    out << '\n';
  }
  cs_free(insn, count);
  return out;
}

const Fmt &TestDisasm::show(const Fmt &out, const cs_insn *insn) {
  out << "(x86_" << cs_insn_name(handle_, insn->id);

  cs_detail *detail = insn->detail;
  for (size_t i = 0, n = detail->x86.op_count; i < n; i++) {
    cs_x86_op *op = detail->x86.operands + i;
    switch (op->type) {
    case X86_OP_REG:
      out << " " << cs_reg_name(handle_, op->reg);
      break;
    case X86_OP_IMM:
      out << ' ' << op->imm;
      break;
    case X86_OP_MEM:
      out << " (x86_mem";
      if (op->mem.disp != 0) {
        out << " _ " << op->mem.disp << "_i"; // suffix for int32
      } else if (op->mem.base != X86_REG_INVALID || op->mem.index != X86_REG_INVALID) {
        out << " _ _";
      }
      if (op->mem.base != X86_REG_INVALID) {
        out << ' ' << cs_reg_name(handle_, op->mem.base);
      } else if (op->mem.index != X86_REG_INVALID) {
        out << " _";
      }
      if (op->mem.index != X86_REG_INVALID) {
        out << ' ' << cs_reg_name(handle_, op->mem.index);
        out << ' ' << op->mem.scale << "_ub";
      }
      out << ')';
      break;
    default:
      break;
    }
  }
  return out << ')';
}

#else // !HAVE_LIBCAPSTONE

TestDisasm::TestDisasm() noexcept {
}
TestDisasm::~TestDisasm() noexcept {
}
const Fmt &TestDisasm::disasm(const Fmt &out, Bytes bytes) {
  (void)bytes;
  return out << "capstone support non compiled";
}

#endif

} // namespace onejit
