/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * util.cpp
 *
 *  Created on Mar 09, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/ir/label.hpp>
#include <onejit/ir/node.hpp>
#include <onejit/ir/util.hpp>

namespace onejit {
namespace ir {

bool is_jump(Node node) noexcept {
  return is_uncond_jump(node) || is_cond_jump(node);
}

bool is_uncond_jump(Node node) noexcept {
  if (node.type() == STMT_1) {
    OpStmt1 op1 = OpStmt1(node.op());
    return op1 == GOTO || op1 == MIR_JMP || op1 == X86_JMP;
  } else {
    return is_return(node);
  }
  return false;
}

bool is_cond_jump(Node node) noexcept {
  switch (node.type()) {
  case STMT_1:
    switch (OpStmt1(node.op())) {
    case X86_JA:
    case X86_JAE:
    case X86_JB:
    case X86_JBE:
    case X86_JE:
    case X86_JG:
    case X86_JGE:
    case X86_JL:
    case X86_JNE:
    case X86_JNO:
    case X86_JNP:
    case X86_JNS:
    case X86_JO:
    case X86_JP:
    case X86_JS:
      return true;
    default:
      break;
    }
    break;

  case STMT_2:
    switch (OpStmt2(node.op())) {
    case JUMP_IF:
    case X86_XBEGIN:
      return true;
    default:
      break;
    }
    break;

  case STMT_3:
    switch (OpStmt3(node.op())) {
    case ASM_JA:
    case ASM_JAE:
    case ASM_JB:
    case ASM_JBE:
    case ASM_JE:
    case ASM_JG:
    case ASM_JGE:
    case ASM_JL:
    case ASM_JNE:
      return true;
    default:
      break;
    }
    break;

  default:
    break;
  }
  return false;
}

bool is_return(Node node) noexcept {
  if (node.type() == STMT_N) {
    OpStmtN opn = OpStmtN(node.op());
    return opn == RETURN || opn == X86_RET;
  }
  return false;
}

Label jump_label(Node node) noexcept {
  Label label;
  if (node.type() != STMT_N) {
    label = node.child_is<Label>(0);
  }
  return label;
}

} // namespace ir
} // namespace onejit
