/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * mem.cpp
 *
 *  Created on Jan 04, 2022
 *      Author Massimiliano Ghilardi
 */

#include <onejit/ir/childrange.hpp>
#include <onejit/ir/const.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/tuple.hpp>
#include <onejit/ir/var.hpp>
#include <onejit/mir/address.hpp>
#include <onejit/mir/compiler.hpp>
#include <onejit/mir/mem.hpp>

namespace onejit {
namespace mir {

Node Mem::create(Func &func, Kind kind, const Address &address) noexcept {
  do {
    Expr args[4] = {};
    size_t len = 0;
    if (address.offset != 0) {
      args[0] = Const{func, address.offset};
      if (!args[0]) {
        break;
      }
      len = 1;
    }
    if (address.base) {
      args[1] = address.base;
      len = 2;
    }
    if (address.index && address.scale) {
      args[2] = address.index;
      args[3] = Const{Uint8, uint16_t(address.scale)};
      len = 4;
    }
    return Base::create(func, kind, MIR_MEM, Exprs{args, len});

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

// shortcut for child_is<Const>(0).val().int64()
int64_t Mem::offset() const noexcept {
  return child_is<Const>(0).val().int64();
}

// shortcut for child_is<Var>(1).local()
Local Mem::base() const noexcept {
  return child_is<Var>(1).local();
}

// shortcut for child_is<Var>(2).local()
Local Mem::index() const noexcept {
  return child_is<Var>(2).local();
}

// shortcut for Scale(child_is<Const>(3).val().uint8())
Scale Mem::scale() const noexcept {
  return Scale(child_is<Const>(3).val().uint8());
}

} // namespace mir
} // namespace onejit
