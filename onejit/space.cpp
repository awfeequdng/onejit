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
 * space.cpp
 *
 *  Created on Feb 06, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/fmt.hpp>
#include <onejit/space.hpp>
#include <onestl/chars.hpp>

namespace onejit {

static const Chars str = "                                                                "
                         "                                                                "
                         "                                                                "
                         "                                                                ";

const Fmt &operator<<(const Fmt &fmt, Space space) {
  const size_t n = str.size();
  size_t left = space.size();

  while (left >= n) {
    fmt << str;
    left -= n;
  }
  if (left) {
    fmt << str.view(0, left);
  }
  return fmt;
}

} // namespace onejit
