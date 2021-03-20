/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * flowgraph.hpp
 *
 *  Created on Mar 08, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FLOWGRAPH_HPP
#define ONEJIT_FLOWGRAPH_HPP

#include <onejit/basicblock.hpp>
#include <onestl/array.hpp>

namespace onejit {

// compute global control flow graph of a function
class FlowGraph {

public:
  FlowGraph() noexcept;
  FlowGraph(FlowGraph &&) noexcept = default;

  ~FlowGraph() noexcept;

  FlowGraph &operator=(FlowGraph &&) noexcept = default;

  // return false if out of memory
  bool build(Span<Node> nodes, Array<Error> &error) noexcept;

  const Fmt &format(const Fmt &fmt) const;

private:
  // return false if out of memory
  bool build_basicblocks(Span<Node> nodes) noexcept;
  // for every label in basicblocks_,
  // set links_[label.index()] to the basic block containing label
  bool resolve_labels() noexcept;
  // for every jump in basicblocks_, find the basicblocks it may jump to,
  // fill links_ accordingly, and assign a slice of it to basicblocks_[i].next_
  bool resolve_next() noexcept;
  // using basicblocks_[*].next_, compute basicblocks_[*].prev_
  bool resolve_prev() noexcept;

  static bool is_label(Node node) noexcept;

  // always returns false
  bool error(Node where, Chars msg) noexcept;

  Array<BasicBlock> basicblocks_;
  Array<BasicBlock *> links_;
  Array<Error> *error_;
  size_t label_n_;
  size_t link_avail_;

}; // class FlowGraph

String to_string(const FlowGraph &flowgraph);

inline const Fmt &operator<<(const Fmt &fmt, const FlowGraph &flowgraph) {
  return flowgraph.format(fmt);
}

} // namespace onejit

#endif // ONEJIT_FLOWGRAPH_HPP
