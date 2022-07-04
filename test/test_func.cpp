/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test_func.cpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include "test.hpp"

#include <onejit/fmt_fwd.hpp>
#include <onejit/func.hpp>
#include <onejit/ir.hpp>

namespace onejit {

void Test::func_fib() {
  Func &f = make_func_fib(Uint64);

  Chars expected = "(if (> var1000_ul 2)\n\
    (return (+ (call label_0 (- var1000_ul 1)) (call label_0 (- var1000_ul 2))))\n\
    (return 1))";
  TEST(to_string(f.get_body()), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_ul)\n\
    (asm_jbe label_1 var1000_ul 2)\n\
    (= var1002_ul (- var1000_ul 1))\n\
    (= var1003_ul (call label_0 var1002_ul))\n\
    (= var1004_ul (- var1000_ul 2))\n\
    (= var1005_ul (call label_0 var1004_ul))\n\
    (= var1001_ul (+ var1003_ul var1005_ul))\n\
    (return var1001_ul)\n\
    (goto label_2)\n\
    label_1\n\
    (= var1001_ul 1)\n\
    (return var1001_ul)\n\
    label_2\n\
    (return var1001_ul))";
  compile(f, NOARCH);
  TEST(to_string(f.get_compiled(NOARCH)), ==, expected);

  expected = "(block\n\
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
  compile(f, MIR);
  TEST(to_string(f.get_compiled(MIR)), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_ul)\n\
    (x86_cmp var1000_ul 2)\n\
    (x86_jbe label_1)\n\
    (= var1002_ul (- var1000_ul 1))\n\
    (x86_call_ label_0 (_set var1003_ul) var1002_ul)\n\
    (= var1004_ul (- var1000_ul 2))\n\
    (x86_call_ label_0 (_set var1005_ul) var1004_ul)\n\
    (x86_lea var1001_ul (x86_mem_p var1003_ul var1005_ul 1))\n\
    (x86_ret var1001_ul)\n\
    (x86_jmp label_2)\n\
    label_1\n\
    (x86_mov var1001_ul 1)\n\
    (x86_ret var1001_ul)\n\
    label_2\n\
    (x86_ret var1001_ul))";
  compile(f, X64);
  TEST(to_string(f.get_compiled(X64)), ==, expected);

  expected = "(flowgraph\n\
    (bb_0\n\
        (nodes\n\
            label_0\n\
            (_set var1000_ul)\n\
            (x86_cmp var1000_ul 2)\n\
            (x86_jbe label_1)\n\
        )\n\
        (next bb_1 bb_3)\n\
    )\n\
    (bb_1\n\
        (prev bb_0)\n\
        (nodes\n\
            (= var1002_ul (- var1000_ul 1))\n\
            (x86_call_ label_0 (_set var1003_ul) var1002_ul)\n\
            (= var1004_ul (- var1000_ul 2))\n\
            (x86_call_ label_0 (_set var1005_ul) var1004_ul)\n\
            (x86_lea var1001_ul (x86_mem_p var1003_ul var1005_ul 1))\n\
            (x86_ret var1001_ul)\n\
        )\n\
    )\n\
    (bb_2\n\
        (nodes\n\
            (x86_jmp label_2)\n\
        )\n\
        (next bb_4)\n\
    )\n\
    (bb_3\n\
        (prev bb_0)\n\
        (nodes\n\
            label_1\n\
            (x86_mov var1001_ul 1)\n\
            (x86_ret var1001_ul)\n\
        )\n\
    )\n\
    (bb_4\n\
        (prev bb_2)\n\
        (nodes\n\
            label_2\n\
            (x86_ret var1001_ul)\n\
        )\n\
    )\n\
)";
  TEST(to_string(comp.flowgraph_), ==, expected);

  // dump_and_clear_code();
  holder.clear();
}

