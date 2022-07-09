/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test.hpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef TEST_TEST_HPP
#define TEST_TEST_HPP

#include <onejit/compiler.hpp>
#include <onejit/func.hpp>
#include <onejit/optimizer.hpp>

#include "test_disasm.hpp"

namespace onejit {

class Test : public TestDisasm {
public:
  Test();
  ~Test();

  void run();

private:
  FuncType ftype();
  void dump_and_clear_code();

  // called by run()
  void stl_bitset(); // test onestl::BitSet
  void stl_graph();  // test onestl::Graph
  void stl_scan();   // test onestl::Scan
  void arch();
  void kind();

  void expr_const() const;
  void expr_simple();
  void expr_nested();
  void expr_tuple();
  void expr_mir();
  void expr_x64();
  void eval_expr();
  void eval_expr_kind(Kind kind);

  void stmt_if();

  void func_fib();
  void func_fib_mir();
  void func_loop();
  void func_loop_mir();
  void func_memchr();
  void func_memchr_mir();
  void func_switch1();
  void func_switch2();
  void func_cond();
  void func_and_or();
  void func_tuple();
  void func_max();

  void optimize();
  void optimize_expr_kind(Kind kind);
  void optimize_assign_kind(Kind kind);
  void regallocator();

  Func &make_func_fib(Kind kind);
  Func &make_func_loop(Kind kind);
  Func &make_func_memchr(Kind kind);

  void compile(Func &func, ArchId archid);

  // return high-resolution clock: CLOCK_THREAD_CPUTIME_ID if available, otherwise CLOCK_REALTIM
  static double get_cpu_clock();

  Code holder;
  Func func;
  Compiler comp;
  Optimizer opt;
};

} // namespace onejit

#endif // TEST_TEST_HPP
