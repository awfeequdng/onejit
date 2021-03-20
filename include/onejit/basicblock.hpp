/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
