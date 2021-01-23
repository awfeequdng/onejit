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
 * buffer.hpp
 *
 *  Created on Jan 23, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_BUFFER_HPP
#define ONESTL_BUFFER_HPP

#include <onestl/vector.hpp>

namespace onestl {

// dynamically resizeable buffer of arbitrary type T,
// which keeps track of allocation failures.
//
// It has the same constraints as Vector<T>:
// 1. T must have trivial copy constructor, destructor and assignment operator
// 2. zero-initializing T must be produce a valid T instance
template <class T> class Buffer : public Vector<T> {

  typedef Vector<T> Base;

public:
  constexpr Buffer() noexcept : Base{}, good_{true} {
  }
  Buffer(const T *addr, size_t n) noexcept : Base{}, good_{Base::dup(addr, n)} {
  }
  // all one-argument constructors are explicit because they allocate,
  // thus they mail fail => we require users to explicitly invoke them.
  explicit Buffer(size_t n) noexcept : Base{}, good_{Base::init(n)} {
  }
  explicit Buffer(const View<T> &other) noexcept : Base{}, good_{Base::dup(other)} {
  }
  explicit Buffer(const Span<T> &other) noexcept : Base{}, good_{Base::dup(other)} {
  }
  explicit Buffer(const Vector<T> &other) noexcept : Base{}, good_{Base::dup(other)} {
  }
  explicit Buffer(const Buffer<T> &other) noexcept : Base{}, good_{Base::dup(other)} {
  }
  Buffer(Buffer<T> &&other) noexcept : Base{}, good_{true} {
    swap(other);
  }

  ~Buffer() noexcept = default;

  Buffer<T> &operator=(Buffer<T> &&other) noexcept {
    swap(other);
    return *this;
  }

  constexpr explicit operator bool() const noexcept {
    return good_;
  }

  bool resize(size_t n) noexcept {
    return (good_ = good_ && Base::resize(n));
  }

  bool reserve(size_t newcap) noexcept {
    return (good_ = good_ && Base::reserve(newcap));
  }

  bool append(const T &src) noexcept {
    return (good_ = good_ && Base::append(src));
  }

  bool append(View<T> src) noexcept {
    return (good_ = good_ && Base::append(src));
  }

  void swap(Buffer &other) noexcept {
    Base::swap(other);
    mem::swap(good_, other.good_);
  }

private:
  bool good_;
};

template <class T> void swap(Buffer<T> &left, Buffer<T> &right) noexcept {
  left.swap(right);
}

} // namespace onestl

#endif /* ONESTL_BUFFER_HPP */
