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
