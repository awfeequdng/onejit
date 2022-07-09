/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * rex_byte_x64.hpp
 *
 *  Created on Jan 29, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_REX_BYTE_HPP
#define ONEJIT_X64_REX_BYTE_HPP

#include <onejit/x64/reg.hpp>

namespace onejit {
namespace x64 {

uint8_t rex_byte(Bits default_size, Bits size, Reg base, Reg index = Reg{}) noexcept;

// REX byte if default width is 32
inline uint8_t rex_byte_default32(Bits size, Reg base, Reg index = Reg{}) noexcept {
  return rex_byte(Bits32, size, base, index);
}

// REX byte if default width is 32
inline uint8_t rex_byte_default64(Bits size, Reg base, Reg index = Reg{}) noexcept {
  return rex_byte(Bits64, size, base, index);
}

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_REX_BYTE_HPP
