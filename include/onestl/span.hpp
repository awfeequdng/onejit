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
 * span.hpp - adapted from twin/include/stl/span.h
 *
 *  Created on Jan 11, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_SPAN_HPP
#define ONESTL_SPAN_HPP

#include <onestl/view.hpp>

namespace onestl {

/** non-owning, read-write span of T[] */
template <class T> class Span : protected View<T> {
private:
  typedef View<T> Base;

protected:
  using Base::data_;
  using Base::size_;

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

  constexpr Span() noexcept : Base{} {
  }
  constexpr Span(T *addr, size_t n) noexcept : Base{addr, n} {
  }
  constexpr Span(Vector<T> &other) noexcept : Base{other} {
  }

  // Span(const Span&) = default;
  // ~Span() = default;
  // operator=(const Span&) = default;

  Span &operator=(Vector<T> &other) noexcept {
    ref(other);
    return *this;
  }

  using Base::at;
  using Base::begin;
  using Base::capacity;
  using Base::data;
  using Base::empty;
  using Base::end;
  using Base::operator==;
  using Base::operator[];
  using Base::clear;
  using Base::size;
  using Base::truncate;
  using Base::view;

  T *data() noexcept {
    return const_cast<T *>(data_);
  }

  // unchecked element access
  T &operator[](size_t index) noexcept {
    return data()[index];
  }

  // checked element access:
  // throws if index is out of bounds
  T &at(size_t index) {
    ONESTL_BOUNDS_TINY(index, <, size_);
    return data()[index];
  }

  T *begin() noexcept {
    return data();
  }

  T *end() noexcept {
    return data() + size_;
  }

  // throws if start or end are out of bounds
  Span<T> span(size_t start, size_t end) {
    ONESTL_BOUNDS_TINY(start, <=, end);
    ONESTL_BOUNDS_TINY(end, <=, size_);
    return Span<T>(data() + start, end - start);
  }

  // throws if this and src have different sizes.
  void copy(View<T> src) {
    ONESTL_BOUNDS_TINY(src.size(), ==, size());
    std::memcpy(data(), src.data(), size() * sizeof(T));
  }

  void swap(Span &other) noexcept {
    Span temp = *this;
    *this = other;
    other = temp;
  }

  void ref(T *addr, size_t n) noexcept {
    data_ = addr;
    size_ = n;
  }
  void ref(Span<T> &other) noexcept {
    data_ = other.data_;
    size_ = other.size_;
  }
  void ref(Vector<T> &other) noexcept {
    Base::ref(other);
  }
};

template <class T> void swap(Span<T> &left, Span<T> &right) noexcept {
  left.swap(right);
}

// ------------- View<T> methods requiring complete type Span<T> ---------------

template <class T>
constexpr View<T>::View(const Span<T> &other) noexcept //
    : data_{other.data()}, size_{other.size()} {
}

template <class T> void View<T>::ref(const Span<T> &other) noexcept {
  data_ = other.data();
  size_ = other.size();
}

} // namespace onestl

#endif // ONESTL_SPAN_HPP
