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
 * bitset.hpp
 *
 *  Created on Feb 26, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_BITSET_HPP
#define ONESTL_BITSET_HPP

#include <onestl/fwd.hpp>
#include <onestl/mem.hpp>
#include <onestl/test_tiny.hpp>

#include <cstddef> // size_t

namespace onestl {

/** resizeable bit sequence */
class BitSet {

private:
  typedef size_t T;

  enum : size_t {
    sizeofT = sizeof(T),
    bitsPerT = sizeofT * 8, // * CHAR_BIT
  };

  T *data_;
  size_t size_; // in bits
  size_t cap_;  // in bits

public:
  typedef size_t Index;

  enum : size_t { NoPos = size_t(-1) };

  constexpr BitSet() noexcept : data_(NULL), size_(0), cap_(0) {
  }
  explicit BitSet(size_t size) noexcept {
    init(size);
  }

  BitSet(const BitSet &other) = delete;
  BitSet(BitSet &&other) noexcept;

  ~BitSet() noexcept {
    destroy();
  }

  BitSet &operator=(const BitSet &other) = delete;
  BitSet &operator=(BitSet &&other) noexcept {
    swap(other);
    return *this;
  }

  constexpr bool empty() const noexcept {
    return size_ == 0;
  }

  constexpr size_t size() const noexcept {
    return size_;
  }

  // maximum number of bits settable without resizing
  constexpr size_t capacity() const noexcept {
    return cap_;
  }

  constexpr explicit operator bool() const noexcept {
    return size_ != 0;
  }

  void clear() noexcept {
    size_ = 0;
  }

  void truncate(size_t newsize) noexcept {
    if (size_ > newsize) {
      size_ = newsize;
    }
  }

  // checked element access:
  // return i-th bit, or false if index is out of bounds
  constexpr bool operator[](Index index) const noexcept {
    return index < size_ ? get(data_, index) : false;
  }

  // set or clear i-th bit. does nothing if index is out of bounds.
  void set(Index index, bool value) noexcept;

  // return index of first non-zero bit between 'start' and 'end'-1
  // return NoPos if all bits in such range are zero
  Index first_set(Index start = 0, Index end = NoPos) const noexcept;

  // set or clear all bits from start to end.
  void fill(Index start, Index end, bool value) noexcept;

  // resize bitset, changing its size.
  // return false if out of memory.
  bool resize(size_t n) noexcept {
    if (size_ >= n) {
      size_ = n;
      return true;
    }
    return grow(n, true);
  }

  // increase bitset capacity.
  // return false if out of memory.
  bool reserve(size_t newcap) noexcept {
    if (newcap <= cap_) {
      return true;
    }
    return realloc(newcap);
  }

  void swap(BitSet &other) noexcept {
    mem::swap(data_, other.data_);
    mem::swap(size_, other.size_);
    mem::swap(cap_, other.cap_);
  }

private:
  bool init(size_t size) noexcept;
  void destroy() noexcept;
  bool grow(size_t newsize, bool zerofill) noexcept;
  bool grow_cap(size_t mincap) noexcept;
  bool realloc(size_t newcap) noexcept;

  static constexpr bool get(const T *data, Index index) noexcept {
    return bool(1 & (data[index / bitsPerT] >> (index % bitsPerT)));
  }
};

inline void swap(BitSet &left, BitSet &right) noexcept {
  left.swap(right);
}

} // namespace onestl

#endif // ONESTL_BITSET_HPP
