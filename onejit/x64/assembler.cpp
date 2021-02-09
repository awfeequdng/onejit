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
 * asm.cpp
 *
 *  Created on Feb 01, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/assembler.hpp>
#include <onejit/node/stmt0.hpp>
#include <onejit/node/stmt1.hpp>
#include <onejit/node/stmt2.hpp>
#include <onejit/node/stmt3.hpp>
#include <onejit/x64/asm.hpp>

namespace onejit {

// declared in onejit/assembler.hpp
Assembler &Assembler::x64(const Node &node) noexcept {
  switch (node.type()) {
  case STMT_0:
    return onejit::x64::Asm0::emit(*this, node.is<Stmt0>());
  case STMT_1:
    return onejit::x64::Asm1::emit(*this, node.is<Stmt1>());
  case STMT_2:
    return onejit::x64::Asm2::emit(*this, node.is<Stmt2>());
  case STMT_3:
    return onejit::x64::Asm3::emit(*this, node.is<Stmt3>());
  default:
    return error(node, "unexpected node type in Assembler::x64, expecting Stmt[0123]");
  }
}

} // namespace onejit
