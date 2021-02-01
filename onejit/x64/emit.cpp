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
 * emit.cpp
 *
 *  Created on Feb 01, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/stmt0.hpp>
#include <onejit/stmt1.hpp>
#include <onejit/stmt2.hpp>
#include <onejit/stmt3.hpp>
#include <onejit/x64/emit.hpp>
#include <onejit/x64/inst.hpp>

namespace onejit {
namespace x64 {

Assembler &emit(Assembler &dst, Node node) noexcept {
  switch (node.type()) {
  case STMT_0:
    return emit(dst, node.is<Stmt0>());
  case STMT_1:
    return emit(dst, node.is<Stmt1>());
  case STMT_2:
    return emit(dst, node.is<Stmt2>());
  case STMT_3:
    return emit(dst, node.is<Stmt3>());
  default:
    return dst.error(node, "unexpected node type in x64::emit, expecting Stmt[0123]");
  }
}

Assembler &emit(Assembler &dst, Stmt0 st) noexcept {
  return Inst0::find(st.op()).emit(dst);
}

Assembler &emit(Assembler &dst, Stmt1 st) noexcept {
  return Inst1::find(st.op()).emit(dst, st);
}

Assembler &emit(Assembler &dst, Stmt2 st) noexcept {
  /// TODO: implement
  (void)dst;
  (void)st;
  return dst;
}

Assembler &emit(Assembler &dst, Stmt3 st) noexcept {
  /// TODO: implement
  (void)dst;
  (void)st;
  return dst;
}

} // namespace x64
} // namespace onejit
