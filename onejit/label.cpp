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
 * label.cpp
 *
 *  Created on Jan 21, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/label.hpp>

namespace onejit {

static constexpr uint16_t trivial_hash(uint32_t val) noexcept {
  return uint16_t(val ^ (val >> 16)) | 1;
}

static constexpr uint16_t trivial_hash(uint64_t val) noexcept {
  return trivial_hash(uint32_t(val ^ (val >> 32)));
}

Label::Label(Func &func) noexcept : Base{func.new_label()} {
}

Label Label::create(Code *holder, uint64_t address, uint16_t index) noexcept {
  while (holder) {
    if (!index && address) {
      index = trivial_hash(address);
    }
    const NodeHeader header{LABEL, Void, index};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint64(address)) {
      return Label{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return Label{};
}

std::ostream &operator<<(std::ostream &out, const Label &l) {
  return out << l.type() << '_' << l.index();
}

} // namespace onejit
