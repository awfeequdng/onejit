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
#include <onestl/sspan.hpp>

namespace onejit {

// interval of nodes without internal labels or jumps:
// labels can only be at the beginning, and jumps can only be at the end
// => a BasicBlock execution is always sequential, from first to last node
class BasicBlock : public Span<Node> {
  using Base = Span<Node>;

public:
  constexpr BasicBlock() noexcept : Base{}, prev_{}, next_{} {
  }

  constexpr explicit BasicBlock(Span<Node> nodes) noexcept //
      : Base{nodes}, prev_{}, next_{} {
  }

  /*constexpr*/ BasicBlock(Span<Node> nodes, size_t a_start, size_t a_end) noexcept
      : Base{nodes.span(a_start, a_end)} {
  }

  // redundant
  // ~BasicBlock() noexcept = default;

  constexpr Span<BasicBlock *> prev() const noexcept {
    return prev_;
  }
  constexpr Span<BasicBlock *> next() const noexcept {
    return next_;
  }

  void set_prev(Span<BasicBlock *> prev) noexcept {
    prev_ = prev;
  }
  void set_next(Span<BasicBlock *> next) noexcept {
    next_ = next;
  }

private:
  SSpan<BasicBlock *> prev_;
  SSpan<BasicBlock *> next_;

}; // class BasicBlock

} // namespace onejit

#endif // ONEJIT_BASICBLOCK_HPP
