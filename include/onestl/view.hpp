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
 * view.hpp - adapted from twin/include/stl/view.h
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_VIEW_HPP
#define ONESTL_VIEW_HPP

#include <onestl/check.hpp>
#include <onestl/fwd.hpp>

#include <cstddef> // size_t
#include <cstring> // memcmp()

namespace onestl {

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

  constexpr View() noexcept : data_(NULL), size_(0) {
  }
  constexpr View(const T *addr, size_t n) noexcept : data_(addr), size_(n) {
  }
  // defined in span.hpp
  constexpr View(const Span<T> &other) noexcept;
  // defined in vector.hpp
  constexpr View(const Vector<T> &other) noexcept;

  // View(const View&) = default;
  // ~View() = default;
  // operator=(const View&) = default;

  View &operator=(const Span<T> &other) noexcept {
    ref(other);
    return *this;
  }

  View &operator=(const Vector<T> &other) noexcept {
    ref(other);
    return *this;
  }

  constexpr size_t capacity() const noexcept {
    return size_;
  }

  constexpr size_t size() const noexcept {
    return size_;
  }

  constexpr bool empty() const noexcept {
    return size_ == 0;
  }

  // unchecked element access
  const T &operator[](size_t index) const noexcept {
    return data_[index];
  }

  // checked element access:
  // throws if index is out of bounds
  const T &at(size_t index) const {
    ONESTL_BOUNDS(index, <, size_);
    return data_[index];
  }

  constexpr const T *data() const noexcept {
    return data_;
  }

  constexpr const T *begin() const noexcept {
    return data_;
  }

  constexpr const T *end() const noexcept {
    return data_ + size_;
  }

  // throws if start or end are out of bounds
  View view(size_t start, size_t end) const {
    ONESTL_BOUNDS(start, <=, end);
    ONESTL_BOUNDS(end, <=, size_);
    return View(data_ + start, end - start);
  }

  void swap(View &other) noexcept {
    View temp = *this;
    *this = other;
    other = temp;
  }

  void ref(const T *addr, size_t n) noexcept {
    data_ = addr;
    size_ = n;
  }
  void ref(const View<T> &other) noexcept {
    data_ = other.data_;
    size_ = other.size_;
  }
  void ref(const Span<T> &other) noexcept;
  void ref(const Vector<T> &other) noexcept;

  template <class VEC> bool operator==(const VEC &other) const noexcept {
    static_assert(
        sizeof(value_type) == sizeof(typename VEC::value_type),
        "onestl::View<T>::operator==(VEC) mismatched sizes of value_type and VEC::value_type");

    return size_ == other.size() && !std::memcmp(data(), other.data(), size_ * sizeof(value_type));
  }
};

template <class T> void swap(View<T> &left, View<T> &right) noexcept {
  left.swap(right);
}

} // namespace onestl

#endif // ONESTL_VIEW_HPP
