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