void Test::func_loop() {
  Func &f = make_func_loop(Uint64);

  Chars expected = "(block\n\
    (= var1001_ul 0)\n\
    (for (= var1002_ul 0) (< var1002_ul var1000_ul) (++ var1002_ul)\n\
        (+= var1001_ul var1002_ul))\n\
    (return var1001_ul))";
  TEST(to_string(f.get_body()), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_ul)\n\
    (= var1001_ul 0)\n\
    (= var1002_ul 0)\n\
    (goto label_2)\n\
    label_1\n\
    (+= var1001_ul var1002_ul)\n\
    (++ var1002_ul)\n\
    label_2\n\
    (asm_jb label_1 var1002_ul var1000_ul)\n\
    label_3\n\
    (return var1001_ul))";
  compile(f, NOARCH);
  TEST(to_string(f.get_compiled(NOARCH)), ==, expected);

  expected = "(block\n\
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
  compile(f, MIR);
  TEST(to_string(f.get_compiled(MIR)), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_ul)\n\
    (x86_mov var1001_ul 0)\n\
    (x86_mov var1002_ul 0)\n\
    (x86_jmp label_2)\n\
    label_1\n\
    (x86_add var1001_ul var1002_ul)\n\
    (x86_inc var1002_ul)\n\
    label_2\n\
    (x86_cmp var1002_ul var1000_ul)\n\
    (x86_jb label_1)\n\
    label_3\n\
    (x86_ret var1001_ul))";
  compile(f, X64);
  TEST(to_string(f.get_compiled(X64)), ==, expected);

  expected = "(flowgraph\n\
    (bb_0\n\
        (nodes\n\
            label_0\n\
            (_set var1000_ul)\n\
            (x86_mov var1001_ul 0)\n\
            (x86_mov var1002_ul 0)\n\
            (x86_jmp label_2)\n\
        )\n\
        (next bb_2)\n\
    )\n\
    (bb_1\n\
        (prev bb_2)\n\
        (nodes\n\
            label_1\n\
            (x86_add var1001_ul var1002_ul)\n\
            (x86_inc var1002_ul)\n\
        )\n\
        (next bb_2)\n\
    )\n\
    (bb_2\n\
        (prev bb_0 bb_1)\n\
        (nodes\n\
            label_2\n\
            (x86_cmp var1002_ul var1000_ul)\n\
            (x86_jb label_1)\n\
        )\n\
        (next bb_3 bb_1)\n\
    )\n\
    (bb_3\n\
        (prev bb_2)\n\
        (nodes\n\
            label_3\n\
            (x86_ret var1001_ul)\n\
        )\n\
    )\n\
)";
  TEST(to_string(comp.flowgraph_), ==, expected);

  // dump_and_clear_code();
  holder.clear();
}

