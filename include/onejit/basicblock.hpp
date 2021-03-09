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
 * basicblock.hpp
 *
 *  Created on Mar 08, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_BASICBLOCK_HPP
#define ONEJIT_BASICBLOCK_HPP

#include <onejit/fwd.hpp>
#include <onejit/ir/node.hpp>
#include <onestl/crange.hpp>
#include <onestl/view.hpp>

namespace onejit {

// interval of nodes without internal labels or jumps:
// labels can only be at the beginning, and jumps can only be at the end
// => a BasicBlock execution is always sequential, from first to last node
class BasicBlock : public Nodes {
  using Base = Nodes;

public:
  constexpr BasicBlock() noexcept : Base{}, prev_{}, next_{} {
  }

  constexpr explicit BasicBlock(Nodes nodes) noexcept //
      : Base{nodes}, prev_{}, next_{} {
  }

  constexpr BasicBlock(Nodes nodes, size_t start, size_t size) noexcept
      : Base{nodes.view(start, size)} {
  }

  // redundant
  // ~BasicBlock() noexcept = default;

  void set_prev(const CRange<BasicBlock *> &prev) noexcept {
    prev_ = prev;
  }

  void set_next(const CRange<BasicBlock *> &next) noexcept {
    next_ = next;
  }

private:
  CRange<BasicBlock *> prev_;
  CRange<BasicBlock *> next_;

}; // class BasicBlock

} // namespace onejit

#endif // ONEJIT_BASICBLOCK_HPP
