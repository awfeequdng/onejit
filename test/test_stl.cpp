/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test_stl.cpp
 *
 *  Created on Feb 27, 2021
 *      Author Massimiliano Ghilardi
 */

#include "test.hpp"

#include <onestl/bitset.hpp>
#include <onestl/graph.hpp>
#include <onestl/io/reader_chars.hpp>
#include <onestl/io/scan.hpp>

namespace onejit {

void Test::stl_bitset() {
  size_t i, lo, hi, n = 3 * 8 * sizeof(size_t);
  BitSet s{n};
  for (i = 0; i < n; i++) {
    TEST(s[i], ==, false);
    TEST(s.find(true, i), ==, BitSet::NoPos);
  }
  TEST(s[n], ==, false); // test out of bounds [n]

  s.fill(true, lo = 3, hi = n - 3);
  for (i = 0; i < lo; i++) {
    TEST(s[i], ==, false);
    TEST(s.find(true, i), ==, lo);
  }
  for (i = lo; i < hi; i++) {
    TEST(s[i], ==, true);
    TEST(s.find(true, i), ==, i);
  }
  for (i = hi; i < n; i++) {
    TEST(s[i], ==, false);
    TEST(s.find(true, i), ==, BitSet::NoPos);
  }

  TEST(s[n], ==, false); // test out of bounds [n]

  for (i = 0; i < n; i++) {
    s.set(i, i & 1);
    TEST(s[i], ==, i & 1);
  }
}

void Test::stl_graph() {
  Graph::Node a, b, n = 14;
  Graph g{n};
  for (a = 0; a < n; a++) {
    TEST(g.degree(a), ==, 0);
    for (b = 0; b < n; b++) {
      TEST(g(a, b), ==, false);
    }
  }
  for (a = 0; a < n; a++) {
    for (b = 0; b <= a; b++) {
      Graph::Degree degree_a = g.degree(a);
      Graph::Degree degree_b = g.degree(b);

      TEST(g(a, b), ==, false);
      TEST(g(b, a), ==, false);
      TEST(g.first_set(a, b), ==, Graph::NoPos);
      TEST(g.first_set(b, a), ==, Graph::NoPos);

      g.set(a, b, true);

      TEST(g(a, b), ==, true);
      TEST(g(b, a), ==, true);
      TEST(g.first_set(a, b), ==, b);
      TEST(g.first_set(b, a), ==, a);

      if (a == b) {
        TEST(g.degree(a), ==, degree_a + 2);
      } else {
        TEST(g.degree(a), ==, degree_a + 1);
        TEST(g.degree(b), ==, degree_b + 1);
      }
    }
  }

  for (a = 0; a < n; a++) {
    for (b = 0; b <= a; b++) {
      Graph::Degree degree_a = g.degree(a);
      Graph::Degree degree_b = g.degree(b);

      TEST(g(a, b), ==, true);
      TEST(g(b, a), ==, true);
      TEST(g.first_set(a, b), ==, b);
      TEST(g.first_set(b, a), ==, a);

      g.set(a, b, false);

      TEST(g(a, b), ==, false);
      TEST(g(b, a), ==, false);
      TEST(g.first_set(a, b), >, b);
      TEST(g.first_set(b, a), >, a);

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
    TEST(g.first_set(a), ==, Graph::NoPos);
    for (b = 0; b < n; b++) {
      TEST(g(a, b), ==, false);
    }
  }

  for (a = 0; a < n; a++) {
    for (b = 0; b <= a; b++) {
      g.set(a, b, true);
    }
  }
  for (a = 0; a < n; a++) {
    TEST(g.degree(a), !=, 0);
    TEST(g.first_set(a), !=, Graph::NoPos);
    g.remove(a);
    TEST(g.degree(a), ==, 0);
    TEST(g.first_set(a), ==, Graph::NoPos);
  }
}

void Test::stl_scan() {
  Chars chars = " \t\n-1234567890";
  Scan scan{&chars};
  TEST(bool(scan), ==, true);

  int64_t i = 0;
  scan >> i;
  TEST(bool(scan), ==, true);
  TEST(scan.eof(), ==, true);
  TEST(i, ==, -1234567890);
}

} // namespace onejit
