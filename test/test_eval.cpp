/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

void Test::eval_expr() {
  func.reset(&holder, Name{&holder, "eval_expr"}, FuncType{&holder, {}, {}});

  for (Kind kind : {Int8, Int16, Int32, Int64,     //
                    Uint8, Uint16, Uint32, Uint64, //
                    Float32, Float64}) {
    eval_expr_kind(kind);
  }
}

void Test::eval_expr_kind(Kind kind) {
  Func &f = func;

  Const one = One(f, kind);
  Const two = Two(f, kind);
  Const three{f, Value{3}.cast(kind)};
  Const four{f, Value{4}.cast(kind)};

  // run eval() on the expression 4 / -1 + (2 * 3)
  Expr expr = Tuple{
      f, ADD,                                    //
      Binary{f, QUO, four, Unary{f, NEG1, one}}, //
      Tuple{f, MUL, two, three},                 //
  };
  Value expected = Value{kind.is_unsigned() ? 6 : 2}.cast(kind);

  TEST(is_const(expr), ==, true);
  Value result = eval(expr);
  TEST(result, ==, expected);

  // run eval() on the expression 3 < 4
  expr = Binary{f, LSS, three, four};
  expected = Value{true};

  TEST(is_const(expr), ==, true);
  result = eval(expr);
  TEST(result, ==, expected);
}

} // namespace onejit
