/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test_x64.cpp
 *
 *  Created on Feb 10, 2021
 *      Author Massimiliano Ghilardi
 */

#include "test.hpp"

#include <onejit/assembler.hpp>
#include <onejit/ir.hpp>
#include <onejit/mir.hpp>

namespace onejit {

void Test::mir_expr() {
  Func &f = func.reset(&holder, Name{&holder, "mir_expr"}, FuncType{&holder, {}, {}});

  {
    mir::Mem mem{f, Int32, mir::Address{1234}};

    Chars expected = "(mir_mem_i 1234)";
    TEST(to_string(mem), ==, expected);
  }

  Assembler assembler;

  // dump_and_clear_code();
  holder.clear();
  assembler.clear();
}

} // namespace onejit
