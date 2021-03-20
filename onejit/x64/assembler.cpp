/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * asm.cpp
 *
 *  Created on Feb 01, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/assembler.hpp>
#include <onejit/ir/stmt0.hpp>
#include <onejit/ir/stmt1.hpp>
#include <onejit/ir/stmt2.hpp>
#include <onejit/ir/stmt3.hpp>
#include <onejit/ir/stmtn.hpp>
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
  case STMT_N:
    return onejit::x64::AsmN::emit(*this, node.is<StmtN>());
  default:
    return error(node, "unexpected node type in Assembler::x64, expecting Stmt[0123N]");
  }
}

} // namespace onejit
