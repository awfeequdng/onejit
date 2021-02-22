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
 * childrange.hpp
 *
 *  Created on Feb 22, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_CHILDRANGE_HPP
#define ONEJIT_NODE_CHILDRANGE_HPP

#include <onejit/node/node.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// range of children inside a Node.
class ChildRange {
public:
  constexpr ChildRange() noexcept : node_{}, start_{}, size_{} {
  }

  constexpr ChildRange(Node node, uint32_t start, uint32_t size) noexcept
      : node_{node}, start_{start}, size_{size} {
  }

  uint32_t constexpr size() const noexcept {
    return size_;
  }

  explicit operator bool() const noexcept;

  // checked element access:
  // return i-th Node by value, or Node{} if index is out of bounds
  Node operator[](uint32_t i) const noexcept {
    return i < size_ ? node_.child(i + start_) : Node{};
  }

private:
  Node node_;
  uint32_t start_, size_;
};

} // namespace onejit

#endif // ONEJIT_NODE_CHILDRANGE_HPP
