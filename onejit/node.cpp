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
 * node.cpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#include "onejit/node.hpp"
#include "onejit/check.hpp"

namespace onejit {

Node::Node(Code *code, Offset byte_offset) : code_(code), offset_(byte_offset), header_(BAD) {
  if (code) {
    header_ = code->at(byte_offset);
  }
}

uint16_t Node::children() const {
  return header_ >> 16;
}

Node Node::child(uint16_t i) const {
  check(i, <, children());
  Offset offset =
      offset_ + code_->uint32(size_t(i) * sizeof(Offset) + offset_ + sizeof(NodeHeader));
  return Node(code_, offset);
}

} // namespace onejit
