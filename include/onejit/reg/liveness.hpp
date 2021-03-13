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
