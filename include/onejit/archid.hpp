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
 * archid.hpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ARCHID_HPP
#define ONEJIT_ARCHID_HPP

#include <onejit/fwd.hpp>

#include <cstdint> // uint8_t

namespace onejit {

enum ArchId : uint8_t {
  NOARCH = 0,
  X64 = 1, // alias for x86_64, amd64
  X86 = 2, // alias for i386, i486, i586, i686, ix86
  ARM64 = 3,
  ARM = 4,

  ARCHID_N,
};

Chars to_string(ArchId a) noexcept;

const Fmt &operator<<(const Fmt &fmt, ArchId archid);

} // namespace onejit

#endif // ONEJIT_ARCHID_HPP
