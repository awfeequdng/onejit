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
#include <onejit/ir.hpp>

namespace onejit {

void Test::optimize() {
  func.reset(&holder, Name{&holder, "optimize"}, FuncType{&holder, {}, {}});

  for (Kind kind : {Int8, Int16, Int32, Int64,     //
                    Uint8, Uint16, Uint32, Uint64, //
                    Float32, Float64}) {
    optimize_expr_kind(kind);
    optimize_assign_kind(kind);
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
  expr = Tuple{f, ADD, one, x};
  optimized = opt.optimize(f, expr);
  String expected;
  Fmt{&expected} << "(+ " << x << " 1)";
  TEST(optimized, !=, expr);
  TEST(to_string(optimized), ==, expected);

  // optimize() on 1<x should return x>1
  expr = Binary(f, LSS, one, x);
  {
    optimized = opt.optimize(f, expr);
    expected.clear();
    Fmt{&expected} << "(> " << x << " 1)";
    TEST(optimized, !=, expr);
    TEST(to_string(optimized), ==, expected);
  }

  // optimize() on !(1<x) should return x<=1
  expr = Unary{f, NOT1, expr};
  {
    optimized = opt.optimize(f, expr);
    expected.clear();
    Fmt{&expected} << "(<= " << x << " 1)";
    TEST(optimized, !=, expr);
    TEST(to_string(optimized), ==, expected);
  }

  if (kind.is_signed()) {
    // optimize() on (x-1)-2 should return x+(-3)
    expr = Binary{f, SUB, Binary{f, SUB, x, one}, two};
    optimized = opt.optimize(f, expr);
    expected.clear();
    Fmt{&expected} << "(+ " << x << " -3)";
    TEST(optimized, !=, expr);
    TEST(to_string(optimized), ==, expected);
  }

  // optimize() on 1+(2+x) should return x+3
  expr = Tuple{f, ADD, one, Tuple{f, ADD, two, x}};
  {
    optimized = opt.optimize(f, expr);
    expected.clear();
    Fmt{&expected} << "(+ " << x << " 3)";
    TEST(optimized, !=, expr);
    TEST(to_string(optimized), ==, expected);
  }

  // optimize() on (x+1) + (-(-x)+2) should return x+x+3
  expr = Tuple{f, ADD, Tuple{f, ADD, x, one}, //
               Tuple{f, ADD, Unary{f, NEG1, Unary{f, NEG1, x}}, two}};
  {
    optimized = opt.optimize(f, expr);
    expected.clear();
    Fmt{&expected} << "(+ " << x << ' ' << x << " 3)";
    TEST(optimized, !=, expr);
    TEST(to_string(optimized), ==, expected);
  }

  // optimize() on ((x == 1) || true) should return true
  expr = Binary{f, LOR, Binary{f, EQL, x, one}, TrueExpr};
  {
    expected.clear();
    Fmt{&expected} << "(|| (== " << x << " 1) true)";
    TEST(to_string(expr), ==, expected);
    optimized = opt.optimize(f, expr);
    expected = "true";
    TEST(optimized, !=, expr);
    TEST(to_string(optimized), ==, expected);
  }

  // optimize() on ((x == 2) && true) should return (x == 2)
  expr = Binary{f, LAND, Binary{f, EQL, x, two}, TrueExpr};
  {
    expected.clear();
    Fmt{&expected} << "(&& (== " << x << " 2) true)";
    TEST(to_string(expr), ==, expected);
    optimized = opt.optimize(f, expr);
    TEST(optimized, !=, expr);
    TEST(optimized, ==, expr.child(0));
  }

  // optimize() on ((f() && true) should return f()
  expr = Binary{f, LAND, Call{f, f, {}}, TrueExpr};
  {
    expected.clear();
    Fmt{&expected} << "(&& (call label_0) true)";
    TEST(to_string(expr), ==, expected);
    optimized = opt.optimize(f, expr);
    TEST(optimized, !=, expr);
    TEST(optimized, ==, expr.child(0));
    expected = "(call label_0)";
    TEST(to_string(optimized), ==, expected);
  }

  // optimize() on (f() && (1 == 2)) should return (comma f() false)
  expr = Binary{f, LAND, Call{f, f, {}}, Binary{f, EQL, one, two}};
  {
    expected.clear();
    Fmt{&expected} << "(&& (call label_0) (== 1 2))";
    TEST(to_string(expr), ==, expected);
    optimized = opt.optimize(f, expr);
    TEST(optimized, !=, expr);
    expected = "(comma (call label_0) false)";
    TEST(to_string(optimized), ==, expected);
  }

  // optimize() on ((x + 1) == (x + 1)) should return true
  expr = Binary{f, EQL, Tuple{f, ADD, x, one}, Tuple{f, ADD, x, one}};
  {
    expected.clear();
    Fmt{&expected} << "(== (+ " << x << " 1) (+ " << x << " 1))";
    TEST(to_string(expr), ==, expected);
    optimized = opt.optimize(f, expr);
    TEST(optimized, !=, expr);
    TEST(optimized, ==, TrueExpr);
  }
}

void Test::optimize_assign_kind(Kind kind) {
  Func &f = func;

  Const one = One(f, kind);
  // Const two = Two(f, kind);
  Var x{f, kind};
  String expected;

  // optimize() on (= x x) should return void
  Assign st{f, ASSIGN, x, x};
  Node optimized = opt.optimize(f, st);
  TEST(optimized, ==, VoidExpr);

  // optimize() on (^= x x) should return (= x 0)
  st = Assign{f, XOR_ASSIGN, x, x};
  optimized = opt.optimize(f, st);
  expected.clear();
  Fmt{&expected} << "(= " << x << " 0)";
  TEST(to_string(optimized), ==, expected);

  // optimize() on (+= x x) should return (*= x 2)
  st = Assign{f, ADD_ASSIGN, x, x};
  optimized = opt.optimize(f, st);
  expected.clear();
  Fmt{&expected} << "(*= " << x << " 2)";
  TEST(to_string(optimized), ==, expected);

  // optimize() on (+= x 1) should return (++ x)
  st = Assign{f, ADD_ASSIGN, x, one};
  optimized = opt.optimize(f, st);
  expected.clear();
  Fmt{&expected} << "(++ " << x << ")";
  TEST(to_string(optimized), ==, expected);

  // optimize() on (= x (- x 1)) should return (-- x)
  st = Assign{f, ASSIGN, x, Binary{f, SUB, x, one}};
  optimized = opt.optimize(f, st);
  expected.clear();
  Fmt{&expected} << "(-- " << x << ")";
  TEST(to_string(optimized), ==, expected);

  if (!kind.is_float()) {
    // optimize() on (&= x (% x x)) should return (= x 0)
    st = Assign{f, AND_ASSIGN, x, Binary{f, REM, x, x}};
    optimized = opt.optimize(f, st);
    expected.clear();
    Fmt{&expected} << "(= " << x << " 0)";
    TEST(to_string(optimized), ==, expected);
  }
}

} // namespace onejit
