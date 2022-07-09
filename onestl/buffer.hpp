/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * buffer.hpp
 *
 *  Created on Jan 23, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_BUFFER_HPP
#define ONESTL_BUFFER_HPP

#include <onestl/array.hpp>

namespace onestl {

// dynamically resizeable buffer of arbitrary type T,
// which keeps track of allocation failures.
//
// It has the same constraints as Array<T>:
// 1. T must have trivial copy constructor, destructor and assignment operator
// 2. zero-initializing T must be produce a valid T instance
template <class T> class Buffer : public Array<T> {

  typedef Array<T> Base;

public:
  constexpr Buffer() noexcept : Base{}, good_{true} {
  }
  Buffer(const T *addr, size_t n) noexcept : Base{}, good_{Base::dup(addr, n)} {
  }
  // all one-argument constructors are explicit because they allocate,
  // thus they mail fail => we require users to explicitly invoke them.
  explicit Buffer(size_t n) noexcept : Base{}, good_{Base::init(n)} {
  }
  explicit Buffer(View<T> other) noexcept : Base{}, good_{Base::dup(other)} {
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

  Buffer<T> &clearerr() noexcept {
    good_ = true;
    return *this;
  }

  Buffer<T> &seterr() noexcept {
    good_ = false;
    return *this;
  }

  Buffer<T> &clear() noexcept {
    Base::clear();
    return clearerr();
  }

  Buffer<T> &truncate(size_t n) noexcept {
    Base::truncate(n);
    return *this;
  }

  Buffer<T> &resize(size_t n) noexcept {
    good_ = good_ && Base::resize(n);
    return *this;
  }

  Buffer<T> &reserve(size_t newcap) noexcept {
    good_ = good_ && Base::reserve(newcap);
    return *this;
  }

  Buffer<T> &append(const T &src) noexcept {
    good_ = good_ && Base::append(src);
    return *this;
  }

  Buffer<T> &append(View<T> src) noexcept {
    good_ = good_ && Base::append(src);
    return *this;
  }

  void swap(Buffer &other) noexcept {
    Base::swap(other);
    mem::swap(good_, other.good_);
  }

protected:
  bool good_;
};

template <class T> void swap(Buffer<T> &left, Buffer<T> &right) noexcept {
  left.swap(right);
}

} // namespace onestl

#endif // ONESTL_BUFFER_HPP
