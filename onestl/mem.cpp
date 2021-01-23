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
