/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test_stmt.cpp
 *
 *  Created on Jun 25, 2021
 *      Author Massimiliano Ghilardi
 */

#include "test.hpp"

#include <onejit/ir.hpp>

namespace onejit {

void Test::stmt_if() {
  Func &f = func;

  Node empty;
  Chars expected = "_";
  TEST(to_string(empty), ==, expected);

  Stmt stmt = BadStmt{};
  expected = "_";
  TEST(to_string(stmt), ==, expected);

  stmt = If{f, TrueExpr, Two(f, Uint8), Zero(Uint8)};
  expected = "(if true\n\
    2\n\
    0)";
  TEST(to_string(stmt), ==, expected);

  stmt = If{f, FalseExpr, VoidConst};
  expected = "(if false\n\
    void\n\
    void)";
  TEST(to_string(stmt), ==, expected);

  holder.clear();
}

} // namespace onejit
