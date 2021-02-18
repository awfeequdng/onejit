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
 * noderange.hpp
 *
 *  Created on Feb 18, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_NODERANGE_HPP
#define ONEJIT_NODE_NODERANGE_HPP

#include <onejit/node/node.hpp>
#include <onestl/vector.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// range of nodes inside a Vector<Node>. it remains valid
// even after the Vector is resized or changes capacity
class NodeRange {
public:
  constexpr NodeRange() noexcept : vec_{}, start_{}, size_{} {
  }

  constexpr NodeRange(Vector<Node> *vec, size_t start, size_t size) noexcept
      : vec_{vec}, start_{start}, size_{size} {
  }

  size_t constexpr size() const noexcept {
    return size_;
  }

  constexpr Node operator[](size_t i) const noexcept {
    return vec_ && i < size_ ? vec_->get(i + start_) : Node{};
  }

  Span<Node> span() noexcept {
    return bool(*this) ? Span<Node>{vec_->data() + start_, size_} : Span<Node>{};
  }

  constexpr Nodes view() const noexcept {
    return bool(*this) ? Nodes{vec_->data() + start_, size_} : Nodes{};
  }

  constexpr explicit operator bool() const noexcept {
    return vec_ && size_ && size_ <= vec_->size() && start_ <= vec_->size() - size_;
  }

private:
  Vector<Node> *vec_;
  size_t start_, size_;
};

} // namespace onejit

#endif // ONEJIT_NODE_NODERANGE_HPP
