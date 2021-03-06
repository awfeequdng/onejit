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
 * test_regallocator.cpp
 *
 *  Created on Mar 04, 2021
 *      Author Massimiliano Ghilardi
 */

#include "test.hpp"

#include <onejit/regallocator.hpp>

#include <cstdio>

namespace onejit {

using Color = RegAllocator::Color;
using Reg = RegAllocator::Reg;

static void to_string(String &result, View<Color> colors) {
  result.clear();
  Fmt fmt{&result};
  for (Color color : colors) {
    fmt << color << ' ';
  }
}

static void run_allocator(String &result, RegAllocator &allocator, Color num_colors) {
  allocator.allocate_regs(num_colors);
  to_string(result, allocator.get_colors());
}

void Test::regallocator() {
  enum : size_t { nreg = 10 };
  RegAllocator allocator{nreg};
  Graph &graph = allocator.graph();
  // connect registers 0..4 together,
  for (Reg r1 = 0; r1 <= 4; r1++) {
    for (Reg r2 = 0; r2 <= 4; r2++) {
      graph.set(r1, r2, true);
    }
  }
  // and connect registers 5..9 together
  for (Reg r1 = 5; r1 <= 9; r1++) {
    for (Reg r2 = 5; r2 <= 9; r2++) {
      graph.set(r1, r2, true);
    }
  }
  // also connect register 4 with 5
  graph.set(Reg(4), Reg(5), true);
  String result;
  run_allocator(result, allocator, Color(5));
  Chars expected = "4 3 2 1 0 4 3 2 1 0 ";
  TEST(result, ==, expected);

  // also connect register 2..7 together
  for (Reg r1 = 2; r1 <= 7; r1++) {
    for (Reg r2 = 2; r2 <= 7; r2++) {
      graph.set(r1, r2, true);
    }
  }
  // and try again
  run_allocator(result, allocator, Color(5));
  expected = "1 0 5 4 3 2 1 0 4 3 ";
  TEST(result, ==, expected);

  // retry with fewer available registers
  run_allocator(result, allocator, Color(4));
  expected = "3 2 5 1 0 4 3 2 1 0 ";
  TEST(result, ==, expected);

  // retry with fewer available registers
  run_allocator(result, allocator, Color(3));
  expected = "2 1 6 5 0 4 3 2 1 0 ";
  TEST(result, ==, expected);

  // retry with fewer available registers
  run_allocator(result, allocator, Color(2));
  expected = "1 0 7 6 5 4 3 2 1 0 ";
  TEST(result, ==, expected);

  // and with more available registers
  run_allocator(result, allocator, Color(6));
  expected = "3 2 5 1 0 4 3 2 1 0 ";
  TEST(result, ==, expected);
}

} // namespace onejit
