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
 * group.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_GROUP_HPP
#define ONEJIT_GROUP_HPP

#include <onejit/chars.hpp>

#include <cstdint> // uint8_t
#include <iosfwd>  // std::ostream

namespace onejit {

// intentionally match Go reflect.Kind values
enum Group : uint8_t {
  gVoid = 0,
  gBool = 1,
  gInt = 2,
  gUint = 7,
  gFloat = 14,
  gComplex = 16,
  gPtr = 22,
  gArch = 27, // arch-specific kind
};

const Chars &to_string(Group g);

std::ostream &operator<<(std::ostream &out, Group g);

} // namespace onejit

#endif // ONEJIT_GROUP_HPP
