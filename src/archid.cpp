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
 * archid.cpp
 *
 *  Created on Jan 28, 2020
 *      Author Massimiliano Ghilardi
 */

#include "archid.hpp"

#include <ostream>

namespace onejit {

std::string to_string(ArchId archid) {
  const char* s = nullptr;
  switch (archid) {
    case NOARCH:
    default:
      s = "NOARCH";
      break;
    case AMD64:
      s = "AMD64";
      break;
    case ARM64:
      s = "ARM64";
      break;
    case ARM:
      s = "ARM";
      break;
    case X86:
      s = "X86";
      break;
  }
  return std::string(s);
}

std::ostream& operator<<(std::ostream& out, ArchId archid) {
  return out << to_string(archid);
}

}  // namespace onejit
