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
 * reg.cpp
 *
 *  Created on Jan 13, 2020
 *      Author Massimiliano Ghilardi
 */

#include "onejit/reg.hpp"
#include "onejit/chars.hpp"

#include <ios>
#include <ostream>

namespace onejit {

std::ostream &operator<<(std::ostream &out, RegId regid) {
  return out << Chars("rid") << std::hex << regid.val() << std::dec;
}

std::ostream &operator<<(std::ostream &out, Reg reg) {
  return out << Chars("reg") << std::hex << reg.regid().val() << std::dec << '_'
             << reg.kind().stringsuffix();
}

} // namespace onejit
