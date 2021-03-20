/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * mem.cpp
 *
 *  Created on Jan 11, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/mem.hpp>

#include <cstdlib>
#include <cstring>

namespace onestl {
namespace mem {

void *alloc_bytes(size_t n_bytes) noexcept {
  return std::malloc(n_bytes);
}

void *alloc_clear_bytes(size_t n_bytes) noexcept {
  return std::calloc(n_bytes, 1);
}

void clear_bytes(void *addr, size_t n_bytes) noexcept {
  std::memset(addr, 0, n_bytes);
}

void free_bytes(void *addr) noexcept {
  std::free(addr);
}

void *realloc_bytes(void *addr, size_t n_bytes) noexcept {
  return std::realloc(addr, n_bytes);
}

} // namespace mem
} // namespace onestl
