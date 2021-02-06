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
 * name.cpp
 *
 *  Created on Jan 21, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/name.hpp>
#include <onestl/chars.hpp>

namespace onejit {

Node Name::create(Code *holder, Chars str) noexcept {
  const size_t n = str.size();
  while (holder && n <= 0xFFFF) {
    const NodeHeader header{NAME, Void, uint16_t(n)};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add(str)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Node{};
}

Chars Name::chars() const noexcept {
  if (const Code *code = Base::code()) {
    const Offset start = sum_uint32(Base::offset_or_direct(), 4);
    const Offset end = code->length();
    const Offset len = size();
    if (end >= start && end - start >= (len + 3) / 4) {
      return Chars{reinterpret_cast<const char *>(code->data()) + start, len};
    }
  }
  return Chars{};
}

const Fmt &Name::format(const Fmt &out, size_t /*depth*/) const {
  return out << '"' << chars() << '"';
}

} // namespace onejit
