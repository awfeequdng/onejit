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
  void arch();
  void kind();
  void const_expr() const;
  void simple_expr();
  void nested_expr();
  void tuple_expr();
  void mir_expr();
  void x64_expr();
  void eval_expr();
  void eval_expr_kind(Kind kind);
  void func_fib();
  void func_loop();
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

  void compile(Func &func, ArchId archid);

  Code holder;
  Func func;
  Compiler comp;
  Optimizer opt;
};

} // namespace onejit