void Test::func_switch1() {
  Kind kind = Uint64;
  Func &f = func.reset(&holder, Name{&holder, "fswitch1"}, FuncType{&holder, {kind}, {kind}});
  Var n = f.param(0);
  Var ret = f.result(0);
  Const zero = Zero(kind);
  Const one = One(f, kind);
  Const two = Two(f, kind);

  /**
   * jit equivalent of C/C++ source code
   *
   * uint64_t fswitch1(uint64_t n) {
   *   uint64_t ret;
   *   switch (n) {
   *     case 0:
   *       ret = 1;
   *       break;
   *     case 1:
   *       ret = 2;
   *       break;
   *     default:
   *       ret = n + 1;
   *       break;
   *   }
   *   return ret;
   * }
   */

  f.set_body( //
      Block{f,
            {Switch{f,
                    n,
                    {Case{f, zero, Assign{f, ASSIGN, ret, one}}, //
                     Case{f, one, Assign{f, ASSIGN, ret, two}},  //
                     Default{f, Assign{f, ASSIGN, ret,           //
                                       Tuple{f, ADD, n, one}}}}},
             Return{f, ret}}});

  Chars expected = "(block\n\
    (switch\n\
        var1000_ul\n\
        (case 0 (= var1001_ul 1))\n\
        (case 1 (= var1001_ul 2))\n\
        (default (= var1001_ul (+ var1000_ul 1))))\n\
    (return var1001_ul))";
  TEST(to_string(f.get_body()), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_ul)\n\
    (asm_jne label_2 var1000_ul 0)\n\
    (= var1001_ul 1)\n\
    (goto label_1)\n\
    label_2\n\
    (asm_jne label_4 var1000_ul 1)\n\
    label_3\n\
    (= var1001_ul 2)\n\
    (goto label_1)\n\
    label_4\n\
    label_5\n\
    (= var1001_ul (+ var1000_ul 1))\n\
    label_1\n\
    (return var1001_ul))";
  compile(f, NOARCH);
  TEST(to_string(f.get_compiled(NOARCH)), ==, expected);

  expected = "(block\n\
    label_0\n\
    (mir_bne label_2 var1000_ul 0)\n\
    (mir_mov var1001_ul 1)\n\
    (mir_jmp label_1)\n\
    label_2\n\
    (mir_bne label_4 var1000_ul 1)\n\
    label_3\n\
    (mir_mov var1001_ul 2)\n\
    (mir_jmp label_1)\n\
    label_4\n\
    label_5\n\
    (mir_add var1001_ul var1000_ul 1)\n\
    label_1\n\
    (mir_ret var1001_ul))";
  compile(f, MIR);
  TEST(to_string(f.get_compiled(MIR)), ==, expected);

  expected = "(flowgraph\n\
    (bb_0\n\
        (nodes\n\
            label_0\n\
            (_set var1000_ul)\n\
            (x86_cmp var1000_ul 0)\n\
            (x86_jne label_2)\n\
        )\n\
        (next bb_1 bb_2)\n\
    )\n\
    (bb_1\n\
        (prev bb_0)\n\
        (nodes\n\
            (x86_mov var1001_ul 1)\n\
            (x86_jmp label_1)\n\
        )\n\
        (next bb_5)\n\
    )\n\
    (bb_2\n\
        (prev bb_0)\n\
        (nodes\n\
            label_2\n\
            (x86_cmp var1000_ul 1)\n\
            (x86_jne label_4)\n\
        )\n\
        (next bb_3 bb_4)\n\
    )\n\
    (bb_3\n\
        (prev bb_2)\n\
        (nodes\n\
            label_3\n\
            (x86_mov var1001_ul 2)\n\
            (x86_jmp label_1)\n\
        )\n\
        (next bb_5)\n\
    )\n\
    (bb_4\n\
        (prev bb_2)\n\
        (nodes\n\
            label_4\n\
            label_5\n\
            (x86_lea var1001_ul (x86_mem_p 1 var1000_ul))\n\
        )\n\
        (next bb_5)\n\
    )\n\
    (bb_5\n\
        (prev bb_1 bb_3 bb_4)\n\
        (nodes\n\
            label_1\n\
            (x86_ret var1001_ul)\n\
        )\n\
    )\n\
)";
  compile(f, X64);
  TEST(to_string(comp.flowgraph_), ==, expected);

  // dump_and_clear_code();
  holder.clear();
}

