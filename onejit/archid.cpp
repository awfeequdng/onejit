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
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/archid.hpp>
#include <onejit/fmt.hpp>

namespace onejit {

static const char archstring[] = //
    "\6NOARCH\0"
    "\3X64\0\0\0\0"
    "\3X86\0\0\0\0"
    "\5ARM64\0\0"
    "\3ARM";

Chars to_string(ArchId archid) noexcept {
  size_t i = 0;
  if (archid < ARCHID_N) {
    i = archid;
  }
  const char *str = &archstring[i * 8];
  return Chars{str + 1, uint8_t(str[0])};
}

const Fmt &operator<<(const Fmt &fmt, ArchId archid) {
  return fmt << to_string(archid);
}

} // namespace onejit
