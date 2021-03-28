/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * asmn.cpp
 *
 *  Created on Feb 20, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/assembler.hpp>
#include <onejit/ir/stmtn.hpp>
#include <onejit/x64/asm.hpp>
#include <onejit/x64/inst.hpp>

namespace onejit {
namespace x64 {

using namespace onejit;

static const InstN instn_vec[] = {
    InstN{"\x0f\x0b"}, /* call_    undefined instruction. 'call' should be assembled instead */
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
