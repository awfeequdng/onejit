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
 * local.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/local.hpp>
#include <onestl/chars.hpp>

#include <ios>
#include <ostream>

namespace onejit {

std::ostream &operator<<(std::ostream &out, LocalId id) {
  return out << Chars("id") << std::hex << id.val() << std::dec;
}

std::ostream &operator<<(std::ostream &out, Local l) {
  return out << Chars("var") << std::hex << l.id().val() << std::dec << '_'
             << l.kind().stringsuffix();
}

} // namespace onejit
