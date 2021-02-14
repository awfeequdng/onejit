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
 * test_eval.cpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include "test.hpp"

#include <onejit/eval.hpp>
#include <onejit/node.hpp>

namespace onejit {

void Test::optimize_expr() {
  func.reset(&holder, Name{&holder, "optimize_expr"}, FuncType{&holder, {}, {}});

  for (Kind kind : {Int8, Int16, Int32, Int64,     //
                    Uint8, Uint16, Uint32, Uint64, //
                    Float32, Float64}) {
    optimize_expr_kind(kind);
  }
}

void Test::optimize_expr_kind(Kind kind) {
  Func &f = func;

  Const one = One(f, kind);
  Const two = Two(f, kind);
  Var x{f, kind};

  // optimize() on --x should return x
  Expr expr = Unary{f, NEG1, Unary{f, NEG1, x}};
  Node optimized = opt.optimize(f, expr);
  TEST(optimized, !=, expr);
  TEST(optimized, ==, x);

  // optimize() on 1+x should return x+1
  expr = Binary(f, ADD2, one, x);
  optimized = opt.optimize(f, expr);
  String expected;
  Fmt{&expected} << "(+ " << x << " 1)";
  TEST(optimized, !=, expr);
  TEST(to_string(optimized), ==, expected);

  // optimize() on 1<x should return x>1
  expr = Binary(f, LSS, one, x);
  optimized = opt.optimize(f, expr);
  expected.clear();
  Fmt{&expected} << "(> " << x << " 1)";
  TEST(optimized, !=, expr);
  TEST(to_string(optimized), ==, expected);

  // optimize() on !(1<x) should return x<=1
  expr = Unary{f, NOT1, expr};
  optimized = opt.optimize(f, expr);
  expected.clear();
  Fmt{&expected} << "(<= " << x << " 1)";
  TEST(optimized, !=, expr);
  TEST(to_string(optimized), ==, expected);

  if (kind.is_signed()) {
    // optimize() on x-1 should return x+(-1)
    expr = Binary{f, SUB, x, one};
    optimized = opt.optimize(f, expr);
    expected.clear();
    Fmt{&expected} << "(+ " << x << " -1)";
    TEST(optimized, !=, expr);
    TEST(to_string(optimized), ==, expected);
  }

  // optimize() on (x+1)+2 should return x+3
  expr = Binary{f, ADD2, Binary{f, ADD2, x, one}, two};
  optimized = opt.optimize(f, expr);
  expected.clear();
  Fmt{&expected} << "(+ " << x << " 3)";
  TEST(optimized, !=, expr);
  TEST(to_string(optimized), ==, expected);

  // optimize() on (x+1)+(x+2) should return (x+x)+3
  expr = Binary{f, ADD2, Binary{f, ADD2, x, one}, Binary{f, ADD2, x, two}};
  optimized = opt.optimize(f, expr);
  expected.clear();
  Fmt{&expected} << "(+ (+ " << x << ' ' << x << ") 3)";
  TEST(optimized, !=, expr);
  TEST(to_string(optimized), ==, expected);
}

} // namespace onejit
