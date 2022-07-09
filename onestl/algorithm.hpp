/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

constexpr inline size_t max2(size_t a, size_t b) noexcept {
  return b < a ? a : b;
}

constexpr inline size_t min2(size_t a, size_t b) noexcept {
  return a < b ? a : b;
}

constexpr inline size_t min3(size_t a, size_t b, size_t c) noexcept {
  return min2(min2(a, b), c);
}

} // namespace onestl

#endif // ONESTL_ALGORITHM_HPP
