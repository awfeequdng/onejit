/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test_main.cpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include "test.hpp"

#include <onejit/assembler.hpp>
#include <onejit/codeparser.hpp>

#include <cstdio> // stdout

#ifdef __unix__
#include <fcntl.h>     // open()
#include <sys/stat.h>  //
#include <sys/types.h> //
#include <unistd.h>    // close(), write()
#endif

namespace onejit {

Test::Test() : holder{}, func{&holder, Name{&holder, "test_func"}, ftype()}, comp{}, opt{} {
}

Test::~Test() {
}

FuncType Test::ftype() {
  FuncType functype{&holder, {Int64, Ptr, Uint64}, {Int64}};
  Chars expected = "(ftype (int64 ptr uint64) -> (int64))";
  TEST(to_string(functype), ==, expected);
  return functype;
}

void Test::run() {
  stl_bitset();
  stl_graph();
  arch();
  kind();
  expr_const();
  expr_simple();
  expr_nested();
  expr_tuple();
  expr_mir();
  expr_x64();
  eval_expr();
  optimize();
  regallocator();
  func_fib();
  func_fib_mir();
  func_loop();
  func_loop_mir();
  func_switch1();
  func_switch2();
  func_cond();
  func_and_or();
  func_tuple();
  func_max();

  Fmt{stdout} << testcount() << " tests passed\n";
}

void Test::compile(Func &f, ArchId archid) {
  // implies comp.compile(f, OptAll);
  comp.compile_arch(f, archid, OptAll);

  CRange<Error> errors = comp.errors();
  if (errors) {
    Fmt{stdout} << f.name() << " compile errors:\n" << errors;
    ONEJIT_THROW_CHECK_FAILED();
  }
}

void Test::dump_and_clear_code() {
  Fmt fmt{stdout};
  for (CodeItem item : holder) {
    fmt << "0x" << Hex{item} << ' ';
  }
  fmt << '\n';

#ifdef __unix__
  {
    int fd = ::open("dump.1jit", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
      (void)::write(fd, holder.data(), holder.length());
      (void)::close(fd);
    }
  }
#endif // __unix__

  CodeParser parser{&holder};
  while (parser) {
    Node node = parser.next();
    fmt << node << '\n';
  }

  holder.clear();
}

} // namespace onejit

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  onejit::Test{}.run();

  return 0;
}
