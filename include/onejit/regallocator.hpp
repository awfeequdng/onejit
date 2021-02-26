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
 * regallocator.hpp
 *
 *  Created on Feb 26, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_REGALLOCATOR_HPP
#define ONEJIT_REGALLOCATOR_HPP

#include <onejit/idset.hpp>

namespace onejit {

// register allocator. uses register interference graph and Chaitin algorithm.
class RegAllocator {

public:
  constexpr RegAllocator() noexcept : set_{} {
  }

  ~RegAllocator() noexcept;

private:
  IdSet set_;
}; // class RegAllocator

} // namespace onejit

#endif // ONEJIT_REGALLOCATOR_HPP
