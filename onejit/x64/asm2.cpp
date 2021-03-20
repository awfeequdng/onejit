/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * asm2.cpp
 *
 *  Created on Feb 02, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/assembler.hpp>
#include <onejit/ir/stmt2.hpp>
#include <onejit/x64/asm.hpp>
#include <onejit/x64/inst.hpp>

namespace onejit {
namespace x64 {

using namespace onejit;

static const Inst2 inst2_vec[] = {
    Inst2{Arg2::None}, /* bad instruction */
};

const Inst2 &Asm2::find(OpStmt2 op) noexcept {
  /// TODO: implement
  (void)op;
  return inst2_vec[0];
}

Assembler &Asm2::emit(Assembler &dst, const Stmt2 &st, const Inst2 &inst) noexcept {
  Node arg0 = st.child(0);
  Node arg1 = st.child(1);
  if (!is_compatible(arg0, arg1, inst.arg())) {
    return dst.error(st, "x64::Asm2::emit: instruction does not support specified argument types");
  }

  return dst.error(st, "unimplemented x64::Asm2::emit");
}

Assembler &Asm2::emit(Assembler &dst, const Stmt2 &st) noexcept {
  return emit(dst, st, find(st.op()));
}

} // namespace x64
} // namespace onejit
