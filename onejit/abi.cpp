/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * abi.cpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/abi.hpp>

namespace onejit {

// if abi is *_auto, autodetect from operating system and/or architecture
Abi abi_autodetect(Abi abi) noexcept {
  if (abi & 0xf) {
    return abi;
  } else if (abi == Abi_auto) {
#if defined(__x86_64__) || defined(__amd64__)
    abi = Abi_x64_auto;
#elif defined(__i386__) || defined(__i686__)
    abi = Abi_x86_auto;
#elif defined(__aarch64__) || defined(__AARCH64EL__)
    abi = Abi_arm64_auto;
#elif defined(__arm__) || defined(__ARMEL__) || defined(__ARM_ARCH)
    abi = Abi_arm32_auto;
#endif
  }
  if (abi == Abi_x64_auto) {
#if defined(__win32)
    abi = Abi_x64_windows;
#else
    abi = Abi_x64_sysv;
#endif
  } else if (abi == Abi_x86_auto) {
#if defined(__win32)
    abi = Abi_x86_mscdecl;
#else
    abi = Abi_x86_cdecl;
#endif
  } else if (abi == Abi_arm64_auto) {
    abi = Abi_arm64;
  } else if (abi == Abi_arm32_auto) {
    abi = Abi_arm32;
  }
  return abi;
}

} // namespace onejit
