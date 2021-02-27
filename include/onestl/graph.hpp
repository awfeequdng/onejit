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
 * graph.hpp
 *
 *  Created on Feb 27, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_GRAPH_HPP
#define ONESTL_GRAPH_HPP

#include <onestl/bitset.hpp>
#include <onestl/mem.hpp>

#include <utility> // std::move(), std::swap()

namespace onestl {

/** undirected, dense graph */
class Graph {

public:
  constexpr Graph() noexcept : bits_(), n_(0) {
  }

  explicit Graph(size_t nodes) noexcept //
      : bits_(nodes * (nodes + 1) / 2), n_(nodes) {
  }

  Graph(const Graph &other) = delete;

  Graph(Graph &&other) noexcept //
      : bits_(std::move(other.bits_)), n_(other.n_) {
    other.n_ = 0;
  }

  ~Graph() noexcept = default;

  Graph &operator=(const Graph &other) = delete;
  Graph &operator=(Graph &&other) noexcept {
    swap(other);
    return *this;
  }

  constexpr size_t size() const noexcept {
    return n_;
  }

  // resize Graph and remove all edges
  bool reset(size_t nodes) noexcept;

  // checked access:
  // return true if i-th and j-th nodes are connected,
  // or false if i or j are out of bounds
  bool operator()(size_t i, size_t j) const noexcept;

  // add or remove an edge betwen nodes j and j.
  // does nothing if i or j are out of bounds
  void set(size_t i, size_t j, bool value) noexcept;

  void swap(Graph &other) noexcept {
    std::swap(n_, other.n_);
    bits_.swap(other.bits_);
  }

private:
  BitSet bits_;
  size_t n_;
};

inline void swap(Graph &left, Graph &right) noexcept {
  left.swap(right);
}

} // namespace onestl

#endif // ONESTL_GRAPH_HPP