void Test::func_switch2() {
  Kind kind = Uint64;
  Func &f = func.reset(&holder, Name{&holder, "fswitch2"}, FuncType{&holder, {kind}, {kind}});
  Var n = f.param(0);
  Var ret = f.result(0);
  Const zero = Zero(kind);
  Const one = One(f, kind);
  Const two = Two(f, kind);

  /**
   * jit equivalent of C/C++ source code
   *
   * uint64_t fswitch2(uint64_t n) {
   *   uint64_t ret;
   *   switch (n) {
   *     case 0:
   *       ret = 1;
   *       break;
   *     default:
   *       ret = n + 1;
   *       break;
   *     case 1:
   *       ret = 2;
   *       break;
   *   }
   *   return ret;
   * }
   */

  f.set_body( //
      Block{f,
            {Switch{f,
                    n,
                    {Case{f, zero, Assign{f, ASSIGN, ret, one}},  //
                     Default{f, Assign{f, ASSIGN, ret,            //
                                       Tuple{f, ADD, n, one}}},   //
                     Case{f, one, Assign{f, ASSIGN, ret, two}}}}, //
             Return{f, ret}}});

  Chars expected = "(block\n\
    (switch\n\
        var1000_ul\n\
        (case 0 (= var1001_ul 1))\n\
        (default (= var1001_ul (+ var1000_ul 1)))\n\
        (case 1 (= var1001_ul 2)))\n\
    (return var1001_ul))";
  TEST(to_string(f.get_body()), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_ul)\n\
    (asm_jne label_2 var1000_ul 0)\n\
    (= var1001_ul 1)\n\
    (goto label_1)\n\
    label_2\n\
    (goto label_4)\n\
    label_3\n\
    (= var1001_ul (+ var1000_ul 1))\n\
    (goto label_1)\n\
    label_4\n\
    (asm_jne label_3 var1000_ul 1)\n\
    label_5\n\
    (= var1001_ul 2)\n\
    label_1\n\
    (return var1001_ul))";
  compile(f, NOARCH);
  TEST(to_string(f.get_compiled(NOARCH)), ==, expected);

  expected = "(block\n\
    label_0\n\
    (mir_bne label_2 var1000_ul 0)\n\
    (mir_mov var1001_ul 1)\n\
    (mir_jmp label_1)\n\
    label_2\n\
    (mir_jmp label_4)\n\
    label_3\n\
    (mir_add var1001_ul var1000_ul 1)\n\
    (mir_jmp label_1)\n\
    label_4\n\
    (mir_bne label_3 var1000_ul 1)\n\
    label_5\n\
    (mir_mov var1001_ul 2)\n\
    label_1\n\
    (mir_ret var1001_ul))";
  compile(f, MIR);
  TEST(to_string(f.get_compiled(MIR)), ==, expected);

  expected = "(flowgraph\n\
    (bb_0\n\
        (nodes\n\
            label_0\n\
            (_set var1000_ul)\n\
            (x86_cmp var1000_ul 0)\n\
            (x86_jne label_2)\n\
        )\n\
        (next bb_1 bb_2)\n\
    )\n\
    (bb_1\n\
        (prev bb_0)\n\
        (nodes\n\
            (x86_mov var1001_ul 1)\n\
            (x86_jmp label_1)\n\
        )\n\
        (next bb_6)\n\
    )\n\
    (bb_2\n\
        (prev bb_0)\n\
        (nodes\n\
            label_2\n\
            (x86_jmp label_4)\n\
        )\n\
        (next bb_4)\n\
    )\n\
    (bb_3\n\
        (prev bb_4)\n\
        (nodes\n\
            label_3\n\
            (x86_lea var1001_ul (x86_mem_p 1 var1000_ul))\n\
            (x86_jmp label_1)\n\
        )\n\
        (next bb_6)\n\
    )\n\
    (bb_4\n\
        (prev bb_2)\n\
        (nodes\n\
            label_4\n\
            (x86_cmp var1000_ul 1)\n\
            (x86_jne label_3)\n\
        )\n\
        (next bb_5 bb_3)\n\
    )\n\
    (bb_5\n\
        (prev bb_4)\n\
        (nodes\n\
            label_5\n\
            (x86_mov var1001_ul 2)\n\
        )\n\
        (next bb_6)\n\
    )\n\
    (bb_6\n\
        (prev bb_1 bb_3 bb_5)\n\
        (nodes\n\
            label_1\n\
            (x86_ret var1001_ul)\n\
        )\n\
    )\n\
)";
  compile(f, X64);
  TEST(to_string(comp.flowgraph_), ==, expected);

  // dump_and_clear_code();
  holder.clear();
}

