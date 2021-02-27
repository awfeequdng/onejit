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

namespace onestl {

bool Graph::reset(size_t nodes) noexcept {
  if (!bits_.resize(nodes * (nodes + 1) / 2)) {
    return false;
  }
  n_ = nodes;
  bits_.fill(0, bits_.size(), false);
  return true;
}

bool Graph::operator()(size_t i, size_t j) const noexcept {
  if (i > j) {
    std::swap(i, j);
  }
  if (j >= n_) {
    return false;
  }
  return bits_[i + j * (j + 1) / 2];
}

void Graph::set(size_t i, size_t j, bool value) noexcept {
  if (i > j) {
    std::swap(i, j);
  }
  if (j < n_) {
    bits_.set(i + j * (j + 1) / 2, value);
  }
}

} // namespace onestl
