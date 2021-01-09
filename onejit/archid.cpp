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
 *  Created on Jan 08, 2020
 *      Author Massimiliano Ghilardi
 */

#include "onejit/archid.hpp"

#include <ostream>

namespace onejit {

static const std::string archstring[] = {
    "NOARCH", "AMD64", "ARM64", "ARM", "X86",
};

const std::string &ArchId::string() const {
  uint8_t i = val_;
  if (i >= sizeof(archstring) / sizeof(archstring[0])) {
    i = 0;
  }
  return archstring[i];
}

std::ostream &operator<<(std::ostream &out, ArchId archid) {
  return out << archid.string();
}

} // namespace onejit