void Test::func_cond() {
  Kind kind = Uint64;
  Func &f = func.reset(&holder, Name{&holder, "fcond"}, FuncType{&holder, {kind}, {kind}});
  Var n = f.param(0);
  Var ret = f.result(0);
  Const zero = Zero(kind);
  Const one = One(f, kind);
  Const two = Two(f, kind);

  /**
   * jit equivalent of C/C++ source code
   *
   * uint64_t fswitch(uint64_t n) {
   *   uint64_t ret;
   *   if (n == 0) {
   *     ret = 1;
   *   } else if (n == 1) {
   *     ret = 2;
   *   } else if (true) {
   *     ret = n + 1;
   *   }
   *   return ret;
   * }
   */

  f.set_body( //
      Block{f,
            {Cond{f,
                  {Binary{f, EQL, n, zero}, Assign{f, ASSIGN, ret, one},      //
                   Binary{f, EQL, n, one}, Assign{f, ASSIGN, ret, two},       //
                   TrueExpr, Assign{f, ASSIGN, ret, Tuple{f, ADD, n, one}}}}, //
             Return{f, ret}}});

  Chars expected = "(block\n\
    (cond\n\
        (== var1000_ul 0)\n\
        (= var1001_ul 1)\n\
        (== var1000_ul 1)\n\
        (= var1001_ul 2)\n\
        true\n\
        (= var1001_ul (+ var1000_ul 1)))\n\
    (return var1001_ul))";
  TEST(to_string(f.get_body()), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_ul)\n\
    (asm_jne label_2 var1000_ul 0)\n\
    (= var1001_ul 1)\n\
    (goto label_1)\n\
    label_2\n\
    (asm_jne label_3 var1000_ul 1)\n\
    (= var1001_ul 2)\n\
    (goto label_1)\n\
    label_3\n\
    (= var1001_ul (+ var1000_ul 1))\n\
    label_1\n\
    (return var1001_ul))";
  compile(f, NOARCH);
  TEST(to_string(f.get_compiled(NOARCH)), ==, expected);

  expected = "(block\n\
    label_0\n\
    (mir_bne label_2 var1000_ul 0)\n\
    (mir_mov var1001_ul 1)\n\
    (mir_jmp label_1)\n\
    label_2\n\
    (mir_bne label_3 var1000_ul 1)\n\
    (mir_mov var1001_ul 2)\n\
    (mir_jmp label_1)\n\
    label_3\n\
    (mir_add var1001_ul var1000_ul 1)\n\
    label_1\n\
    (mir_ret var1001_ul))";
  compile(f, MIR);
  TEST(to_string(f.get_compiled(MIR)), ==, expected);

  expected = "(flowgraph\n\
    (bb_0\n\
        (nodes\n\
            label_0\n\
            (_set var1000_ul)\n\
            (x86_cmp var1000_ul 0)\n\
            (x86_jne label_2)\n\
        )\n\
        (next bb_1 bb_2)\n\
    )\n\
    (bb_1\n\
        (prev bb_0)\n\
        (nodes\n\
            (x86_mov var1001_ul 1)\n\
            (x86_jmp label_1)\n\
        )\n\
        (next bb_5)\n\
    )\n\
    (bb_2\n\
        (prev bb_0)\n\
        (nodes\n\
            label_2\n\
            (x86_cmp var1000_ul 1)\n\
            (x86_jne label_3)\n\
        )\n\
        (next bb_3 bb_4)\n\
    )\n\
    (bb_3\n\
        (prev bb_2)\n\
        (nodes\n\
            (x86_mov var1001_ul 2)\n\
            (x86_jmp label_1)\n\
        )\n\
        (next bb_5)\n\
    )\n\
    (bb_4\n\
        (prev bb_2)\n\
        (nodes\n\
            label_3\n\
            (x86_lea var1001_ul (x86_mem_p 1 var1000_ul))\n\
        )\n\
        (next bb_5)\n\
    )\n\
    (bb_5\n\
        (prev bb_1 bb_3 bb_4)\n\
        (nodes\n\
            label_1\n\
            (x86_ret var1001_ul)\n\
        )\n\
    )\n\
)";
  compile(f, X64);
  TEST(to_string(comp.flowgraph_), ==, expected);

  // dump_and_clear_code();
  holder.clear();
}

