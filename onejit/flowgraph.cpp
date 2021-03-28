/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * flowgraph.cpp
 *
 *  Created on Mar 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/algorithm.hpp>
#include <onejit/error.hpp>
#include <onejit/flowgraph.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/node.hpp>
#include <onejit/ir/util.hpp>
#include <onejit/type.hpp>

#include <cstdio>

namespace onejit {

FlowGraph::FlowGraph() noexcept : basicblocks_{}, links_{}, error_{}, label_n_{}, link_avail_{} {
}

FlowGraph::~FlowGraph() noexcept {
}

bool FlowGraph::build(Span<Node> nodes, Array<Error> &error) noexcept {
  basicblocks_.clear();
  links_.clear();
  error_ = &error;
  link_avail_ = label_n_ = 0;

  if (!nodes) {
    return true;
  }
  return build_basicblocks(nodes) && resolve_labels() //
         && resolve_next() && resolve_prev();
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
      node = nodes[i];
      if (is_label(node)) {
        jumps++;
        break;
      }
      i++;
      if (ir::is_uncond_jump(node)) {
        jumps++;
        break;
      } else if (ir::is_cond_jump(node)) {
        if (i == n) {
          // nodes end with a conditional jump:
          // it is either unreachable or followed by an implicit RETURN
          jumps++;
        } else {
          jumps += 2;
        }
        break;
      }
    }
    ok = basicblocks_.append(BasicBlock{nodes, start, i});
    start = i;
  }
  link_avail_ = label_n_ = 1 + label_max;
  return ok && links_.resize(1 + label_max + jumps * 2);
}

bool FlowGraph::resolve_labels() noexcept {
  for (BasicBlock &bb : basicblocks_) {
    for (Node node : bb) {
      if (!is_label(node)) {
        break;
      }
      const size_t index = node.is<Label>().index();
      links_.set(index, &bb);
    }
  }
  return true;
}

bool FlowGraph::resolve_next() noexcept {
  size_t link_start = label_n_;
  size_t link_end = link_start;
  size_t i = 0, n = basicblocks_.size();
  for (BasicBlock &bb : basicblocks_) {
    i++;
    const size_t node_n = bb.size();
    if (node_n == 0) {
      continue;
    }
    Node node = bb[node_n - 1];
    const bool is_uncond_jump = ir::is_uncond_jump(node);
    if (!is_uncond_jump && i < n) {
      // either a conditional jump, or no jump at all
      // => basicblock may fallthrough to next basicblock
      links_.set(link_end++, &bb + 1);
    }
    if (is_uncond_jump || ir::is_cond_jump(node)) {
      Label label = ir::jump_label(node);
      const size_t index = label.index();
      if (!label) {
        // RETURN, X86_RET or similar
      } else if (index >= label_n_) {
        return error(node, "invalid label");
      } else if (BasicBlock *to = links_[index]) {
        links_.set(link_end++, to);
      } else {
        return error(node, "label not found");
      }
    }
    bb.set_next(links_.span(link_start, link_end));
    link_start = link_end;
  }
  link_avail_ = link_end;
  return true;
}

bool FlowGraph::resolve_prev() noexcept {
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
  return true;
}

bool FlowGraph::error(Node where, Chars msg) noexcept {
  if (error_) {
    error_->append(Error{where, msg});
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////

String to_string(const FlowGraph &flowgraph) {
  String str;
  Fmt{&str} << flowgraph;
  return str;
}

const Fmt &FlowGraph::format(const Fmt &fmt) const {
  fmt << "(flowgraph";
  size_t i = 0;
  for (const BasicBlock &bb : basicblocks_) {
    fmt << "\n    (bb_" << (i++);
    if (Span<BasicBlock *> span = bb.prev()) {
      fmt << "\n        (prev";
      for (const BasicBlock *prev : span) {
        fmt << " bb_" << (prev - basicblocks_.data());
      }
      fmt << ')';
    }
    fmt << "\n        (nodes";
    for (Node node : bb) {
      fmt << "\n            ";
      node.format(fmt, Syntax::Default, 3);
    }
    fmt << "\n        )";
    if (Span<BasicBlock *> span = bb.next()) {
      fmt << "\n        (next";
      for (const BasicBlock *next : span) {
        fmt << " bb_" << (next - basicblocks_.data());
      }
      fmt << ")";
    }
    fmt << "\n    )";
  }
  return fmt << "\n)";
}

} // namespace onejit
