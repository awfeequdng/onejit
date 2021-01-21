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
 *  Created on Jan 21, 2020
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/label.hpp>

namespace onejit {

Label Label::create(uint16_t index, Code *holder) noexcept {
  const NodeHeader header{LABEL, Void, index};

  while (holder) {
    CodeItem offset = holder->length();

    // destination address not known yet: set to zero
    if (holder->add(header) && holder->add(uint32_t(0))) {
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
