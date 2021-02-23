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

#include <onejit/node/childrange.hpp>
#include <onejit/node/const.hpp>
#include <onejit/node/label.hpp>
#include <onejit/node/tuple.hpp>
#include <onejit/node/var.hpp>
#include <onejit/x64/address.hpp>
#include <onejit/x64/compiler.hpp>
#include <onejit/x64/mem.hpp>

namespace onejit {
namespace x64 {

Node Mem::create(Func &func, Kind kind, const Address &address) noexcept {
  do {
    Expr args[5] = {};
    size_t len = 0;
    if (address.label) {
      args[0] = address.label;
      len = 1;
    }
    if (address.offset != 0) {
      args[1] = Const{func, address.offset};
      if (!args[1]) {
        break;
      }
      len = 2;
    }
    if (address.base) {
      args[2] = address.base;
      len = 3;
    }
    if (address.index && address.scale) {
      args[3] = address.index;
      args[4] = Const{Uint8, uint16_t(address.scale.val())};
      len = 5;
    }
    return Base::create(func, kind, X86_MEM, Exprs{args, len});

  } while (false);
  return Mem{};
}

Node Mem::create(Compiler &comp, Kind kind, Exprs children) noexcept {
  Func *func = comp.func();
  Address address;
  if (func && address.insert(comp, children)) {
    return create(*func, kind, address);
  }
  return Node{};
}

Node Mem::create(Compiler &comp, Kind kind, const ChildRange &children) noexcept {
  Func *func = comp.func();
  Address address;
  if (func && address.insert(comp, children)) {
    return create(*func, kind, address);
  }
  return Node{};
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
