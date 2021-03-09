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
 * flowgraph.cpp
 *
 *  Created on Mar 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/flowgraph.hpp>
#include <onejit/node/node.hpp>
#include <onejit/type.hpp>

namespace onejit {

FlowGraph::FlowGraph() noexcept : bb_{}, link_{} {
}

FlowGraph::~FlowGraph() noexcept {
}

bool FlowGraph::compute(Nodes nodes) noexcept {
  bb_.clear();
  link_.clear();

  if (!nodes) {
    return true;
  }
  size_t start = 0, i = 0;
  const size_t n = nodes.size();
  Node node;
  bool ok = true;

  while (ok && i < n) {
    while (i < n && is_label(node = nodes[i])) {
      i++;
    }
    while (i < n && !is_jump(node = nodes[i])) {
      i++;
    }
    ok = bb_.append(BasicBlock{nodes, start, i - start});
    start = i;
  }
  return ok;
}

bool FlowGraph::is_label(Node node) noexcept {
  return node.type() == LABEL;
}

bool FlowGraph::is_jump(Node node) noexcept {
  switch (node.type()) {
  case STMT_1:
    switch (OpStmt1(node.op())) {
    case GOTO:

    case ASM_JA:
    case ASM_JAE:
    case ASM_JB:
    case ASM_JBE:
    case ASM_JE:
    case ASM_JG:
    case ASM_JGE:
    case ASM_JL:
    case ASM_JNE:

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

    case X86_JMP:
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

  case STMT_N:
    switch (OpStmtN(node.op())) {
    case RETURN:
    case X86_RET:
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

} // namespace onejit
