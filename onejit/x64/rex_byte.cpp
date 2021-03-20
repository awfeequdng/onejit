/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * rex_byte.cpp
 *
 *  Created on Feb 01, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/x64/reg.hpp>
#include <onejit/x64/rex_byte.hpp>

namespace onejit {
namespace x64 {



uint8_t rex_byte(Bits default_size, Bits size, Reg base, Reg index) noexcept {
  uint8_t byte = rhi(base) | rhi(index) << 1;
  if (default_size < Bits64 && size >= Bits64) {
    // REX byte is needed to use 64-bit registers when default size is 32 bits
    byte |= 0x48;
  } else if (byte || (base && !index                 //
                      && base.kind().bits() == Bits8 //
                      && base.reg_id() >= RSP)) {
    // and also to use the 8-bit registers %spl %bpl %sil %dil
    byte |= 0x40;
  }
  return byte;
}

} // namespace x64
} // namespace onejit
