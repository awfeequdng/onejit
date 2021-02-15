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
 * optimizer_result.hpp
 *
 *  Created on Feb 15, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPTIMIZER_RESULT_HPP
#define ONEJIT_OPTIMIZER_RESULT_HPP

#include <onejit/optimizer.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////

enum Optimizer::Result : uint8_t {
  IsNone = 0,
  IsSame = 1 << 0,
  IsPure = 1 << 1,
  IsConst = 1 << 2,
  IsAll = 0x7,
};

constexpr Optimizer::Result operator~(Optimizer::Result a) noexcept {
  return Optimizer::Result(~uint8_t(a));
}

constexpr Optimizer::Result operator&(Optimizer::Result a, Optimizer::Result b) noexcept {
  return Optimizer::Result(uint8_t(a) & uint8_t(b));
}

constexpr Optimizer::Result operator|(Optimizer::Result a, Optimizer::Result b) noexcept {
  return Optimizer::Result(uint8_t(a) | uint8_t(b));
}

inline Optimizer::Result &operator&=(Optimizer::Result &a, Optimizer::Result b) noexcept {
  return a = a & b;
}

inline Optimizer::Result &operator|=(Optimizer::Result &a, Optimizer::Result b) noexcept {
  return a = a | b;
}

} // namespace onejit

#endif // ONEJIT_OPTIMIZER_RESULT_HPP
