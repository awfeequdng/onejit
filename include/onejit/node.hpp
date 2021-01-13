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

#ifndef ONEJIT_NODE_HPP
#define ONEJIT_NODE_HPP

#include <onejit/nodeheader.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// base interface of BinaryExpr, UnaryExpr, Stmt*
class Node : private NodeHeader {
  using Base = NodeHeader;

  friend class Reg;
  friend class Stmt0;

public:
  constexpr Node() : Base(BAD), offset_(0), code_(nullptr) {
  }

  using Base::kind;
  using Base::type;

  constexpr uint16_t children() const {
    return code_ == nullptr ? 0 : type() == UNARY ? 1 : type() == BINARY ? 2 : Base::data();
  }

  Node child(uint16_t i) const;

protected:
  constexpr Node(Code *code, Offset byte_offset, NodeHeader header)
      : Base(header), offset_(byte_offset), code_(code) {
  }

  constexpr Code *code() const {
    return code_;
  }

  constexpr Offset offset() const {
    return offset_;
  }

  constexpr NodeHeader header() const {
    return *this;
  }

private:
  Offset offset_;
  Code *code_;
};

} // namespace onejit

#endif // ONEJIT_ARG_HPP
