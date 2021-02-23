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
#include <onestl/vectorhelper.hpp>

#include <cstdint>     // uint32_t
#include <type_traits> // std::is_trivial<T>

namespace onestl {

/**
 * Dynamically resizeable vector of arbitrary type T, with two constraints:
 * 1. T must have trivial copy constructor, destructor and assignment operator
 * 2. zero-initializing T must be produce a valid T instance
 *
 * Iterator invalidation:
 *
 * The following methods always invalidate *all* references, pointers, iterators,
 * spans and views to the vector elements:
 *   operator=  dup  swap
 *
 * The following methods will invalidate *all* references, pointers, iterators,
 * spans and views to the vector elements *only* if they change the vector's capacity:
 *   append reserve resize
 *
 * Some methods may change the vector's size, while preserving its capacity.
 * Doing so invalidates the references, pointers, iterators, spans and views
 * to the vector elements *at or after* the minimum between the old and new size.
 * The following methods may change the size, and either preserve or change the capacity:
 *   append resize
 * The following methods may change the size, but always preserve the capacity:
 *   clear truncate
 *
 * For increased safety, Vector::operator[] returns T by value, not by reference.
 * Reason: references can be invalidated by one of the methods above.
 * To overwrite Vector elements, use Vector::set()
 */
template <class T> class Vector : protected Span<T> {
  static_assert(std::is_nothrow_default_constructible<T>::value,
                "Vector<T>: element type T default constructor must be nothrow");
  static_assert(std::is_trivially_copyable<T>::value,
                "Vector<T>: element type T must be trivially copyable");
  static_assert(std::is_trivially_destructible<T>::value,
                "Vector<T>: element type T must be trivially destructible");

  typedef Span<T> Base;
  friend class VectorHelper;
  template <class T2> friend class CRange;
  template <class T2> friend class Range;

  // do not implement. reason: any allocation failure would not be visible
  Vector<T> &operator=(const Vector<T> &other) noexcept = delete;

protected:
  using Base::data_;
  using Base::size_;
  size_t cap_;

  bool init(size_t n) noexcept {
    return VectorHelper::cast(*this).init(n, sizeof(T));
  }

  void destroy() noexcept {
    if (data_ != NULL) {
      mem::free(data());
    }
  }

  bool grow_capacity(size_t n) noexcept {
    if (cap_ >= n) {
      return true;
    }
    return VectorHelper::cast(*this).grow_capacity(n, sizeof(T));
  }

  bool grow(size_t n, bool zerofill) noexcept {
    return VectorHelper::cast(*this).grow(n, sizeof(T), zerofill);
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
  using Base::set;
  using Base::operator==;
  using Base::operator[];
  using Base::clear;
  using Base::size;
  using Base::span;
  using Base::truncate;
  using Base::view;

  Vector<T> &operator=(Vector<T> &&other) noexcept {
    swap(other);
    return *this;
  }

  bool dup(const T *addr, size_t n) noexcept {
    if (n == 0) {
      size_ = 0;
      return true;
    }
    return VectorHelper::cast(*this).dup(addr, n, sizeof(T));
  }
  bool dup(View<T> other) noexcept {
    return dup(other.data(), other.size());
  }

  bool resize(size_t n) noexcept {
    if (size_ >= n) {
      size_ = n;
      return true;
    }
    return grow(n, true);
  }

  bool reserve(size_t newcap) noexcept {
    if (newcap <= cap_) {
      return true;
    }
    return VectorHelper::cast(*this).reserve(newcap, sizeof(T));
  }

  bool append(const T &src) noexcept {
    return append(View<T>{&src, 1});
  }
  bool append(View<T> src) noexcept {
    return VectorHelper::cast(*this).append(src.data(), src.size(), sizeof(T));
  }

  void swap(Vector<T> &other) noexcept {
    VectorHelper &h_this = VectorHelper::cast(*this);
    VectorHelper &h_other = VectorHelper::cast(other);
    VectorHelper h_tmp = h_this;
    h_this = h_other;
    h_other = h_tmp;
  }
};

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

#endif // ONESTL_VECTOR_HPP
