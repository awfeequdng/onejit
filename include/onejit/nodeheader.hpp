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
 * nodeheader.hpp
 *
 *  Created on Jan 12, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODEHEADER_HPP
#define ONEJIT_NODEHEADER_HPP

#include <onejit/fwd.hpp>
#include <onejit/kind.hpp>
#include <onejit/type.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// first CodeItem contained in BinaryExpr, UnaryExpr, Stmt*
class NodeHeader {
  friend class Node;

public:
  constexpr NodeHeader() : type_(BAD), kind_(Void), data_(0) {
  }

  constexpr explicit NodeHeader(CodeItem item)
      : type_(Type(item & 0xFF)), kind_(Kind(item >> 8)), data_(item >> 16) {
  }

  constexpr NodeHeader(Type type, Kind kind, uint16_t data)
      : type_(type), kind_(kind), data_(data) {
  }

  constexpr Type type() const {
    return type_;
  }

  constexpr Kind kind() const {
    return kind_;
  }

  constexpr uint32_t data() const {
    return data_;
  }

private:
  Type type_;
  Kind kind_;
  uint16_t data_;
};

} // namespace onejit

#endif // ONEJIT_ARG_HPP
