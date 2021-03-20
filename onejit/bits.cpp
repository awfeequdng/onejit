/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * bits.cpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/bits.hpp>
#include <onejit/fmt.hpp>

namespace onejit {

const Fmt &operator<<(const Fmt &fmt, Bits bits) {
  return fmt << bits.val();
}

} // namespace onejit
