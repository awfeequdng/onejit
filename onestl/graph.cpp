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
  size_t oldn = degree_.size();
  if (degree_.resize(nodes)) {
    if (bits_.resize(nodes * (nodes + 1) / 2)) {
      bits_.fill(0, bits_.size(), false);
      std::memset(degree_.data(), '\0', nodes * sizeof(Degree));
      return true;
    }
    degree_.resize(oldn);
  }
  return false;
}

bool Graph::operator()(Node a, Node b) const noexcept {
  if (a > b) {
    mem::swap(a, b);
  }
  if (b >= size()) {
    return false;
  }
  return bits_[a + b * (b + 1) / 2];
}

void Graph::set(Node a, Node b, bool value) noexcept {
  if (a > b) {
    mem::swap(a, b);
  }
  if (b < size()) {
    size_t offset = a + b * (b + 1) / 2;
    bool prev = bits_[offset];
    if (Degree delta = Degree(value) - Degree(prev)) {
      degree_.data()[a] += delta;
      degree_.data()[b] += delta; // even if a == b
      bits_.set(offset, value);
    }
  }
}

#if 0  // TODO
Graph::Node Graph::first_neighbor(Node node) const noexcept {
  if (degree(node) != 0) {
  }
  return Node(-1);
}

void Graph::remove(Node node) noexcept {
  Degree deg = degree(node);
  Node other = Node(-1);
}
#endif // 0

} // namespace onestl
