/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * util.hpp
 *
 *  Created on Feb 01, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_UTIL_HPP
#define ONEJIT_X64_UTIL_HPP

#include <onejit/x64/fwd.hpp>

namespace onejit {
namespace x64 {

////////////////////////////////////////////////////////////////////////////////
class Util {
public:
  static size_t get_offset_minbytes(Mem mem, Reg base, Reg index) noexcept;

  static size_t insert_modrm_sib(uint8_t buf[], size_t len, size_t immediate_bytes, //
                                 Reg base, Reg index, Scale scale);

  static size_t insert_offset_or_imm(uint8_t buf[], size_t len, size_t immediate_bytes,
                                     int32_t offset);

  static bool validate_reg(Assembler &dst, Reg reg);

  static bool validate_mem(Assembler &dst, Mem mem);
};

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_UTIL_HPP
