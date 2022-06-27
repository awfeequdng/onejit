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

#include <onejit/assembler.hpp>
#include <onejit/ir.hpp>
#include <onejit/mir.hpp>

#include "test.hpp"

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
  Func &f = make_func_fib(Uint32);
  compile(f, MIR);

  Chars expected = "(block\n\
    label_0\n\
    (mir_ubles label_1 var1000_ui 2)\n\
    (mir_subs var1002_ui var1000_ui 1)\n\
    (mir_call label_0 (mir_rets var1003_ui) var1002_ui)\n\
    (mir_subs var1004_ui var1000_ui 2)\n\
    (mir_call label_0 (mir_rets var1005_ui) var1004_ui)\n\
    (mir_adds var1001_ui var1003_ui var1005_ui)\n\
    (mir_ret var1001_ui)\n\
    (mir_jmp label_2)\n\
    label_1\n\
    (mir_mov var1001_ui 1)\n\
    (mir_ret var1001_ui)\n\
    label_2\n\
    (mir_ret var1001_ui))";

  TEST(to_string(f.get_compiled(MIR)), ==, expected);

  mir::Assembler assembler;

  void *jit_func_addr = assembler.assemble(f);

  Fmt fmt{stdout};
  fmt << assembler.errors();
  TEST(assembler.errors().size(), ==, 0);

  if (false) {
    // +16 skips the preamble 'movabs $0x____, %r11; jmp *%r11'
    const uint8_t *disam_addr = static_cast<const uint8_t *>(jit_func_addr) + 16;

    fmt << "jit_func compiled    at address 0x" << Hex(jit_func_addr) << '\n' //
        << "jit_func disassembly at address 0x" << Hex(disam_addr) << '\n';

    disasm(fmt, Bytes{disam_addr, 126}) << '\n';
  }

  // call jit-compiled function to compute fibonacci(40)
  using JitFtype = uint32_t (*)(uint32_t);
  JitFtype jit_func = JitFtype(jit_func_addr);

  for (size_t i = 0; i < 1; i++) {
    const double start = get_cpu_clock();
    const uint32_t arg = 40;
    const uint32_t ret = jit_func(arg);
    const double end = get_cpu_clock();

    TEST(ret, ==, 102334155);

    fmt << "  MIR jit-compiled function fibonacci(" << arg << ")\ttook " << (end - start)
        << " seconds\n";
  }
}

void Test::func_loop_mir() {
  Func &f = make_func_loop(Uint64);
  compile(f, MIR);

  Chars expected = "(block\n\
    label_0\n\
    (mir_mov var1001_ul 0)\n\
    (mir_mov var1002_ul 0)\n\
    (mir_jmp label_2)\n\
    label_1\n\
    (mir_add var1001_ul var1001_ul var1002_ul)\n\
    (mir_add var1002_ul var1002_ul 1)\n\
    label_2\n\
    (mir_ublt label_1 var1002_ul var1000_ul)\n\
    label_3\n\
    (mir_ret var1001_ul))";

  TEST(to_string(f.get_compiled(MIR)), ==, expected);

  mir::Assembler assembler;

  void *jit_func_addr = assembler.assemble(f);

  Fmt fmt{stdout};
  fmt << assembler.errors();
  TEST(assembler.errors().size(), ==, 0);

  if (false) {
    // +16 skips the preamble 'movabs $0x____, %r11; jmp *%r11'
    const uint8_t *disam_addr = static_cast<const uint8_t *>(jit_func_addr) + 16;

    fmt << "jit_func compiled    at address 0x" << Hex(jit_func_addr) << '\n' //
        << "jit_func disassembly at address 0x" << Hex(disam_addr) << '\n';

    disasm(fmt, Bytes{disam_addr, 64}) << '\n';
  }

  // call jit-compiled function to compute fibonacci(40)
  using JitFtype = uint64_t (*)(uint64_t);
  JitFtype jit_func = JitFtype(jit_func_addr);

  for (size_t i = 0; i < 1; i++) {
    const double start = get_cpu_clock();
    const uint64_t arg = 1234567890ul;
    const uint64_t ret = jit_func(arg);
    const double end = get_cpu_clock();

    TEST(ret, ==, arg * (arg - 1) / 2);

    fmt << "  MIR jit-compiled function loop(" << arg << ")\ttook " << (end - start)
        << " seconds\n";
  }
}

void Test::func_memchr_mir() {
  Func &f = make_func_memchr(Uint64);

  Chars expected = "(block\n\
    label_0\n\
    (mir_mov var1004_ul 0)\n\
    (mir_jmp label_2)\n\
    label_1\n\
    (mir_bnes label_4 var1002_ub (mir_mem_ub var1000_p var1004_ul 1))\n\
    (mir_add var1003_p var1004_ul var1000_p)\n\
    (mir_ret var1003_p)\n\
    label_4\n\
    (mir_add var1004_ul var1004_ul 1)\n\
    label_2\n\
    (mir_ublt label_1 var1004_ul var1001_ul)\n\
    label_3\n\
    (mir_mov var1003_p 0x0)\n\
    (mir_ret var1003_p))";
  compile(f, MIR);
  TEST(to_string(f.get_compiled(MIR)), ==, expected);
}

} // namespace onejit
