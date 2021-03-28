/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * name.cpp
 *
 *  Created on Jan 21, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/ir/name.hpp>
#include <onestl/chars.hpp>

namespace onejit {
namespace ir {

Node Name::create(Code *holder, Chars str) noexcept {
  const size_t n = str.size();
  while (holder && n <= 0xFFFF) {
    const Header header{NAME, Void, uint16_t(n)};
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
    const Offset start = add_uint32(Base::offset_or_direct(), 4);
    const Offset end = code->length();
    const Offset len = size();
    if (end >= start && end - start >= (len + 3) / 4) {
      return Chars{reinterpret_cast<const char *>(code->data()) + start, len};
    }
  }
  return Chars{};
}

const Fmt &Name::format(const Fmt &fmt, Syntax /*syntax*/, size_t /*depth*/) const {
  return fmt << '"' << chars() << '"';
}

} // namespace ir
} // namespace onejit
