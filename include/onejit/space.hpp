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
 * space.hpp
 *
 *  Created on Feb 06, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_SPACE_HPP
#define ONEJIT_SPACE_HPP

#include <onejit/fwd.hpp>

namespace onejit {

class Space {
public:
  constexpr explicit Space(size_t size) noexcept : size_{size} {
  }

  constexpr size_t size() const noexcept {
    return size_;
  }

private:
  size_t size_;
};

const Fmt &operator<<(const Fmt &out, Space space);

} // namespace onejit

#endif // ONEJIT_SPACE_HPP
