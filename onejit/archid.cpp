/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * archid.cpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/archid.hpp>
#include <onejit/fmt.hpp>

namespace onejit {

static const char archstring[] = //
    "\6NOARCH\0"
    "\3X64\0\0\0\0"
    "\3X86\0\0\0\0"
    "\5ARM64\0\0"
    "\3ARM";

Chars to_string(ArchId archid) noexcept {
  size_t i = 0;
  if (archid < ARCHID_N) {
    i = archid;
  }
  const char *str = &archstring[i * 8];
  return Chars{str + 1, uint8_t(str[0])};
}

const Fmt &operator<<(const Fmt &fmt, ArchId archid) {
  return fmt << to_string(archid);
}

} // namespace onejit
