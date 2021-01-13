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
#include "onejit/code.hpp"

namespace onejit {

static bool is_direct(CodeItem item) {
  return item == 0 || (item & 3) != 0;
}

static NodeHeader direct_header(CodeItem item) {
  Type t = BAD;
  Kind k = Void;
  if (item <= FALLTHROUGH) {
    t = Type(item);
  } else if ((item & 1) == 1) {
    t = CONST;
    k = Kind((item >> 1) & 0x7F);
  } else {
    t = REG;
    k = Kind((item >> 2) & 0x7F);
  }
  return NodeHeader(t, k, 0);
}

static uint32_t direct_data(CodeItem item, NodeHeader header) {
  const Type t = header.type();
  uint32_t data;
  if (t <= FALLTHROUGH) {
    data = 0;
  } else if (t == CONST) {
    if (header.kind().is_unsigned()) {
      data = item >> 8;
    } else {
      // signed right shift is implementation-dependent: use a division
      data = int32_t(item & ~0xFF) / 256;
    }
  } else { // t == REG
    data = item >> 9;
  }
  return data;
}

Node Node::child(uint16_t i) const {
  check(i, <, children());
  const CodeItem item = code_->uint32(size_t(i) * sizeof(Offset) + offset_ + sizeof(NodeHeader));
  if (is_direct(item)) {
    NodeHeader header = direct_header(item);
    return Node(nullptr, direct_data(item, header), header);
  } else {
    const Offset offset = offset_ + item;
    return Node(code_, offset, NodeHeader(code_->uint32(offset)));
  }
}

} // namespace onejit
