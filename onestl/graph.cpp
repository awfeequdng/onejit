/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * graph.cpp
 *
 *  Created on Feb 27, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/graph.hpp>
#include <onestl/mem.hpp>

namespace onestl {

Graph::~Graph() noexcept {
}

bool Graph::reset(Size nodes) noexcept {
  size_t newn = nodes;
  size_t oldn = degree_.size();
  if (degree_.resize(newn)) {
    if (bits_.resize(newn * newn)) {
      bits_.fill(false);
      std::memset(degree_.data(), '\0', newn * sizeof(Degree));
      return true;
    }
    degree_.resize(oldn);
  }
  return false;
}

bool Graph::operator()(Node a, Node b) const noexcept {
  if (a < b) {
    mem::swap(a, b);
  }
  const Size n = size();
  if (a >= n) {
    return false;
  }
  return bits_[a + b * n];
}

void Graph::set(Node a, Node b, bool value) noexcept {
  if (a < b) {
    mem::swap(a, b);
  }
  const size_t n = size(); // not Size, "* n" below could overflow
  if (a < n) {
    size_t offset = a + b * n;
    bool prev = bits_[offset];
    if (Degree delta = Degree(value) - Degree(prev)) {
      degree_.data()[a] += delta;
      degree_.data()[b] += delta; // even if a == b
      bits_.set(offset, value);
      if (a != b) {
        // set both a->b and b->a
        bits_.set(b + a * n, value);
      }
    }
  }
}

Graph::Node Graph::first_set(Node node, Node first_neighbor) const noexcept {
  size_t n = size(); // not Size, "* n" below could overflow
  if (first_neighbor >= n || degree(node) == 0) {
    // degree(node) == 0 also catches node >= n
    return NoPos;
  }
  size_t y_offset = node * n;
  size_t offset = bits_.find(true, y_offset + first_neighbor, y_offset + n);
  // NoPos is uint32_t(-1), while BitSet::NoPos is size_t(-1)
  if (offset != BitSet::NoPos) {
    offset -= y_offset;
  }
  return offset;
}

void Graph::remove(Node node) noexcept {
  Degree deg = degree(node);
  Node other = Node(0);
  while (deg && (other = first_set(node, other)) != NoPos) {
    set(node, other, false);
    --deg;
    ++other;
  }
}

bool Graph::dup(const Graph &other) noexcept {
  if (this == &other) {
    return true;
  }
  if (!reset(other.size())) {
    return false;
  }
  bits_.copy(other.bits_);     // noexcept
  degree_.copy(other.degree_); // noexcept
  return true;
}

} // namespace onestl
