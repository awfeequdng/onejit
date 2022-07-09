/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * liveness.hpp
 *
 *  Created on Mar 08, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_REG_LIVENESS_HPP
#define ONEJIT_REG_LIVENESS_HPP

#include <onejit/fwd.hpp>
#include <onestl/bitset.hpp>

namespace onejit {
namespace reg {

// perform register liveness analysis
class Liveness {

public:
  Liveness() noexcept;

  Liveness(Liveness &&) noexcept = default;
  Liveness(const Liveness &) noexcept = delete;

  ~Liveness() noexcept;

  Liveness &operator=(Liveness &&) noexcept = default;
  Liveness &operator=(const Liveness &) noexcept = delete;

}; // class Liveness

} // namespace reg
} // namespace onejit

#endif // ONEJIT_REG_LIVENESS_HPP
