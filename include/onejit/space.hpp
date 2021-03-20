/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * space.hpp
 *
 *  Created on Feb 06, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_SPACE_HPP
#define ONEJIT_SPACE_HPP

#include <onejit/fwd.hpp>

namespace onejit {

class Space {
public:
  constexpr explicit Space(size_t size) noexcept : size_{size} {
  }

  constexpr size_t size() const noexcept {
    return size_;
  }

private:
  size_t size_;
};

// write space.size() spaces
const Fmt &operator<<(const Fmt &fmt, Space space);

} // namespace onejit

#endif // ONEJIT_SPACE_HPP
