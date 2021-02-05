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

enum class eArchId : uint8_t {
  NOARCH = 0,
  X64 = 1, // alias for x86_64, amd64
  ARM64 = 2,
  X86 = 3, // 32-bit i386, i486, i586 ...
  ARM = 4,
};

class ArchId {
public:
  constexpr /*implicit*/ ArchId(eArchId val) noexcept : val_{val} {
  }

  Chars string() const noexcept;

  constexpr eArchId val() const noexcept {
    return val_;
  }

private:
  eArchId val_;
};

const Fmt &operator<<(const Fmt &out, ArchId archid);
inline const Fmt &operator<<(const Fmt &out, eArchId earchid) {
  return out << ArchId{earchid};
}

constexpr const ArchId NOARCH{eArchId::NOARCH};
constexpr const ArchId X64{eArchId::X64};
constexpr const ArchId ARM64{eArchId::ARM64};
constexpr const ArchId X86{eArchId::X86};
constexpr const ArchId ARM{eArchId::ARM};

}; // namespace onejit

#endif // ONEJIT_ARCHID_HPP
