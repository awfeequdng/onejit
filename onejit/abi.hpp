/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
