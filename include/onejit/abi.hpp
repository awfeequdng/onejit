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
 * abi.hpp
 *
 *  Created on Mar 15, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ABI_HPP
#define ONEJIT_ABI_HPP

#include <cstdint>

namespace onejit {

// Enum describing function call ABI
enum Abi : uint32_t {
  Abi_auto = 0x0, // autodetect from operating system and architecture

  Abi_x64_auto = 0x10,
  Abi_x64_go1 = 0x11,     // all params/results on stack
  Abi_x64_windows = 0x12, // params in RCX,RDX,R8,R9 or XMM0..XMM3. result in RAX or XMM0
  // params in RDI,RSI,RDX,RCX,R8,R9 and XMM0..XMM7. result in RAX,RDX and XMM0
  Abi_x64_sysv = 0x13,

  Abi_x86_auto = 0x20,
  Abi_x86_go1 = 0x21, // all params/results on stack
  Abi_x86_cdecl = 0x22,
  Abi_x86_mscdecl = 0x23,
  Abi_x86_stdcall = 0x24,
  Abi_x86_fastcall = 0x25,

  Abi_arm64_auto = 0x30,
  Abi_arm64_go1 = 0x31, // all params/results on stack
  Abi_arm64 = 0x32,     // as defined by Arm Ltd.

  Abi_arm32_auto = 0x40,
  Abi_arm32_go1 = 0x41, // all params/results on stack
  Abi_arm32 = 0x42,     // as defined by Arm Ltd.
};

// if abi is *_auto, autodetect from operating system and/or architecture
Abi abi_autodetect(Abi abi) noexcept;

} // namespace onejit

#endif // ONEJIT_IR_ABI_HPP
