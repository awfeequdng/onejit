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
  kind();
  const_expr();
  simple_expr();
  nested_expr();
  x64_expr();
  eval_expr();
  optimize_expr();
  func_fib();
  func_loop();
  func_switch1();
  func_switch2();
  func_cond();
  func_and_or();

  Fmt{stdout} << testcount() << " tests passed\n";
}

void Test::compile(Func &f) {
  comp.compile(f);
}

void Test::dump_and_clear_code() {
  Fmt out{stdout};
  for (CodeItem item : holder) {
    out << "0x" << Hex{item} << ' ';
  }
  out << '\n';

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
    out << node << '\n';
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
