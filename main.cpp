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

#include <onejit/code.hpp>
#include <onejit/const.hpp>
#include <onejit/func.hpp>
#include <onejit/kind.hpp>

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
  for (uint8_t i = 0; i <= ArchFlags.val(); i++) {
    const VarExpr v = func.new_var(Kind(i));
    std::cout << "VarExpr " << v << '\n';
    func.add(v);

    const Var var1 = v.var();
    const Var var2 = Var::from_direct(var1.direct());
    CHECK(var1, ==, var2);
  }
  Const c1(uint32_t(0x7FFFFF));
  Const c2(1.0f);
  func.add(c1);
  func.add(c2);
}

} // namespace onejit

using namespace onejit;

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  onejit::Test::run();

  return 0;
}
