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
 * main.cpp
 *
 *  Created on Jan 08, 2020
 *      Author Massimiliano Ghilardi
 */

#include <onejit/chars.hpp>
#include <onejit/func.hpp>

#include <iostream>

namespace onejit {
class Test {
public:
  static void run();
};

void Test::run() {
  for (uint8_t i = 0; i < 2 * (1 + ArchFlags.val()); i++) {
    const Kind k(i);
    std::cout << "Kind " << k << ", Group " << k.group() << ", bits " << k.bits() << '\n';
  }

  Code holder;
  Func func(&holder);

  ConstExpr ce = func.new_const(Const{1.5f});

  for (uint8_t i = 0; i <= ArchFlags.val(); i++) {
    const Expr ve = func.new_var(Kind(i));
    std::cout << ve.type() << ' ' << ve.to<Node>() << '\n';

    const Node node = func.new_binary(ve.kind(), ce, ADD, ve);
    std::cout << node.type() << ' ' << node.to<Expr>() << '\n';

    const Var var1 = ve.to<VarExpr>().var();
    const Var var2 = Var::parse_direct(var1.direct());
    CHECK(var1, ==, var2);
  }

  std::cout << std::hex;
  for (const CodeItem item : holder) {
    std::cout << Chars("0x") << item << ' ';
  }
  std::cout << '\n';
}

} // namespace onejit

using namespace onejit;

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  onejit::Test::run();

  return 0;
}
