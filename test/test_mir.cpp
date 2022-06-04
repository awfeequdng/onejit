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

void Test::expr_mir() {
  Func &f = func.reset(&holder, Name{&holder, "expr_mir"}, FuncType{&holder, {}, {}});

  {
    mir::Mem mem{f, Uint8, mir::Address{1234, Var{f, Ptr}, Var{f, Uint64}, mir::Scale8}};

    Chars expected = "(mir_mem_ub 1234 var1000_p var1001_ul 8)";
    TEST(to_string(mem), ==, expected);
  }
}

void Test::func_fib_mir() {
  make_func_fib(Uint64);
  Func &f = func;
  compile(f, MIR);

  Chars expected = "(block\n\
    label_0\n\
    (mir_uble label_1 var1000_ul 2)\n\
    (mir_sub var1002_ul var1000_ul 1)\n\
    (mir_call label_0 (mir_rets var1003_ul) var1002_ul)\n\
    (mir_sub var1004_ul var1000_ul 2)\n\
    (mir_call label_0 (mir_rets var1005_ul) var1004_ul)\n\
    (mir_add var1001_ul var1003_ul var1005_ul)\n\
    (mir_ret var1001_ul)\n\
    (mir_jmp label_2)\n\
    label_1\n\
    (mir_mov var1001_ul 1)\n\
    (mir_ret var1001_ul)\n\
    label_2\n\
    (mir_ret var1001_ul))";

  TEST(to_string(f.get_compiled(MIR)), ==, expected);

  mir::Assembler assembler;

  assembler.add(f);
}

} // namespace onejit
