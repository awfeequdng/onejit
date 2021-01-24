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
 * vectorhelper.hpp
 *
 *  Created on Jan 24, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_VECTORHELPER_HPP
#define ONESTL_VECTORHELPER_HPP

#include <onestl/fwd.hpp>

#include <cstddef> // size_t

namespace onestl {

class VectorHelper {

  template <class T> friend class Vector;

private:
  template <class T> static VectorHelper &cast(Vector<T> &v) noexcept {
    static_assert(sizeof(Vector<T>) == sizeof(VectorHelper),
                  "Vector<T> and VectorHelper must have the same sizeof()");

    return reinterpret_cast<VectorHelper &>(v);
  }

  bool init(size_t n, size_t sizeofT) noexcept;

  bool grow_capacity(size_t mincap, size_t sizeofT) noexcept;

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

#endif // ONESTL_VECTORHELPER_HPP
