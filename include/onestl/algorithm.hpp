/*
 * onestl - Tiny STL C++ library
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
 * algorithm.hpp
 *
 *  Created on Mar 13, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_ALGORITHM_HPP
#define ONESTL_ALGORITHM_HPP

#include <cstddef> // size_t

namespace onestl {

constexpr size_t max2(size_t a, size_t b) noexcept {
  return b < a ? a : b;
}

constexpr size_t min2(size_t a, size_t b) noexcept {
  return a < b ? a : b;
}

} // namespace onestl

#endif // ONESTL_ALGORITHM_HPP
