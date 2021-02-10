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

  optimize_expr_T<int8_t>();
  optimize_expr_T<int16_t>();
  optimize_expr_T<int32_t>();
  optimize_expr_T<int64_t>();
  optimize_expr_T<uint8_t>();
  optimize_expr_T<uint16_t>();
  optimize_expr_T<uint32_t>();
  optimize_expr_T<uint64_t>();
  optimize_expr_T<float>();
  optimize_expr_T<double>();
}

template <class T> void Test::optimize_expr_T() {
  Func &f = func;

  Const one{f, T(1)};
  Kind kind = one.kind();
  Var x{f, kind};

  // optimize() on --x should return x
  Expr expr = Unary{f, NEG1, Unary{f, NEG1, x}};
  Node optimized = opt.optimize(f, expr);
  TEST(optimized, !=, expr);
  TEST(optimized, ==, x);

  // optimize() on 1+x should return x+1
  expr = Binary(f, ADD, one, x);
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
}

} // namespace onejit
