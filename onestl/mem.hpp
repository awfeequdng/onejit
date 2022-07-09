/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * mem.hpp
 *
 *  Created on Jan 11, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_MEM_HPP
#define ONESTL_MEM_HPP

#include <cstddef> // size_t
#include <utility> // std::move()

namespace onestl {
namespace mem {

void *alloc_bytes(size_t n_bytes) noexcept;
void *alloc_clear_bytes(size_t n_bytes) noexcept;
void clear_bytes(void *addr, size_t n_bytes) noexcept;
void free_bytes(void *addr) noexcept;
void *realloc_bytes(void *addr, size_t new_n_bytes) noexcept;

template <class T> T *alloc(size_t n_elements) noexcept {
  return reinterpret_cast<T *>(alloc_bytes(n_elements * sizeof(T)));
}

template <class T> T *alloc_clear(size_t n_elements) noexcept {
  return reinterpret_cast<T *>(alloc_clear_bytes(n_elements * sizeof(T)));
}

template <class T> void clear(T *addr, size_t n_elements) noexcept {
  clear_bytes(addr, n_elements * sizeof(T));
}

template <class T> void free(T *addr) noexcept {
  free_bytes(addr);
}

template <class T> T *realloc(T *addr, size_t n_elements) noexcept {
  return reinterpret_cast<T *>(realloc_bytes(addr, n_elements * sizeof(T)));
}

template <class T> void swap(T &a, T &b) noexcept {
  static_assert(std::is_nothrow_move_constructible<T>::value,
                "onestl::mem::swap<T>: type T must be nothrow_move_constructible");
  static_assert(std::is_nothrow_move_assignable<T>::value,
                "onestl::mem::swap<T>: type T must be nothrow_move_assignable");

  T tmp = std::move(a);
  a = std::move(b);
  b = std::move(tmp);
}

} // namespace mem
} // namespace onestl

#endif // ONESTL_MEM_HPP
