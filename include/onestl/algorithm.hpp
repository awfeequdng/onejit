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

constexpr size_t max2(size_t a, size_t b) noexcept {
  return b < a ? a : b;
}

constexpr size_t min2(size_t a, size_t b) noexcept {
  return a < b ? a : b;
}

} // namespace onestl

#endif // ONESTL_ALGORITHM_HPP
