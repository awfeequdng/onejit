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
 * asm3.cpp
 *
 *  Created on Feb 02, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/assembler.hpp>
#include <onejit/stmt3.hpp>
#include <onejit/x64/asm.hpp>
#include <onejit/x64/inst.hpp>

namespace onejit {
namespace x64 {

using namespace onejit;

static const Inst3 inst3_vec[] = {
    Inst3{Arg3::None}, /* bad instruction */
};

const Inst3 &Asm3::find(OpStmt3 op) noexcept {
  /// TODO: implement
  (void)op;
  return inst3_vec[0];
}

Assembler &Asm3::emit(Assembler &dst, const Stmt3 &st, const Inst3 &inst) noexcept {
  Node arg0 = st.child(0);
  Node arg1 = st.child(1);
  Node arg2 = st.child(2);
  if (!is_compatible(arg0, arg1, arg2, inst.arg())) {
    return dst.error(st, "x64::Asm3::emit: instruction does not support specified argument types");
  }

  return dst.error(st, "unimplemented x64::Asm3::emit");
}

Assembler &Asm3::emit(Assembler &dst, const Stmt3 &st) noexcept {
  return emit(dst, st, find(st.op()));
}

} // namespace x64
} // namespace onejit
