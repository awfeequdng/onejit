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

#include <cstring>

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

void BitSet::fill(size_t start, size_t end, bool value) noexcept {
  if (end > size_) {
    end = size_;
  }
  if (start >= end) {
    return;
  }
  const size_t pattern = -T(value); // 0 or 0xffff....ffff

  {
    size_t nstart = start / bitsPerT;
    size_t nend = end / bitsPerT;
    if (nstart == nend) {
      // fill a single T or a fragment of it
      size_t head = start % bitsPerT;
      size_t fillmask1 = ~T(0) << head; // n high bits set
      size_t keepmask1 = ~fillmask1;    // n'=head low bits set

      size_t tail = end % bitsPerT;
      size_t keepmask2 = ~T(0) << tail; // n high bits set
      size_t fillmask2 = ~keepmask2;    // n'=tail low bits set

      size_t keepmask = keepmask1 | keepmask2;
      size_t fillmask = fillmask1 & fillmask2;

      size_t &ref = data_[start / bitsPerT];
      ref = (ref & keepmask) | (pattern & fillmask);
      return;
    }
  }

  if (size_t head = start % bitsPerT) {
    // fill unaligned head fragment
    size_t fillmask = ~T(0) << head; // n high bits set
    size_t keepmask = ~fillmask;     // n'=head low bits set
    size_t &ref = data_[start / bitsPerT];
    ref = (ref & keepmask) | (pattern & fillmask);
    start = start - head + bitsPerT;
  }
  // start is now a multiple of bitsPerT
  {
    // fill bulk
    size_t nstart = start / bitsPerT;
    size_t nend = end / bitsPerT;
    if (nstart < nend) {
      std::memset(data_ + nstart, uint8_t(pattern), (nend - nstart) * sizeofT);
      start = nend * bitsPerT;
    }
  }
  // start is still a multiple of bitsPerT
  if (size_t tail = end % bitsPerT) {
    // fill unaligned tail fragment
    size_t keepmask = ~T(0) << tail; // n high bits set
    size_t fillmask = ~keepmask;     // n'=tail low bits set
    size_t &ref = data_[end / bitsPerT];
    ref = (ref & keepmask) | (pattern & fillmask);
  }
}

bool BitSet::grow(size_t newsize, bool zerofill) noexcept {
  if (!grow_cap(newsize)) {
    return false;
  }
  const size_t oldsize = size_;
  size_ = newsize;
  if (zerofill && newsize > oldsize) {
    fill(oldsize, newsize, false);
  }
  return true;
}

bool BitSet::grow_cap(size_t mincap) noexcept {
  if (cap_ >= mincap) {
    return true;
  }
  const size_t cap2 = cap_ >= bitsPerT ? cap_ * 2 : bitsPerT;
  return realloc(mincap >= cap2 ? mincap : cap2);
}

ONESTL_NOINLINE bool BitSet::realloc(size_t newcap) noexcept {
  const size_t oldn = (cap_ + bitsPerT - 1) / bitsPerT;
  const size_t n = (newcap + bitsPerT - 1) / bitsPerT;
  if (n > oldn) {
    T *newdata = mem::realloc(data_, n);
    if (!newdata) {
      return false;
    }
    data_ = newdata;
    cap_ = n * bitsPerT;
  } else {
    cap_ = oldn * bitsPerT;
  }
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