void Test::func_and_or() {
  Func &f = func.reset(&holder, Name{&holder, "f_and_or"}, FuncType{&holder, {Ptr, Ptr}, {Bool}});
  Var a = f.param(0);
  Var b = f.param(1);
  Mem ma{f, Bool, {a}};
  Mem mb{f, Bool, {b}};

  /**
   * jit equivalent of C/C++ source code
   *
   * bool fand_or(bool *a, bool *b) {
   *   return (*a && *b) ^ (*a || *b);
   * }
   */

  f.set_body(Return{f, Tuple{f, XOR,                  //
                             Binary{f, LAND, ma, mb}, //
                             Binary{f, LOR, ma, mb}}});

  Chars expected = "(return (^ (&& (mem_e var1000_p) (mem_e var1001_p)) "
                   "(|| (mem_e var1000_p) (mem_e var1001_p))))";
  TEST(to_string(f.get_body()), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_p var1001_p)\n\
    (= var1003_e (mem_e var1000_p))\n\
    (asm_je label_1 var1003_e false)\n\
    (= var1003_e (mem_e var1001_p))\n\
    label_1\n\
    (= var1004_e (mem_e var1000_p))\n\
    (asm_jne label_2 var1004_e false)\n\
    (= var1004_e (mem_e var1001_p))\n\
    label_2\n\
    (= var1002_e (^ var1003_e var1004_e))\n\
    (return var1002_e))";
  compile(f, NOARCH);
  TEST(to_string(f.get_compiled(NOARCH)), ==, expected);

  expected = "(block\n\
    label_0\n\
    (mir_mov var1003_e (mir_mem_e var1000_p))\n\
    (mir_beqs label_1 var1003_e false)\n\
    (mir_mov var1003_e (mir_mem_e var1001_p))\n\
    label_1\n\
    (mir_mov var1004_e (mir_mem_e var1000_p))\n\
    (mir_bnes label_2 var1004_e false)\n\
    (mir_mov var1004_e (mir_mem_e var1001_p))\n\
    label_2\n\
    (mir_xors var1002_e var1003_e var1004_e)\n\
    (mir_ret var1002_e))";
  compile(f, MIR);
  TEST(to_string(f.get_compiled(MIR)), ==, expected);

  // dump_and_clear_code();
  holder.clear();
}

void Test::func_tuple() {
  Func &f = func.reset(&holder, Name{&holder, "ftuple"},
                       FuncType{&holder, {Float64, Float64, Float64}, {Float64}});
  Var a = f.param(0);
  Var b = f.param(1);
  Var c = f.param(2);

  /**
   * jit equivalent of C/C++ source code
   *
   * bool ftuple(double a, double b, double c) {
   *   return a * b * c;
   * }
   */

  f.set_body(Return{f, Tuple{f, Float64, MUL, {a, b, c}}});

  Chars expected = "(return (* var1000_df var1001_df var1002_df))";
  TEST(to_string(f.get_body()), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_df var1001_df var1002_df)\n\
    (= var1003_df (* var1000_df var1001_df var1002_df))\n\
    (return var1003_df))";
  compile(f, NOARCH);
  TEST(to_string(f.get_compiled(NOARCH)), ==, expected);

  expected = "(block\n\
    label_0\n\
    (mir_dmul var1004_df var1000_df var1001_df)\n\
    (mir_dmul var1003_df var1004_df var1002_df)\n\
    (mir_ret var1003_df))";
  compile(f, MIR);
  TEST(to_string(f.get_compiled(MIR)), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_df var1001_df var1002_df)\n\
    (= var1003_df (* var1000_df var1001_df var1002_df))\n\
    (x86_ret var1003_df))";
  compile(f, X64);
  TEST(to_string(f.get_compiled(X64)), ==, expected);
}

