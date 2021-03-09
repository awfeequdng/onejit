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
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/local.hpp>
#include <onejit/ir/childrange.hpp>
#include <onejit/ir/header.hpp>

#include <cstring>

namespace onejit {

Code::Code() noexcept : Base{} {
  if (reserve(64)) {
    init();
  }
}

Code::Code(size_t capacity) noexcept : Base{} {
  if (reserve(capacity)) {
    init();
  }
}

Code::~Code() noexcept {
  static_assert(sizeof(Header) == 4, "sizeof(Header) must be 4");
  static_assert(sizeof(CodeItem) == 4, "sizeof(CodeItem) must be 4");
  static_assert(sizeof(Offset) == 4, "sizeof(Offset) must be 4");
  static_assert(sizeof(uint32_t) == 4, "sizeof(uint32_t) must be 4");
  static_assert(sizeof(uint64_t) == 8, "sizeof(uint32_t) must be 8");
  static_assert(sizeof(float) == 4, "sizeof(double) must be 4");
  static_assert(sizeof(double) == 8, "sizeof(double) must be 8");
}

Code &Code::init() noexcept {
  // add magic signature {CONST uint8x4 "1JIT"} in case Code is saved to file
  return add(Header{CONST, Uint8.simdn(4), 0}) //
      .add_uint32(0x54494A31);
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

Code &Code::add_item(const CodeItem item) noexcept {
  return add(CodeItems{&item, 1});
}

Code &Code::add_uint64(uint64_t u64) noexcept {
  const union {
    uint64_t u64;
    uint32_t u32[2];
  } x = {u64};
  return add(CodeItems{x.u32, 2});
}

ONEJIT_NOINLINE Code &Code::add(CodeItems data) noexcept {
  Base::append(data);
  return *this;
}

Code &Code::add(const Kind kind) noexcept {
  // save Kind wrapped in a direct Var:
  // any generic parser will be able to recognize it
  return add_item(Local{kind, Id{}}.direct());
}

Code &Code::add(const Kinds kinds) noexcept {
  for (const Kind kind : kinds) {
    if (!add(kind)) {
      break;
    }
  }
  return *this;
}

Code &Code::add(View<char> chars) noexcept {
  const char *addr = chars.data();
  size_t n = chars.size();
  if (n >= sizeof(CodeItem)) {
    if (!add(CodeItems{reinterpret_cast<const CodeItem *>(addr), n / sizeof(CodeItem)})) {
      return *this;
    }
    const size_t left = n % sizeof(CodeItem);
    addr += n - left;
    n = left;
  }
  if (n == 0) {
    return *this;
  }
  // pad to a whole CodeItem adding one or more '\0'
  union {
    CodeItem item;
    char ch[sizeof(CodeItem)];
  } u = {0};
  std::memcpy(u.ch, addr, n);
  return add_item(u.item);
}

Code &Code::add(const Node &node, Offset parent_offset) noexcept {
  Offset offset = node.offset_or_direct();
  if (!node.is_direct()) {
    // save relative offset between parent and child:
    // makes it easier to concatenate different Code objects
    offset -= parent_offset;
  }
  return add_item(offset);
}

Code &Code::add_ranges(const ChildRanges &nodes, Offset parent_offset) noexcept {
  for (size_t i = 0, ni = nodes.size(); i < ni; i++) {
    ChildRange range = nodes[i];
    for (size_t j = 0, nj = range.size(); j < nj; j++) {
      if (!add(range[j], parent_offset)) {
        return *this;
      }
    }
  }
  return *this;
}

} // namespace onejit
