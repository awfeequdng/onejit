/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test_mir.cpp
 *
 *  Created on Jan 04, 2022
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
    mir::Mem mem{f, Uint8, mir::Address{1234, Var{f, Ptr}, Var{f, Uint64}, mir::Scale8}};

    Chars expected = "(mir_mem_ub 1234 var1000_p var1001_ul 8)";
    TEST(to_string(mem), ==, expected);
  }

  Assembler assembler;

  // dump_and_clear_code();
  holder.clear();
  assembler.clear();
}

} // namespace onejit
