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
#include <onestl/graph.hpp>

namespace onejit {

void Test::stl_bitset() {
  size_t i, lo, hi, n = 3 * 8 * sizeof(size_t);
  BitSet s{n};
  for (i = 0; i < n; i++) {
    TEST(s[i], ==, false);
  }
  TEST(s[n], ==, false); // test out of bounds [n]

  s.fill(lo = 3, hi = n - 3, true);
  for (i = 0; i < n; i++) {
    TEST(s[i], ==, i >= lo && i < hi);
  }
  TEST(s[n], ==, false); // test out of bounds [n]

  for (i = 0; i < n; i++) {
    s.set(i, i & 1);
    TEST(s[i], ==, i & 1);
  }
}

void Test::stl_graph() {
  size_t a, b, n = 32;
  Graph g{n};
  for (a = 0; a < n; a++) {
    TEST(g.degree(a), ==, 0);
    for (b = 0; b < n; b++) {
      TEST(g(a, b), ==, false);
    }
  }
  for (a = 0; a < n; a++) {
    for (b = a; b < n; b++) {
      size_t degree_a = g.degree(a);
      size_t degree_b = g.degree(b);

      TEST(g(a, b), ==, false);
      TEST(g(b, a), ==, false);
      g.set(a, b, true);
      TEST(g(a, b), ==, true);
      TEST(g(b, a), ==, true);
      if (a == b) {
        TEST(g.degree(a), ==, degree_a + 2);
      } else {
        TEST(g.degree(a), ==, degree_a + 1);
        TEST(g.degree(b), ==, degree_b + 1);
      }
    }
  }

  for (a = 0; a < n; a++) {
    for (b = a; b < n; b++) {
      size_t degree_a = g.degree(a);
      size_t degree_b = g.degree(b);

      TEST(g(a, b), ==, true);
      TEST(g(b, a), ==, true);
      g.set(a, b, false);
      TEST(g(a, b), ==, false);
      TEST(g(b, a), ==, false);
      if (a == b) {
        TEST(g.degree(a), ==, degree_a - 2);
      } else {
        TEST(g.degree(a), ==, degree_a - 1);
        TEST(g.degree(b), ==, degree_b - 1);
      }
    }
  }

  for (a = 0; a < n; a++) {
    TEST(g.degree(a), ==, 0);
    for (b = 0; b < n; b++) {
      TEST(g(a, b), ==, false);
    }
  }
}

} // namespace onejit
