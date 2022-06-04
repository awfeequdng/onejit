/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

  constexpr Span(const Span<T> &) noexcept = default;
  constexpr Span(Span<T> &&) noexcept = default;
  ~Span() noexcept = default;

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

  /// @return a sub-span of this Span. returns Span{} if start or end are out of bounds.
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

  // set all elements to 'value'
  void fill(T value) noexcept(noexcept(value = value)) {
    T *addr = data();
    for (size_t i = 0, n = size_; i < n; i++) {
      addr[i] = value;
    }
  }

protected:
  // cannot expose assignment operator. reason: if dynamic type is Array<T>
  // assigning from another Span may replace an owned data_ with a non-owned one
  Span<T> &operator=(const Span<T> &) noexcept = default;
  Span<T> &operator=(Span<T> &&) noexcept = default;

  // used *only* by method span(start, end)
  constexpr explicit Span(View<T> other) noexcept : Base{other} {
  }

  using Base::data_;
  using Base::size_;
};

} // namespace onestl

#endif // ONESTL_SPAN_HPP
