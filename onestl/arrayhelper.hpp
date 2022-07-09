/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * arrayhelper.hpp
 *
 *  Created on Jan 24, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_ARRAYHELPER_HPP
#define ONESTL_ARRAYHELPER_HPP

#include <onestl/fwd.hpp>

#include <cstddef> // size_t

namespace onestl {

class ArrayHelper {

  template <class T> friend class Array;

private:
  template <class T> static ArrayHelper &cast(Array<T> &v) noexcept {
    static_assert(sizeof(Array<T>) == sizeof(ArrayHelper),
                  "Array<T> and ArrayHelper must have the same sizeof()");

    return reinterpret_cast<ArrayHelper &>(v);
  }

  bool init(size_t n, size_t sizeofT) noexcept;

  bool grow_cap(size_t mincap, size_t sizeofT) noexcept;

  bool reserve(size_t newcap, size_t sizeofT) noexcept;

  bool grow(size_t n, size_t sizeofT, bool zerofill) noexcept;

  bool dup(const void *addr, size_t n, size_t sizeofT) noexcept;

  bool append(const void *addr, size_t n, size_t sizeofT) noexcept;

private:
  char *data_;
  size_t size_;
  size_t cap_;
};

} // namespace onestl

#endif // ONESTL_ARRAYHELPER_HPP
