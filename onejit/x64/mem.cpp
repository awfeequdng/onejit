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
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/ir/childrange.hpp>
#include <onejit/ir/const.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/tuple.hpp>
#include <onejit/ir/var.hpp>
#include <onejit/x64/address.hpp>
#include <onejit/x64/compiler.hpp>
#include <onejit/x64/mem.hpp>

namespace onejit {
namespace x64 {

Node Mem::create(Func &func, Kind kind, const Address &address) noexcept {

  Expr args[5] = {};
  size_t len = 0;
  if (address.label) {
    args[0] = address.label;
    len = 1;
  }
  if (address.offset != 0) {
    args[1] = Const{func, address.offset};
    if (!args[1]) {
      return Mem{};
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
