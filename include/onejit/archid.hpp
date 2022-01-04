/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * archid.hpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ARCHID_HPP
#define ONEJIT_ARCHID_HPP

#include <onejit/fwd.hpp>

#include <cstdint> // uint8_t

namespace onejit {

enum ArchId : uint8_t {
  NOARCH = 0,
  MIR = 1, // uses libmir for native code generation
  X64 = 2, // alias for x86_64, amd64
  X86 = 3, // alias for i386, i486, i586, i686, ix86
  ARM64 = 4,
  ARM = 5,

  ARCHID_N,
};

Chars to_string(ArchId a) noexcept;

const Fmt &operator<<(const Fmt &fmt, ArchId archid);

} // namespace onejit

#endif // ONEJIT_ARCHID_HPP
