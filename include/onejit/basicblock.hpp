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
#include <onestl/crange.hpp>

namespace onejit {

// range of nodes without internal labels or jumps:
// labels can only be at the beginning, and jumps can only be at the end
// => a BasicBlock execution is always sequential, from first to last node
class BasicBlock : public CRange<Node> {
  using Base = CRange<Node>;

public:
  constexpr BasicBlock() noexcept : Base{}, prev_{}, next_{} {
  }

  constexpr explicit BasicBlock(const Nodes *nodes) noexcept : Base{nodes}, prev_{}, next_{} {
  }

  constexpr BasicBlock(const Nodes *nodes, size_t a_start, size_t a_size) noexcept
      : Base{nodes, a_start, a_size}, prev_{}, next_{} {
  }

  // redundant
  // ~BasicBlock() noexcept = default;

private:
  CRange<BasicBlock *> prev_; // predecessors
  CRange<BasicBlock *> next_; // successors

}; // class BasicBlock

} // namespace onejit

#endif // ONEJIT_BASICBLOCK_HPP
