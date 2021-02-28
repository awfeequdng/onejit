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

#include <onejit_config.h> // HAVE_*
#include <onestl/bitset.hpp>
#include <onestl/mem.hpp>

#include <cstring>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

namespace onestl {

#ifdef HAVE_FFS
inline unsigned find_first_set(unsigned i) noexcept {
  return unsigned(::ffs(int(i)));
}
#else  // !HAVE_FFS
static unsigned find_first_set(unsigned i) noexcept {
  unsigned ret = 0;
  while (i) {
    if (uint8_t(i)) {
      while ((i & 1) == 0) {
        ret++;
        i >>= 1;
      }
      return ret;
    } else {
      ret += 8;
      i >>= 8;
    }
  }
  return 0;
}
#endif // HAVE_FFS

#if defined(HAVE_FFSL)
inline unsigned find_first_set(unsigned long i) {
  return unsigned(::ffsl(long(i)));
}
#elif defined(HAVE_FFS)
inline unsigned find_first_set(unsigned long i) {
  if (int lo = ::ffs(int(i))) {
    return unsigned(lo);
  }
  unsigned shift = 8 * sizeof(int);
  if (int hi = ::ffs(int(i >> shift))) {
    return unsigned(hi) + shift;
  }
  return 0;
}
#else
static unsigned find_first_set(unsigned long i) noexcept {
  unsigned ret = 0;
  while (i) {
    if (uint8_t(i)) {
      while ((i & 1) == 0) {
        ret++;
        i >>= 1;
      }
      return ret;
    } else {
      ret += 8;
      i >>= 8;
    }
  }
  return 0;
}
#endif // !HAVE_FFSL

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

void BitSet::set(Index index, bool value) noexcept {
  if (index >= size_) {
    return;
  }
  const T fillmask = T(1) << (index % bitsPerT);
  const T keepmask = T(~fillmask);
  const T pattern = T(-T(value)); // 0 or 0xff..ff
  T &ref = data_[index / bitsPerT];
  ref = (ref & keepmask) | (pattern & fillmask);
}

BitSet::Index BitSet::first_set(Index start, Index end) const noexcept {
  if (end > size_) {
    end = size_;
  }
  if (start < end) {
    T bits = data_[start / bitsPerT] >> (start % bitsPerT);
    if (bits) {
      start += find_first_set(bits) - 1;
      return start < end ? start : NoPos;
    }
    start = bitsPerT + start / bitsPerT * bitsPerT;
    while (start < end) {
      bits = data_[start / bitsPerT];
      if (bits) {
        start |= find_first_set(bits) - 1;
        return start < end ? start : NoPos;
      }
      start += bitsPerT;
    }
  }
  return NoPos;
}

void BitSet::fill(Index start, Index end, bool value) noexcept {
  if (end > size_) {
    end = size_;
  }
  if (start >= end) {
    return;
  }
  const T pattern = T(-T(value)); // 0 or 0xff..ff

  {
    size_t nstart = start / bitsPerT;
    size_t nend = end / bitsPerT;
    if (nstart == nend) {
      // fill a single T or a fragment of it
      size_t head = start % bitsPerT;
      T fillmask1 = T(~T(0)) << head; // n high bits set
      T keepmask1 = T(~fillmask1);    // n'=head low bits set

      size_t tail = end % bitsPerT;
      T keepmask2 = T(~T(0)) << tail; // n high bits set
      T fillmask2 = T(~keepmask2);    // n'=tail low bits set

      T keepmask = keepmask1 | keepmask2;
      T fillmask = fillmask1 & fillmask2;

      T &ref = data_[start / bitsPerT];
      ref = (ref & keepmask) | (pattern & fillmask);
      return;
    }
  }

  if (size_t head = start % bitsPerT) {
    // fill unaligned head fragment
    T fillmask = T(~T(0)) << head; // n high bits set
    T keepmask = T(~fillmask);     // n'=head low bits set
    T &ref = data_[start / bitsPerT];
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
    T keepmask = T(~T(0)) << tail; // n high bits set
    T fillmask = T(~keepmask);     // n'=tail low bits set
    T &ref = data_[end / bitsPerT];
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

void BitSet::copy(const BitSet &src) {
  if (this != &src) {
    ONESTL_BOUNDS_TINY(src.size(), ==, size());
    std::memcpy(data_, src.data_, (size_ + bitsPerT - 1) / bitsPerT * sizeofT);
  }
}

} // namespace onestl
