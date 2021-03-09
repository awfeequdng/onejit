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
#include <onejit/ir/node.hpp>
#include <onejit/ir/util.hpp>
#include <onejit/type.hpp>

namespace onejit {

FlowGraph::FlowGraph() noexcept : bb_{}, link_{} {
}

FlowGraph::~FlowGraph() noexcept {
}

bool FlowGraph::build(Span<Node> nodes) noexcept {
  bb_.clear();
  link_.clear();

  if (!nodes) {
    return true;
  }
  size_t start = 0, i = 0;
  size_t n = nodes.size();
  size_t links = 0;
  Node node;
  bool ok = true;

  while (ok && i < n) {
    while (i < n && is_label(node = nodes[i])) {
      i++;
    }
    while (i < n) {
      node = nodes[i];
      if (ir::is_uncond_jump(node)) {
        links++;
        break;
      } else if (ir::is_cond_jump(node)) {
        links += 2;
        break;
      } else {
        i++;
      }
    }
    ok = bb_.append(BasicBlock{nodes, start, i - start});
    start = i;
  }
  ok = ok && link_.resize(links * 2);
  if (!ok) {
    return ok;
  }
  return ok;
}

bool FlowGraph::is_label(Node node) noexcept {
  return node.type() == LABEL;
}

} // namespace onejit