void Test::func_max() {
  Func &f = func.reset(&holder, Name{&holder, "fmax"},
                       FuncType{&holder, {Float64, Float64, Float64}, {Float64}});
  Var a = f.param(0);
  Var b = f.param(1);
  Var c = f.param(2);

  /**
   * jit equivalent of C++ source code
   *
   * double fmax(double a, double b, double c) {
   *   return std::max({a+b, b+c, a+c});
   * }
   */

  f.set_body(Return{f, Tuple{f,
                             Float64,
                             MAX,
                             {Tuple{f, ADD, a, b}, //
                              Tuple{f, ADD, b, c}, //
                              Tuple{f, ADD, a, c}}}});

  Chars expected = "(return (max (+ var1000_df var1001_df) (+ var1001_df var1002_df) (+ var1000_df "
                   "var1002_df)))";
  TEST(to_string(f.get_body()), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_df var1001_df var1002_df)\n\
    (= var1004_df (+ var1000_df var1001_df))\n\
    (= var1005_df (+ var1000_df var1002_df))\n\
    (= var1006_df (+ var1001_df var1002_df))\n\
    (= var1003_df (max var1004_df var1005_df var1006_df))\n\
    (return var1003_df))";
  compile(f, NOARCH);
  TEST(to_string(f.get_compiled(NOARCH)), ==, expected);

  expected = "(block\n\
    label_0\n\
    (mir_dadd var1004_df var1000_df var1001_df)\n\
    (mir_dadd var1005_df var1000_df var1002_df)\n\
    (mir_dadd var1006_df var1001_df var1002_df)\n\
    (mir_dmov var1007_df var1004_df)\n\
    (mir_dbge label_1 var1007_df var1005_df)\n\
    (mir_dmov var1007_df var1005_df)\n\
    label_1\n\
    (mir_dmov var1003_df var1007_df)\n\
    (mir_dbge label_2 var1003_df var1006_df)\n\
    (mir_dmov var1003_df var1006_df)\n\
    label_2\n\
    (mir_ret var1003_df))";
  compile(f, MIR);
  TEST(to_string(f.get_compiled(MIR)), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_df var1001_df var1002_df)\n\
    (x86_lea var1004_df (x86_mem_p var1000_df var1001_df 1))\n\
    (x86_lea var1005_df (x86_mem_p var1000_df var1002_df 1))\n\
    (x86_lea var1006_df (x86_mem_p var1001_df var1002_df 1))\n\
    (= var1003_df (max var1004_df var1005_df var1006_df))\n\
    (x86_ret var1003_df))";
  compile(f, X64);
  TEST(to_string(f.get_compiled(X64)), ==, expected);
}

void Test::func_memchr() {
  Func &f = make_func_memchr(Uint64);

  Chars expected = "(block\n\
    (for (= var1004_ul 0) (< var1004_ul var1001_ul) (++ var1004_ul)\n\
        (if (== (mem_ub var1000_p var1004_ul) var1002_ub)\n\
            (return (+ var1000_p var1004_ul))\n\
            void))\n\
    (return 0x0))";
  TEST(to_string(f.get_body()), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_p var1001_ul var1002_ub)\n\
    (= var1004_ul 0)\n\
    (goto label_2)\n\
    label_1\n\
    (asm_jne label_4 var1002_ub (mem_ub var1000_p var1004_ul))\n\
    (= var1003_p (+ var1004_ul var1000_p))\n\
    (return var1003_p)\n\
    label_4\n\
    (++ var1004_ul)\n\
    label_2\n\
    (asm_jb label_1 var1004_ul var1001_ul)\n\
    label_3\n\
    (= var1003_p 0x0)\n\
    (return var1003_p))";
  compile(f, NOARCH);
  TEST(to_string(f.get_compiled(NOARCH)), ==, expected);

  expected = "(block\n\
    label_0\n\
    (_set var1000_p var1001_ul var1002_ub)\n\
    (x86_mov var1004_ul 0)\n\
    (x86_jmp label_2)\n\
    label_1\n\
    (x86_cmp var1002_ub (x86_mem_ub var1000_p var1004_ul 1))\n\
    (x86_jne label_4)\n\
    (x86_lea var1003_p (x86_mem_p var1004_ul var1000_p 1))\n\
    (x86_ret var1003_p)\n\
    label_4\n\
    (x86_inc var1004_ul)\n\
    label_2\n\
    (x86_cmp var1004_ul var1001_ul)\n\
    (x86_jb label_1)\n\
    label_3\n\
    (x86_mov var1003_p 0x0)\n\
    (x86_ret var1003_p))";
  compile(f, X64);
  TEST(to_string(f.get_compiled(X64)), ==, expected);
}

} // namespace onejit
