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
 * noderange.hpp
 *
 *  Created on Feb 18, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONESTL_RANGE_HPP
#define ONESTL_RANGE_HPP

#include <onestl/vector.hpp>

namespace onestl {

////////////////////////////////////////////////////////////////////////////////
// range of elements inside a Vector<T>. it remains valid
// even after the Vector is resized or changes capacity
template <class T> class Range {
public:
  constexpr Range() noexcept : vec_{}, start_{}, size_{} {
  }

  constexpr Range(Vector<T> *vec, size_t start, size_t size) noexcept
      : vec_{vec}, start_{start}, size_{size} {
  }

  size_t constexpr size() const noexcept {
    return size_;
  }

  constexpr explicit operator bool() const noexcept {
    return vec_ && size_ && size_ <= vec_->size() && start_ <= vec_->size() - size_;
  }

  // checked element read:
  // return i-th element by value, or T{} if index is out of bounds
  constexpr T operator[](size_t index) const noexcept(noexcept(T{})) {
    return vec_ && index < size_ ? (*vec_)[index + start_] : T{};
  }

  // checked element overwrite:
  // set i-th element, or do nothing if index is out of bounds
  void set(size_t index, T elem) const noexcept {
    if (vec_ && index < size_) {
      vec_->set(index + start_, elem);
    }
  }

  Span<T> span() noexcept {
    return bool(*this) ? Span<T>{vec_->data() + start_, size_} : Span<T>{};
  }

  constexpr View<T> view() const noexcept {
    return bool(*this) ? View<T>{vec_->data() + start_, size_} : View<T>{};
  }

private:
  Vector<T> *vec_;
  size_t start_, size_;
};

} // namespace onestl

#endif // ONESTL_RANGE_HPP
