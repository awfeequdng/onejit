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
 * view.hpp - adapted from twin/include/stl/view.h
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_VIEW_HPP
#define ONEJIT_VIEW_HPP

#include <cassert>
#include <cstddef> // size_t
#include <cstring> // memcmp()

namespace onejit {

/** read-only view of T[] */
template <class T> class View {

protected:
  const T *data_;
  size_t size_;

public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef const T value_type;
  typedef const T &reference;
  typedef const T *pointer;
  typedef const T *iterator;
  typedef const T &const_reference;
  typedef const T *const_pointer;
  typedef const T *const_iterator;

  constexpr View() : data_(NULL), size_(0) {
  }
  constexpr View(const T *addr, size_t n) : data_(addr), size_(n) {
  }

#if 0
  View(const Span<T> &other) {
    ref(other);
  }
  View(const Vector<T> &other) {
    ref(other);
  }

  // View(const View&) = default;
  // ~View() = default;
  // operator=(const View&) = default;

  View &operator=(const Span<T> &other) {
    ref(other);
    return *this;
  }

  View &operator=(const Vector<T> &other) {
    ref(other);
    return *this;
  }
#endif // 0

  constexpr size_t capacity() const {
    return size_;
  }

  constexpr size_t size() const {
    return size_;
  }

  constexpr const T *data() const {
    return data_;
  }

  const T &operator[](size_t index) const {
    assert(index < size_);
    return data_[index];
  }

  constexpr explicit operator bool() const {
    return size_ != 0;
  }

  constexpr bool empty() const {
    return size_ == 0;
  }

  template <class VEC> bool operator==(const VEC &other) const {
    static_assert(
        sizeof(value_type) == sizeof(typename VEC::value_type),
        "onejit::View<T>::operator==(VEC) mismatched sizes of value_type and VEC::value_type");

    return size_ == other.size() && !std::memcmp(data(), other.data(), size_ * sizeof(value_type));
  }

  void ref(const T *addr, size_t n) {
    data_ = addr;
    size_ = n;
  }
  void ref(const View<T> &other) {
    data_ = other.data_;
    size_ = other.size_;
  }
#if 0
  void ref(const Span<T> &other);
  void ref(const Vector<T> &other);
#endif // 0

  constexpr const T *begin() const {
    return data_;
  }

  const T *end() const {
    assert(data_ || !size_);
    return data_ + size_;
  }

  View view(size_t start, size_t end) const {
    assert(start <= end);
    assert(end <= size_);
    return View(data_ + start, end - start);
  }

  void swap(View &other) {
    View temp = *this;
    *this = other;
    other = temp;
  }
};

template <class T> void swap(View<T> &left, View<T> &right) {
  left.swap(right);
}

} // namespace onejit

#endif /* ONEJIT_VIEW_HPP */
