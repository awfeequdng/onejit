/*
 * onestl - Tiny STL C++ library
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
 * graph.cpp
 *
 *  Created on Feb 27, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/graph.hpp>
#include <onestl/mem.hpp>

namespace onestl {

bool Graph::reset(size_t nodes) noexcept {
  size_t oldn = edges_.size();
  if (edges_.resize(nodes)) {
    if (bits_.resize(nodes * (nodes + 1) / 2)) {
      bits_.fill(0, bits_.size(), false);
      std::memset(edges_.data(), '\0', nodes * sizeof(uint32_t));
      return true;
    }
    edges_.resize(oldn);
  }
  return false;
}

bool Graph::operator()(size_t a, size_t b) const noexcept {
  if (a > b) {
    mem::swap(a, b);
  }
  if (b >= size()) {
    return false;
  }
  return bits_[a + b * (b + 1) / 2];
}

void Graph::set(size_t a, size_t b, bool value) noexcept {
  if (a > b) {
    mem::swap(a, b);
  }
  if (b < size()) {
    size_t offset = a + b * (b + 1) / 2;
    bool prev = bits_[offset];
    if (uint32_t delta = uint32_t(value) - uint32_t(prev)) {
      bits_.set(offset, value);
      edges_.data()[a] += delta;
      edges_.data()[b] += delta; // even if a == b
    }
  }
}

void Graph::swap(Graph &other) noexcept {
  bits_.swap(other.bits_);
  edges_.swap(other.edges_);
}

} // namespace onestl
