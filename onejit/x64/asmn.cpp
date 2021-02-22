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
 * asmn.cpp
 *
 *  Created on Feb 20, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/assembler.hpp>
#include <onejit/node/stmtn.hpp>
#include <onejit/x64/asm.hpp>
#include <onejit/x64/inst.hpp>

namespace onejit {
namespace x64 {

using namespace onejit;

static const InstN instn_vec[] = {
    InstN{"\x0f\x0b"}, /* ud2      undefined instruction, causes SIGILL  */
    InstN{"\xc3"},     /* ret      return from function call             */
};

const InstN &AsmN::find(OpStmtN op) noexcept {
  size_t i = 0;
  if (op >= X86_RET && op <= X86_RET) {
    i = size_t(op) - X86_RET + 1;
  }
  return instn_vec[i];
}

Assembler &AsmN::emit(Assembler &dst, const InstN &inst) noexcept {
  return dst.add(inst.bytes());
}

Assembler &AsmN::emit(Assembler &dst, const StmtN &st) noexcept {
  return emit(dst, find(st.op()));
}

} // namespace x64
} // namespace onejit
