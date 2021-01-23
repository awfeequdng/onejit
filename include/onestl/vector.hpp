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
 * vector.hpp - adapted from twin/include/stl/vector.h
 *
 *  Created on Jan 11, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_VECTOR_HPP
#define ONESTL_VECTOR_HPP

#include <onestl/mem.hpp>
#include <onestl/span.hpp>

#include <cstdint>     // uint32_t
#include <type_traits> // std::is_trivial<T>

namespace onestl {

// dynamically resizeable vector of arbitrary type T, with two constraints:
// 1. T must have trivial copy constructor, destructor and assignment operator
// 2. zero-initializing T must be produce a valid T instance
template <class T> class Vector : protected Span<T> {
  static_assert(std::is_trivially_copyable<T>::value,
                "Vector<T>: element type T must be trivially copyable");
  static_assert(std::is_trivially_destructible<T>::value,
                "Vector<T>: element type T must be trivially destructible");

  typedef Span<T> Base;

  // do not implement. reason: any allocation failure would not be visible
  Vector<T> &operator=(const Vector<T> &other) noexcept = delete;

protected:
  using Base::data_;
  using Base::size_;
  size_t cap_;

  bool init(size_t n) noexcept {
    data_ = mem::alloc<T>(n);
    if (n && !data_) {
      // mem::alloc() failed
      data_ = NULL;
      cap_ = size_ = 0;
      return false;
    }
    cap_ = size_ = n;
    return true;
  }

  void destroy() noexcept {
    if (data_ != NULL) {
      mem::free(data());
    }
  }

  bool ensure_capacity(size_t n) noexcept {
    if (cap_ >= n) {
      return true;
    }
    const size_t cap2 = cap_ >= 10 ? cap_ * 2 : 10;
    return reserve(n >= cap2 ? n : cap2);
  }

  bool ONESTL_NOINLINE resize0(size_t n, bool zerofill) noexcept {
    if (!ensure_capacity(n)) {
      return false;
    }
    if (zerofill && n > size_) {
      mem::clear(data() + size_, n - size_);
    }
    size_ = n;
    return true;
  }

public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef T &reference;
  typedef T *pointer;
  typedef T *iterator;
  typedef const T &const_reference;
  typedef const T *const_pointer;
  typedef const T *const_iterator;

  constexpr Vector() noexcept : Base{}, cap_{0} {
  }
  Vector(const T *addr, size_t n) noexcept : Base{}, cap_{0} {
    dup(addr, n);
  }
  // all one-argument constructors are explicit because they allocate,
  // thus they mail fail => we require users to explicitly invoke them.
  explicit Vector(size_t n) noexcept : Base{}, cap_{0} {
    init(n);
  }
  explicit Vector(const View<T> &other) noexcept : Base{}, cap_{0} {
    dup(other.data(), other.size());
  }
  explicit Vector(const Span<T> &other) noexcept : Base{}, cap_{0} {
    dup(other.data(), other.size());
  }
  explicit Vector(const Vector<T> &other) noexcept : Base{}, cap_{0} {
    dup(other.data(), other.size());
  }
  Vector(Vector<T> &&other) noexcept : Base{}, cap_{} {
    swap(other);
  }

  ~Vector() noexcept {
    destroy();
  }

  constexpr size_t capacity() const noexcept {
    return cap_;
  }
  using Base::begin;
  using Base::copy;
  using Base::data;
  using Base::empty;
  using Base::end;
  using Base::operator[];
  using Base::operator==;
  using Base::size;
  using Base::span;
  using Base::view;

  Vector<T> &operator=(Vector<T> &&other) noexcept {
    swap(other);
    return *this;
  }

  bool ONESTL_NOINLINE dup(const T *addr, size_t n) noexcept {
    if (!ensure_capacity(n)) {
      return false;
    }
    std::memcpy(data(), addr, n * sizeof(T));
    size_ = n;
    return true;
  }
  bool dup(View<T> other) noexcept {
    return dup(other.data(), other.size());
  }
  bool dup(Span<T> other) noexcept {
    return dup(other.data(), other.size());
  }
  bool dup(const Vector &other) noexcept {
    return dup(other.data(), other.size());
  }

  void clear() noexcept {
    (void)resize0(0, false);
  }

  bool resize(size_t n) noexcept {
    return resize0(n, true);
  }

  bool reserve(size_t newcap) noexcept {
    if (newcap > cap_) {
      T *olddata = data();
      T *newdata = mem::realloc(olddata, newcap);
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

  void truncate(size_t n) noexcept {
    if (size_ > n) {
      size_ = n;
    }
  }

  bool append(const T &src) noexcept {
    return append(View<T>(&src, 1));
  }

  bool ONESTL_NOINLINE append(View<T> src) noexcept {
    const size_t oldn = size_;
    const size_t srcn = src.size();
    if (!resize0(oldn + srcn, false)) {
      return false;
    }
    span(oldn, size()).copy(src);
    return true;
  }

  void swap(Vector &other) noexcept {
    mem::swap(data_, other.data_);
    mem::swap(size_, other.size_);
    mem::swap(cap_, other.cap_);
  }
};

typedef Vector<char> CharVec;

extern template class Vector<char>;     // defined in onestl/string.cpp
extern template class Vector<uint32_t>; // defined in onestl/vector.cpp

template <class T> void swap(Vector<T> &left, Vector<T> &right) noexcept {
  left.swap(right);
}

// ------------- View<T> methods requiring complete type Vector<T> ---------------

template <class T>
constexpr View<T>::View(const Vector<T> &other) noexcept //
    : data_{other.data()}, size_{other.size()} {
}

template <class T> void View<T>::ref(const Vector<T> &other) noexcept {
  data_ = other.data();
  size_ = other.size();
}

} // namespace onestl

#endif /* ONESTL_VECTOR_HPP */
