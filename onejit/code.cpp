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
 * code.cpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/nodeheader.hpp>
#include <onejit/var.hpp>

#include <cstring>

namespace onejit {

Code::Code() noexcept : Base{}, good_{true} {
}

Code::Code(size_t capacity) noexcept : Base{}, good_{true} {
  good_ &= reserve(capacity);
}

Code::~Code() noexcept {
  static_assert(sizeof(NodeHeader) == 4, "sizeof(NodeHeader) must be 4");
  static_assert(sizeof(CodeItem) == 4, "sizeof(CodeItem) must be 4");
  static_assert(sizeof(Offset) == 4, "sizeof(Offset) must be 4");
  static_assert(sizeof(uint32_t) == 4, "sizeof(uint32_t) must be 4");
  static_assert(sizeof(uint64_t) == 8, "sizeof(uint32_t) must be 8");
  static_assert(sizeof(float) == 4, "sizeof(double) must be 4");
  static_assert(sizeof(double) == 8, "sizeof(double) must be 8");
}

uint64_t Code::uint64(Offset byte_offset) const noexcept {
  union {
    uint64_t u64;
    uint32_t u32[2];
  } x = {0};
  const size_t index = byte_offset / sizeof(T);
  if (index + 1 < size()) {
    std::memcpy(&x, data() + index, sizeof(x));
  }
  return x.u64;
}

Code &Code::add(const uint32_t item) noexcept {
  return add(CodeItems{&item, 1});
}

Code &Code::add(uint64_t u64) noexcept {
  const union {
    uint64_t u64;
    uint32_t u32[2];
  } x = {u64};
  return add(CodeItems{x.u32, 2});
}

Code &ONEJIT_NOINLINE Code::add(CodeItems data) noexcept {
  good_ = good_ && Base::append(data);
  return *this;
}

Code &Code::add(const Kind kind) noexcept {
  // save Kind wrapped in a direct VarExpr:
  // any generic parser will be able to recognize it
  return add(Var{kind, VarId{}}.direct());
}

Code &Code::add(const Kinds kinds) noexcept {
  for (const Kind kind : kinds) {
    if (!add(kind)) {
      break;
    }
  }
  return *this;
}

Code &Code::add(const Node &node, Offset parent_offset) noexcept {
  Offset offset = node.offset_or_direct();
  if (!node.is_direct()) {
    // save relative offset between parent and child:
    // makes it easier to concatenate different Code objects
    offset -= parent_offset;
  }
  return add(offset);
}

Code &ONEJIT_NOINLINE Code::add(const Nodes nodes, Offset parent_offset) noexcept {
  for (const Node &node : nodes) {
    if (!add(node, parent_offset)) {
      break;
    }
  }
  return *this;
}

} // namespace onejit
