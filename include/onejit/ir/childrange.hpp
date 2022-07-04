/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * childrange.hpp
 *
 *  Created on Feb 22, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_CHILDRANGE_HPP
#define ONEJIT_IR_CHILDRANGE_HPP

#include <onejit/ir/node.hpp>

namespace onejit {
namespace ir {

////////////////////////////////////////////////////////////////////////////////
// range of children inside a Node.
class ChildRange {
public:
  constexpr ChildRange() noexcept : node_{}, start_{}, size_{} {
  }

  // construct a ChildRange containing all children of Node
  explicit ChildRange(Node node) noexcept //
      : node_{node}, start_{0}, size_{node.children()} {
  }

  // construct a ChildRange containing a subse of Node's children
  constexpr ChildRange(Node node, uint32_t start, uint32_t size) noexcept
      : node_{node}, start_{start}, size_{size} {
  }

  uint32_t constexpr size() const noexcept {
    return size_;
  }

  explicit operator bool() const noexcept;

  // checked element access:
  /// @return i-th Node by value, or Node{} if index is out of bounds
  Node operator[](uint32_t i) const noexcept {
    return i < size_ ? node_.child(i + start_) : Node{};
  }

private:
  Node node_;
  uint32_t start_, size_;
};

const Fmt &operator<<(const Fmt &fmt, const ChildRange &range);

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_CHILDRANGE_HPP
