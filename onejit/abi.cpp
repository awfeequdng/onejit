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
