/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * assembler.cpp
 *
 *  Created on Jan 30, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/assembler.hpp>
#include <onejit/error.hpp>

namespace onejit {

Assembler::~Assembler() noexcept {
}

Assembler &Assembler::add_relocation(Label l) noexcept {
  if (l && !relocation_.append(Relocation{size(), l})) {
    good_ = false;
  }
  return *this;
}

Assembler &Assembler::error(Node where, Chars msg) noexcept {
  good_ = good_ && error_.append(Error{where, msg});
  return *this;
}

Assembler &Assembler::out_of_memory(Node where) noexcept {
  // always set good_ to false
  good_ = good_ && error_.append(Error{where, "out of memory"}) && false;
  return *this;
}

} // namespace onejit
