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
 * crange.hpp
 *
 *  Created on Feb 23, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONESTL_CRANGE_HPP
#define ONESTL_CRANGE_HPP

#include <onestl/array.hpp>

namespace onestl {

////////////////////////////////////////////////////////////////////////////////
/**
 * read-only range of elements inside an underlying View<T>, Span<T> or Array<T>
 * it remains valid even after the View or Span is reassigned with ref(), swap() or operator=
 * or the Array is resized or changes capacity.
 *
 * CRange start and size can be changed at any time with set_bounds(),
 * even to values out of bounds from the underlying View, Span or Array.
 *
 * CRange operator bool, operator[], and view() will check if start and size
 * are out of bounds with respect to the underlying View, Span or Array size *in that moment*
 * (the size of underlying View or Span can change by reassigning it as described above,
 * and the size of underlying Array can change by resizing it).
 */
template <class T> class CRange {
public:
  // create an invalid CRange. to construct a valid CRange,
  // use one of the other constructors
  constexpr CRange() noexcept : impl_{}, start_{}, size_{} {
  }

  constexpr explicit CRange(const View<T> *view) noexcept
      : impl_{view}, start_{0}, size_{view ? view->size() : 0} {
  }

  // defined in range.hpp
  constexpr explicit CRange(const Range<T> &range) noexcept;

  constexpr CRange(const View<T> *view, size_t a_start, size_t a_size) noexcept
      : impl_{view}, start_{a_start}, size_{a_size} {
  }
  // defined in range.hpp
  constexpr CRange(const Range<T> &range, size_t a_start, size_t a_size) noexcept;

  constexpr size_t start() const noexcept {
    return start_;
  }

  constexpr size_t size() const noexcept {
    return size_;
  }

  constexpr bool empty() const noexcept {
    return size_ == 0;
  }

  // change start() and size().
  void set_bounds(size_t start, size_t size) noexcept {
    start_ = start;
    size_ = size;
  }

  constexpr explicit operator bool() const noexcept {
    return impl_ && size_ && size_ <= impl_->size() && start_ <= impl_->size() - size_;
  }

  // checked element access:
  // return i-th element by value, or T{} if index is out of bounds
  constexpr T operator[](size_t index) const noexcept(noexcept(T{})) {
    return impl_ && index < size_ ? (*impl_)[index + start_] : T{};
  }

  // convert this CRange to a View. Note: the returned object WILL BE INVALIDATED
  // if the underlying View/Span is reassigned with ref(), swap() or operator=
  // or the underlying Array is resized or changes capacity
  constexpr View<T> view() const noexcept {
    return bool(*this) ? View<T>{impl_->data() + start_, size_} : View<T>{};
  }

protected:
  const View<T> *impl_;
  size_t start_, size_;
};

} // namespace onestl

#endif // ONESTL_CRANGE_HPP
