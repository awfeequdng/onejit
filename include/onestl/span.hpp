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
template <class T> class Span : public View<T> {
private:
  typedef View<T> Base;

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

  Span(const Span<T> &) = default;
  Span(Span<T> &&) = default;
  ~Span() = default;

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

  // checked element access:
  // set i-th element, or do nothing if index is out of bounds
  void set(size_t index, T elem) noexcept(noexcept(elem = std::move(elem))) {
    if (index < size_) {
      data()[index] = std::move(elem);
    }
  }

  // checked element access:
  // increment i-th element by delta, or do nothing if index is out of bounds
  void inc(size_t index, T delta) noexcept(noexcept(delta += delta)) {
    if (index < size_) {
      data()[index] += delta;
    }
  }

  // checked element access:
  // decrement i-th element by delta, or do nothing if index is out of bounds
  void dec(size_t index, T delta) noexcept(noexcept(delta -= delta)) {
    if (index < size_) {
      data()[index] -= delta;
    }
  }

  T *begin() noexcept {
    return data();
  }

  T *end() noexcept {
    return data() + size_;
  }

  // return a sub-span of this Span. returns Span{} if start or end are out of bounds.
  /*constexpr*/ Span<T> span(size_t start, size_t end) noexcept {
    return Span<T>{Base::view(start, end)};
  }

  // copy elements from src to this.
  // number of copied elements is the minimum between size() and src.size()
  void copy(View<T> src) noexcept {
    if (data() != src.data()) {
      const size_t n = min2(size(), src.size());
      std::memmove(data(), src.data(), n * sizeof(T));
    }
  }

protected:
  // cannot expose assignment operator. reason: if dynamic type is Array<T>
  // assigning from another Span may replace an owned data_ with a non-owned one
  Span<T> &operator=(const Span<T> &) noexcept = default;
  Span<T> &operator=(Span<T> &&) noexcept = default;

  // used *only* by span(start, end)
  constexpr explicit Span(const View<T> &other) noexcept : Base{other} {
  }

  using Base::data_;
  using Base::min2;
  using Base::size_;
};

} // namespace onestl

#endif // ONESTL_SPAN_HPP
