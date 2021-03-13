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

#include <onestl/algorithm.hpp>
#include <onestl/fwd.hpp>
#include <onestl/test_tiny.hpp>

#include <cstddef>     // size_t
#include <cstring>     // memcmp()
#include <type_traits> // std::is_nothrow_*<>

namespace onestl {

/** non-owning, read-only view of T[] */
template <class T> class View {

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

  constexpr View(const View<T> &) noexcept = default;
  constexpr View(View<T> &&) noexcept = default;
  ~View() = default;

  constexpr size_t capacity() const noexcept {
    return size_;
  }

  constexpr size_t size() const noexcept {
    return size_;
  }

  constexpr bool empty() const noexcept {
    return size_ == 0;
  }

  // checked element access:
  // returns i-th element by value, or T{} if index is out of bounds
  constexpr T operator[](size_t index) const
      noexcept(noexcept(T{}) && std::is_nothrow_copy_constructible<T>::value) {
    return index < size_ ? data_[index] : T{};
  }

  constexpr explicit operator bool() const noexcept {
    return size_ != 0;
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

  // return sub-view of this view. returns View{} if start or end are out of bounds.
  constexpr View<T> view(size_t start, size_t end) const noexcept {
    return start <= end && end <= size_ ? View<T>{data_ + start, end - start} : View<T>{};
  }

  void clear() noexcept {
    size_ = 0;
  }

  void truncate(size_t n) noexcept {
    if (size_ > n) {
      size_ = n;
    }
  }

  template <class VEC> //
  bool operator==(const VEC &other) const noexcept {
    static_assert(
        sizeof(value_type) == sizeof(typename VEC::value_type),
        "onestl::View<T>::operator==(VEC) mismatched sizes of value_type and VEC::value_type");

    const void *vdata = static_cast<const void *>(data_);
    const void *votherdata = static_cast<const void *>(other.data());

    return size_ == other.size()   //
           && (vdata == votherdata //
               || !std::memcmp(vdata, votherdata, size_ * sizeof(value_type)));
  }

protected:
  // cannot expose assignment operator. reason: if dynamic type is Array<T>
  // assigning from another View may replace an owned data_ with a non-owned one
  View<T> &operator=(const View<T> &) noexcept = default;
  View<T> &operator=(View<T> &&) noexcept = default;

  const T *data_;
  size_t size_;
};

typedef View<uint8_t> Bytes;

} // namespace onestl

#endif // ONESTL_VIEW_HPP
