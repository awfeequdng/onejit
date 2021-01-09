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
 *  Created on Jan 08, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ARCHID_HPP
#define ONEJIT_ARCHID_HPP

#include <onejit/chars.hpp>

#include <cstdint> // uint8_t
#include <iosfwd>

namespace onejit {

class ArchId {
public:
  constexpr explicit ArchId(uint8_t archid) : val_(archid) {
  }

  const Chars &string() const;

  constexpr uint8_t val() const {
    return val_;
  }

private:
  uint8_t val_;
};

std::ostream &operator<<(std::ostream &out, ArchId archid);

constexpr const ArchId NOARCH(0);
constexpr const ArchId AMD64(1);
constexpr const ArchId ARM64(2);
constexpr const ArchId ARM(3);
constexpr const ArchId X86(4);

}; // namespace onejit

#endif // ONEJIT_ARCH_HPP
