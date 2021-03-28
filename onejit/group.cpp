/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * group.cpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/group.hpp>
#include <onestl/chars.hpp>
#include <onestl/fmt.hpp>

namespace onejit {

static const Chars gstring[] = {
    "void", "bool",          //
    "int",  "uint", "float", //
    "ptr",  "arch", "?",     //
};

const Chars &to_string(Group g) noexcept {
  enum _ { n = sizeof(gstring) / sizeof(gstring[0]) };
  uint8_t i = (uint8_t)g;
  if (i >= n) {
    i = n - 1; // "?"
  }
  return gstring[i];
}

const Fmt &operator<<(const Fmt &fmt, Group g) {
  return fmt << to_string(g);
}

} // namespace onejit
