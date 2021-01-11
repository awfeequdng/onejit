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
 * node.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ARG_HPP
#define ONEJIT_ARG_HPP

#include <onejit/code.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Node {
public:
  constexpr Node() : code_(nullptr), offset_(0), header_(BAD) {
  }

  Type type() const {
    return Type(header_);
  }

  uint16_t children() const;
  Node child(uint16_t i) const;

protected:
  Node(Code *code, Offset byte_offset);

  constexpr Node(Code *code, Offset byte_offset, NodeHeader header)
      : code_(code), offset_(byte_offset), header_(header) {
  }

  constexpr Code *code() const {
    return code_;
  }

  constexpr Offset offset() const {
    return offset_;
  }

  constexpr NodeHeader header() const {
    return header_;
  }

private:
  Code *code_;
  Offset offset_;
  NodeHeader header_;
};

} // namespace onejit

#endif // ONEJIT_ARG_HPP
