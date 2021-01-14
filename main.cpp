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

using namespace onejit;

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  for (uint8_t i = 0; i < 2 * (1 + ArchFlags.val()); i++) {
    const Kind k(i);
    std::cout << "Kind " << k << ", Group " << k.group() << ", bits " << k.bits() << '\n';
  }

  Code holder;
  Func func(&holder);
  for (uint8_t i = 0; i <= ArchFlags.val(); i++) {
    const Reg r = func.new_reg(Kind(i));
    std::cout << "Reg " << r << '\n';
    func.add(r);

    const Reg r2 = Reg::from_direct(r.direct());
    CHECK(r, ==, r2);
  }
  Const c(uint32_t(0x7FFFFF));
  func.add(c);

  return 0;
}
