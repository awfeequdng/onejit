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

  eval_expr_T<int8_t>();
  eval_expr_T<int16_t>();
  eval_expr_T<int32_t>();
  eval_expr_T<int64_t>();
  eval_expr_T<uint8_t>();
  eval_expr_T<uint16_t>();
  eval_expr_T<uint32_t>();
  eval_expr_T<uint64_t>();
  eval_expr_T<float>();
  eval_expr_T<double>();
}

template <class T> void Test::eval_expr_T() {
  Func &f = func;

  Const one = Const{f, T(1)};
  Const two = Const{f, T(2)};
  Const three = Const{f, T(3)};
  Const four = Const{f, T(4)};

  // run eval() on the expression 4 / -1 + (2 * 3)
  Expr expr = Binary{
      f, ADD,                                    //
      Binary{f, QUO, four, Unary{f, NEG1, one}}, //
      Binary{f, MUL, two, three},                //
  };
  Value expected{T(T(4) / T(-T(1)) + T(2) * T(3))};

  TEST(is_const(expr), ==, true);
  Value result = eval(expr);
  TEST(result, ==, expected);

  // run eval() on the expression 3 < 4
  expr = Binary{f, LSS, three, four};
  expected = Value{T(3) < T(4)};

  TEST(is_const(expr), ==, true);
  result = eval(expr);
  TEST(result, ==, expected);
}

} // namespace onejit
