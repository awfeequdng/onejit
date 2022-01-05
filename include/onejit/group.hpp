/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * group.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_GROUP_HPP
#define ONEJIT_GROUP_HPP

#include <onejit/fwd.hpp>

#include <cstdint> // uint8_t

namespace onejit {

enum Group : uint8_t {
  gVoid = 0,
  gBool = 1,
  gInt = 2,
  gUint = 3,
  gFloat = 4,
  gPtr = 5,
};

const Chars &to_string(Group g) noexcept;

const Fmt &operator<<(const Fmt &fmt, Group g);

} // namespace onejit

#endif // ONEJIT_GROUP_HPP
