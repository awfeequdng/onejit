/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * imm.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/imm.hpp>
#include <onestl/chars.hpp>

namespace onejit {

Imm Imm::parse_indirect(Kind kind, Offset offset, const Code *holder) noexcept {
  uint64_t bits;
  if (kind.bits().val() == 64) {
    bits = holder->uint64(offset);
  } else {
    bits = holder->uint32(offset);
  }
  return Imm{kind, bits};
}

Code &Imm::write_indirect(Code *holder) const noexcept {
  if (kind().bits().val() == 64) {
    return holder->add_uint64(uint64());
  } else {
    return holder->add_uint32(uint32());
  }
}

} // namespace onejit
