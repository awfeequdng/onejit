/*
 * onejit - JIT compiler in C++
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
 *  Created on Jan 11, 2020
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_SPAN_HPP
#define ONEJIT_SPAN_HPP

#include <onejit/view.hpp>

namespace onejit {

/** read-write span of T[] */
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

  constexpr Span() : Base() {
  }
  constexpr Span(T *addr, size_t n) : Base(addr, n) {
  }
  Span(Vector<T> &other) : Base(other) {
  }

  // Span(const Span&) = default;
  // ~Span() = default;
  // operator=(const Span&) = default;

  Span &operator=(Vector<T> &other) {
    ref(other);
    return *this;
  }

  using Base::begin;
  using Base::capacity;
  using Base::data;
  using Base::empty;
  using Base::end;
  using Base::operator==;
  using Base::operator[];
  using Base::operator bool;
  using Base::size;
  using Base::view;

  constexpr T *data() {
    return const_cast<T *>(data_);
  }

  T &operator[](size_t index) {
    check(index, <, size_);
    return data()[index];
  }

  T *begin() {
    return data();
  }

  T *end() {
    check(data_, ||, size_ == 0);
    return data() + size_;
  }

  Span<T> span(size_t start, size_t end) {
    check(start, <=, end);
    check(end, <=, size_);
    return Span<T>(data() + start, end - start);
  }

  void copy(View<T> src) {
    check(src.size(), ==, size());
    std::memcpy(data(), src.data(), size() * sizeof(T));
  }

  void swap(Span &other) {
    Span temp = *this;
    *this = other;
    other = temp;
  }

  void ref(T *addr, size_t n) {
    data_ = addr;
    size_ = n;
  }
  void ref(Span<T> &other) {
    data_ = other.data_;
    size_ = other.size_;
  }
  void ref(Vector<T> &other);
};

template <class T> void View<T>::ref(const Span<T> &other) {
  data_ = other.data();
  size_ = other.size();
}

template <class T> void swap(Span<T> &left, Span<T> &right) {
  left.swap(right);
}

typedef Span<char> CharSpan;

} // namespace onejit

#endif /* ONEJIT_SPAN_HPP */
