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
 * mem.hpp
 *
 *  Created on Jan 11, 2020
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_MEM_HPP
#define ONEJIT_MEM_HPP

#include <cstddef> // size_t

namespace onejit {
namespace mem {

void *alloc_bytes(size_t n_bytes) throw();
void *realloc_bytes(void *addr, size_t new_n_bytes) throw();
void free_bytes(void *addr) throw();

template <class T> T *alloc(size_t n_elements) throw() {
  return reinterpret_cast<T *>(alloc_bytes(n_elements * sizeof(T)));
}

template <class T> T *realloc(T *addr, size_t n_elements) throw() {
  return reinterpret_cast<T *>(realloc_bytes(addr, n_elements * sizeof(T)));
}
template <class T> void free(T *addr) throw() {
  free_bytes(addr);
}

} // namespace mem
} // namespace onejit

#endif /* ONEJIT_MEM_HPP */
