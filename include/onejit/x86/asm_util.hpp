/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
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
 * asm_util.hpp
 *
 *  Created on Feb 01, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X86_ASM_UTIL_HPP
#define ONEJIT_X86_ASM_UTIL_HPP

#include <onejit/x86/fwd.hpp>
#include <onejit/x86/scale.hpp>

namespace onejit {
namespace x86 {

class AsmUtil {
public:
  static size_t get_offset_minbytes(Mem mem, Reg base, Reg index) noexcept;

  static size_t insert_modrm_sib(uint8_t buf[], size_t len, size_t immediate_bytes, //
                                 Reg base, Reg index, Scale scale);

  static size_t insert_offset_or_imm(uint8_t buf[], size_t len, size_t immediate_bytes,
                                     int32_t offset);
};

} // namespace x86
} // namespace onejit

#endif // ONEJIT_X86_ASM_UTIL_HPP
