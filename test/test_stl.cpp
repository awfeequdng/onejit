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
 * test_stl.cpp
 *
 *  Created on Feb 27, 2021
 *      Author Massimiliano Ghilardi
 */

#include "test.hpp"

#include <onestl/bitset.hpp>

namespace onejit {

void Test::stl() {
  size_t i, x, y, n = 3 * 8 * sizeof(size_t);
  BitSet s{n};
  for (i = 0; i < n; i++) {
    TEST(s[i], ==, false);
  }
  TEST(s[n], ==, false); // test out of bounds [n]

  s.fill(x = 3, y = n - 3, true);
  for (i = 0; i < n; i++) {
    TEST(s[i], ==, i >= x && i < y);
  }
  TEST(s[n], ==, false); // test out of bounds [n]

  for (i = 0; i < n; i++) {
    s.set(i, i & 1);
    TEST(s[i], ==, i & 1);
  }
}

} // namespace onejit
