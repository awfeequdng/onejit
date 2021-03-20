/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * space.cpp
 *
 *  Created on Feb 06, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/fmt.hpp>
#include <onejit/space.hpp>
#include <onestl/chars.hpp>

namespace onejit {

static const Chars str = "                                                                "
                         "                                                                "
                         "                                                                "
                         "                                                                ";

const Fmt &operator<<(const Fmt &fmt, Space space) {
  const size_t n = str.size();
  size_t left = space.size();

  while (left >= n) {
    fmt << str;
    left -= n;
  }
  if (left) {
    fmt << str.view(0, left);
  }
  return fmt;
}

} // namespace onejit
