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
 * arrayhelper.cpp
 *
 *  Created on Jan 24, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/arrayhelper.hpp>
#include <onestl/mem.hpp>

#include <cstring>

namespace onestl {

ONESTL_NOINLINE bool ArrayHelper::init(size_t n, size_t sizeofT) noexcept {
  n *= sizeofT;
  data_ = mem::alloc<char>(n);
  if (n && !data_) {
    // mem::alloc() failed
    data_ = NULL;
    cap_ = size_ = 0;
    return false;
  }
  cap_ = size_ = n;
  return true;
}

ONESTL_NOINLINE bool ArrayHelper::grow(size_t n, size_t sizeofT, bool zerofill) noexcept {
  if (!grow_cap(n, sizeofT)) {
    return false;
  }
  if (zerofill && n > size_) {
    mem::clear<char>(data_ + size_ * sizeofT, (n - size_) * sizeofT);
  }
  size_ = n;
  return true;
}

bool ArrayHelper::grow_cap(size_t mincap, size_t sizeofT) noexcept {
  if (cap_ >= mincap) {
    return true;
  }
  const size_t cap2 = cap_ >= 10 ? cap_ * 2 : 10;
  return reserve(mincap >= cap2 ? mincap : cap2, sizeofT);
}

ONESTL_NOINLINE bool ArrayHelper::reserve(size_t newcap, size_t sizeofT) noexcept {
  if (newcap > cap_) {
    char *newdata = mem::realloc<char>(data_, newcap * sizeofT);
    if (!newdata) {
      if (cap_ == 0) {
        data_ = NULL;
      }
      return false;
    }
    data_ = newdata;
    cap_ = newcap;
  }
  return true;
}

ONESTL_NOINLINE bool ArrayHelper::dup(const void *addr, size_t n, size_t sizeofT) noexcept {
  if (n != 0) {
    if (!grow_cap(n, sizeofT)) {
      return false;
    }
    std::memcpy(data_, addr, n * sizeofT);
  }
  size_ = n;
  return true;
}

ONESTL_NOINLINE bool ArrayHelper::append(const void *addr, size_t n, size_t sizeofT) noexcept {
  if (n != 0) {
    const size_t oldn = size_;
    if (!grow(oldn + n, sizeofT, false)) {
      return false;
    }
    std::memcpy(data_ + oldn * sizeofT, addr, n * sizeofT);
  }
  return true;
}

} // namespace onestl
