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

#include <onejit/algorithm.hpp>
#include <onejit/flowgraph.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/node.hpp>
#include <onejit/ir/util.hpp>
#include <onejit/type.hpp>

namespace onejit {

FlowGraph::FlowGraph() noexcept : basicblocks_{}, links_{}, label_n_{}, link_avail_{} {
}

FlowGraph::~FlowGraph() noexcept {
}

bool FlowGraph::build(Span<Node> nodes) noexcept {
  basicblocks_.clear();
  links_.clear();
  link_avail_ = label_n_ = 0;

  if (!nodes) {
    return true;
  }
  if (!build_basicblocks(nodes)) {
    return false;
  }
  resolve_labels();
  resolve_next();
  resolve_prev();
  return true;
}

bool FlowGraph::is_label(Node node) noexcept {
  return node.type() == LABEL;
}

bool FlowGraph::build_basicblocks(Span<Node> nodes) noexcept {
  Node node;
  size_t start = 0, i = 0;
  size_t n = nodes.size();
  size_t jumps = 0;
  size_t label_max = 0;
  bool ok = true;

  while (ok && i < n) {
    while (i < n && is_label(node = nodes[i])) {
      i++;
      label_max = max2(label_max, node.is<Label>().index());
    }
    while (i < n) {
      node = nodes[i++];
      if (ir::is_uncond_jump(node)) {
        jumps++;
        break;
      } else if (ir::is_cond_jump(node)) {
        if (i == n) {
          // invalid nodes: they end with a conditional jump
          // where should it fallthrough when it does NOT jump?
          return false;
        }
        jumps += 2;
        break;
      }
    }
    ok = basicblocks_.append(BasicBlock{nodes, start, i});
    start = i;
  }
  link_avail_ = label_n_ = 1 + label_max;
  return ok && links_.resize(1 + label_max + jumps * 2);
}

void FlowGraph::resolve_labels() noexcept {
  for (BasicBlock &bb : basicblocks_) {
    for (Node node : bb) {
      if (!is_label(node)) {
        break;
      }
      const size_t index = node.is<Label>().index();
      links_.set(index, &bb);
    }
  }
}

void FlowGraph::resolve_next() noexcept {
  size_t link_start = label_n_;
  size_t link_end = link_start;
  for (BasicBlock &bb : basicblocks_) {
    const size_t node_n = bb.size();
    if (node_n == 0) {
      continue;
    }
    Node node = bb[node_n - 1];
    const bool is_cond_jump = ir::is_cond_jump(node);
    if (!is_cond_jump && !ir::is_uncond_jump(node)) {
      continue;
    }
    Label label = ir::jump_label(node);
    const size_t index = label.index();
    if (!label || index >= label_n_) {
      continue;
    }
    links_.set(link_end++, links_[index]);
    if (is_cond_jump) {
      // conditional jump may fallthrough to next basicblock
      links_.set(link_end++, &bb + 1);
    }
    bb.set_next(links_.span(link_start, link_end));
    link_start = link_end;
  }
  link_avail_ = link_end;
}

void FlowGraph::resolve_prev() noexcept {
  size_t link_start = link_avail_;
  size_t link_end = link_start;
  // this is O(n^2) where n = basicblocks_.size()
  // optimizing to O(n) requires incrementally populating
  // a map to -> [from_1...from_k]
  for (BasicBlock &bb : basicblocks_) {
    for (BasicBlock &from : basicblocks_) {
      for (BasicBlock *jump_to : from.next()) {
        if (jump_to == &bb) {
          links_.set(link_end++, &from);
          break;
        }
      }
    }
    if (link_start < link_end) {
      bb.set_prev(links_.span(link_start, link_end));
      link_start = link_end;
    }
  }
  link_avail_ = link_end;
}

} // namespace onejit
