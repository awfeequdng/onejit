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
 * mem.hpp
 *
 *  Created on Jan 11, 2020
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_MEM_HPP
#define ONESTL_MEM_HPP

#include <cstddef> // size_t
#include <utility> // std::move()

namespace onestl {
namespace mem {

void *alloc_bytes(size_t n_bytes) throw();
void clear_bytes(void *addr, size_t n_bytes) throw();
void free_bytes(void *addr) throw();
void *realloc_bytes(void *addr, size_t new_n_bytes) throw();

template <class T> T *alloc(size_t n_elements) throw() {
  return reinterpret_cast<T *>(alloc_bytes(n_elements * sizeof(T)));
}

template <class T> void clear(T *addr, size_t n_elements) throw() {
  clear_bytes(addr, n_elements * sizeof(T));
}

template <class T> void free(T *addr) throw() {
  free_bytes(addr);
}

template <class T> T *realloc(T *addr, size_t n_elements) throw() {
  return reinterpret_cast<T *>(realloc_bytes(addr, n_elements * sizeof(T)));
}

template <class T> void swap(T &a, T &b) {
  T tmp = std::move(a);
  a = std::move(b);
  b = std::move(tmp);
}

} // namespace mem
} // namespace onestl

#endif /* ONESTL_MEM_HPP */
