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
 * array.hpp - adapted from twin/include/stl/vector.h
 *
 *  Created on Jan 11, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_ARRAY_HPP
#define ONESTL_ARRAY_HPP

#include <onestl/arrayhelper.hpp>
#include <onestl/mem.hpp>
#include <onestl/span.hpp>

#include <cstdint>     // uint32_t
#include <type_traits> // std::is_trivial<T>

namespace onestl {

/**
 * Dynamically resizeable array of arbitrary type T, with two constraints:
 * 1. T must have trivial copy constructor, destructor and assignment operator
 * 2. zero-initializing T must be produce a valid T instance
 *
 * Iterator invalidation:
 *
 * The following methods always invalidate *all* references, pointers, iterators,
 * spans and views to the array elements:
 *   operator=  dup  swap
 *
 * The following methods will invalidate *all* references, pointers, iterators,
 * spans and views to the array elements *only* if they change the array's capacity:
 *   append reserve resize
 *
 * Some methods may change the array's size, while preserving its capacity.
 * Doing so invalidates the references, pointers, iterators, spans and views
 * to the array elements *at or after* the minimum between the old and new size.
 * The following methods may change the size, and either preserve or change the capacity:
 *   append resize
 * The following methods may change the size, but always preserve the capacity:
 *   clear truncate
 *
 * For increased safety, Array::operator[] returns T by value, not by reference.
 * Reason: references can be invalidated by one of the methods above.
 * To overwrite Array elements, use Array::set()
 */
template <class T> class Array : protected Span<T> {
  static_assert(std::is_nothrow_default_constructible<T>::value,
                "Array<T>: element type T default constructor must be nothrow");
  static_assert(std::is_trivially_copyable<T>::value,
                "Array<T>: element type T must be trivially copyable");
  static_assert(std::is_trivially_destructible<T>::value,
                "Array<T>: element type T must be trivially destructible");

  typedef Span<T> Base;
  friend class ArrayHelper;
  template <class T2> friend class CRange;
  template <class T2> friend class Range;

  // do not implement. reason: any allocation failure would not be visible
  Array<T> &operator=(const Array<T> &other) noexcept = delete;

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

  constexpr Array() noexcept : Base{}, cap_{0} {
  }
  Array(const T *addr, size_t n) noexcept : Base{}, cap_{0} {
    dup(addr, n);
  }
  // all one-argument constructors are explicit because they allocate,
  // thus they mail fail => we require users to explicitly invoke them.
  explicit Array(size_t n) noexcept : Base{}, cap_{0} {
    init(n);
  }
  explicit Array(const View<T> &other) noexcept : Base{}, cap_{0} {
    dup(other.data(), other.size());
  }
  explicit Array(const Span<T> &other) noexcept : Base{}, cap_{0} {
    dup(other.data(), other.size());
  }
  explicit Array(const Array<T> &other) noexcept : Base{}, cap_{0} {
    dup(other.data(), other.size());
  }
  Array(Array<T> &&other) noexcept : Base{}, cap_{} {
    swap(other);
  }

  ~Array() noexcept {
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

  Array<T> &operator=(Array<T> &&other) noexcept {
    swap(other);
    return *this;
  }

  bool dup(const T *addr, size_t n) noexcept {
    if (n == 0) {
      size_ = 0;
      return true;
    }
    return ArrayHelper::cast(*this).dup(addr, n, sizeof(T));
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
    return ArrayHelper::cast(*this).reserve(newcap, sizeof(T));
  }

  bool append(const T &src) noexcept {
    return append(View<T>{&src, 1});
  }
  bool append(View<T> src) noexcept {
    return ArrayHelper::cast(*this).append(src.data(), src.size(), sizeof(T));
  }

  void swap(Array<T> &other) noexcept {
    ArrayHelper &h_this = ArrayHelper::cast(*this);
    ArrayHelper &h_other = ArrayHelper::cast(other);
    ArrayHelper h_tmp = h_this;
    h_this = h_other;
    h_other = h_tmp;
  }

protected:
  bool init(size_t n) noexcept {
    return ArrayHelper::cast(*this).init(n, sizeof(T));
  }

  void destroy() noexcept {
    if (data_ != NULL) {
      mem::free(data());
    }
  }

  bool grow(size_t n, bool zerofill) noexcept {
    return ArrayHelper::cast(*this).grow(n, sizeof(T), zerofill);
  }

  using Base::data_;
  using Base::size_;
  size_t cap_;
};

extern template class Array<char>;     // defined in onestl/string.cpp
extern template class Array<uint32_t>; // defined in onestl/array.cpp

template <class T> void swap(Array<T> &left, Array<T> &right) noexcept {
  left.swap(right);
}

// ------------- View<T> methods requiring complete type Array<T> ---------------

template <class T>
constexpr View<T>::View(const Array<T> &other) noexcept //
    : data_{other.data()}, size_{other.size()} {
}

template <class T> void View<T>::ref(const Array<T> &other) noexcept {
  data_ = other.data();
  size_ = other.size();
}

} // namespace onestl

#endif // ONESTL_ARRAY_HPP
