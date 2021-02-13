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
 * mem.cpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/node/const.hpp>
#include <onejit/x64/mem.hpp>

namespace onejit {
namespace x64 {

Node Mem::create(Func &func, Kind kind, const Label &label, const int32_t offset, const Var &base,
                 const Var &index, Scale scale) noexcept {
  do {
    Expr args[5] = {};
    size_t len = 0;
    if (label) {
      args[0] = label;
      len = 1;
    }
    if (offset != 0) {
      args[1] = Const{func, offset};
      if (!args[1]) {
        break;
      }
      len = 2;
    }
    if (base) {
      args[2] = base;
      len = 3;
    }
    if (index && scale != Scale0) {
      args[3] = index;
      args[4] = Const{Uint8, uint16_t(scale.val())};
      len = 5;
    }
    return Base::create(func, kind, X86_MEM, Exprs{args, len});

  } while (false);
  return Mem{};
}

// shortcut for child_is<Const>(1).val().int32()
int32_t Mem::offset() const noexcept {
  return child_is<Const>(1).val().int32();
}

// shortcut for child_is<Var>(2).local()
Local Mem::base() const noexcept {
  return child_is<Var>(2).local();
}

// shortcut for child_is<Var>(3).local()
Local Mem::index() const noexcept {
  return child_is<Var>(3).local();
}

// shortcut for Scale(child_is<Const>(4).val().uint8())
Scale Mem::scale() const noexcept {
  return Scale(child_is<Const>(4).val().uint8());
}

} // namespace x64
} // namespace onejit
