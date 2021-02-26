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
 * bitset.cpp
 *
 *  Created on Feb 26, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/bitset.hpp>
#include <onestl/mem.hpp>

namespace onestl {

BitSet::BitSet(BitSet &&other) noexcept //
    : data_(other.data_), size_(other.size_), cap_(other.cap_) {
  other.data_ = NULL;
  other.cap_ = other.size_ = 0;
}

ONESTL_NOINLINE bool BitSet::init(size_t size) noexcept {
  const size_t n = (size + bitsPerT - 1) / bitsPerT;
  data_ = NULL;
  if (n && !(data_ = mem::alloc_clear<T>(n))) {
    // mem::alloc_clear() failed
    cap_ = size_ = 0;
    return false;
  }
  size_ = size;
  cap_ = n * bitsPerT;
  return true;
}

ONESTL_NOINLINE void BitSet::destroy() noexcept {
  T *data = data_;
  data_ = NULL;
  cap_ = size_ = 0;
  mem::free(data);
}

void BitSet::set(size_t index, bool value) noexcept {
  if (index >= size_) {
    return;
  }
  const size_t mask = size_t(1) << (index % bitsPerT);
  T &ref = data_[index / bitsPerT];
  if (value) {
    ref |= mask;
  } else {
    ref &= ~mask;
  }
}

bool BitSet::grow(size_t newsize) noexcept {
  if (!realloc(newsize)) {
    return false;
  }
  size_ = newsize;
  return true;
}

bool BitSet::realloc(size_t newcap) noexcept {
  if (newcap < bitsPerT) {
    newcap = bitsPerT;
  }
  if (newcap < cap_ * 2) {
    newcap = cap_ * 2;
  }
  const size_t oldn = cap_ / bitsPerT;
  const size_t n = (newcap + bitsPerT - 1) / bitsPerT;
  T *data = mem::realloc(data_, n);
  if (!data) {
    return false;
  }
  mem::clear(data + oldn, n - oldn);
  data_ = data;
  cap_ = n * bitsPerT;
  return true;
}

void BitSet::swap(BitSet &other) noexcept {
  if (this != &other) {
    mem::swap(data_, other.data_);
    mem::swap(size_, other.size_);
    mem::swap(cap_, other.cap_);
  }
}

} // namespace onestl
