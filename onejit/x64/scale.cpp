/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * scale.cpp
 *
 *  Created on Feb 03, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/x64/scale.hpp>

namespace onejit {
namespace x64 {

static const char sstring[6] = {'0', '1', '2', '4', '8', '?'};

const Chars Scale::string() const noexcept {
  enum _ { n = ONEJIT_N_OF(sstring) };
  size_t i = val_;
  if (i >= n) {
    i = n - 1; // "?"
  }
  return Chars{sstring + i, 1};
}

ONEJIT_NOINLINE const Fmt &operator<<(const Fmt &fmt, Scale scale) {
  return fmt << scale.string();
}

const Fmt &operator<<(const Fmt &fmt, eScale escale) {
  return fmt << Scale{escale};
}

} // namespace x64
} // namespace onejit
