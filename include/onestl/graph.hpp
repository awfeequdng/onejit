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

#include <onestl/array.hpp>
#include <onestl/bitset.hpp>
#include <onestl/mem.hpp>

namespace onestl {

/** undirected, dense graph */
class Graph {

public:
  typedef uint32_t Degree;
  typedef size_t Node;
  enum : size_t { NoPos = BitSet::NoPos };

  constexpr Graph() noexcept : bits_(), degree_() {
  }

  explicit Graph(size_t nodes) noexcept //
      : bits_(nodes * nodes), degree_(nodes) {
  }

  Graph(const Graph &other) = delete;
  Graph(Graph &&other) noexcept = default;

  ~Graph() noexcept = default;

  Graph &operator=(const Graph &other) = delete;
  Graph &operator=(Graph &&other) noexcept {
    swap(other);
    return *this;
  }

  constexpr size_t size() const noexcept {
    return degree_.size();
  }

  // resize Graph and remove all edges
  bool reset(size_t nodes) noexcept;

  // return true if nodes a and b are connected,
  // or false if a or b are out of bounds
  bool operator()(Node a, Node b) const noexcept;

  // add or remove an edge betwen nodes a and b.
  // does nothing if a or b are out of bounds
  void set(Node a, Node b, bool value) noexcept;

  // return number of edges connected to specified node.
  constexpr Degree degree(Node node) const noexcept {
    return degree_[node];
  }

  // search among edges of specified node, and return first connected node >= first_neighbor.
  // return NoPos if node has no edges connecting to nodes >= first_neighbor
  Node first_set(Node node, Node first_neighbor = Node(0)) const noexcept;

  // remove specified node and all its edges
  void remove(Node node) noexcept;

  // copy content of other Graph into this graph.
  // return false if out of memory.
  bool dup(const Graph &other) noexcept;

  void swap(Graph &other) noexcept {
    bits_.swap(other.bits_);
    degree_.swap(other.degree_);
  }

private:
  BitSet bits_;
  Array<Degree> degree_;
};

inline void swap(Graph &left, Graph &right) noexcept {
  left.swap(right);
}

} // namespace onestl

#endif // ONESTL_GRAPH_HPP
