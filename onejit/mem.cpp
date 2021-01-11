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
 * mem.cpp
 *
 *  Created on Jan 11, 2020
 *      Author Massimiliano Ghilardi
 */

#include "onejit/mem.hpp"

#include <cstdlib>
#include <exception>

namespace onejit {
namespace mem {

static void throw_bad_alloc() {
  throw std::bad_alloc();
}

void *alloc_bytes(size_t n_bytes) {
  if (n_bytes == 0) {
    return NULL;
  }
  void *addr = std::malloc(n_bytes);
  if (!addr) {
    throw_bad_alloc();
  }
  return addr;
}

void *realloc_bytes(void *addr, size_t n_bytes) {
  if (!addr) {
    return alloc_bytes(n_bytes);
  }
  if (n_bytes == 0) {
    free_bytes(addr);
    return NULL;
  }
  void *new_addr = std::realloc(addr, n_bytes);
  if (!new_addr) {
    throw_bad_alloc();
  }
  return new_addr;
}

void free_bytes(void *addr) {
  if (addr) {
    std::free(addr);
  }
}

} // namespace mem
} // namespace onejit
