/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

  constexpr CRange(const View<T> *view, size_t a_start, size_t a_end) noexcept
      : impl_{view}, start_{a_start}, size_{a_end > a_start ? a_end - a_start : 0} {
  }
  // defined in range.hpp
  constexpr CRange(const Range<T> &range, size_t a_start, size_t a_end) noexcept;

  // redundant
  // ~CRange() noexcept = default;

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
  void set_bounds(size_t a_start, size_t a_end) noexcept {
    start_ = a_start;
    size_ = a_end > a_start ? a_end - a_start : 0;
  }

  constexpr explicit operator bool() const noexcept {
    return impl_ && size_ && size_ <= impl_->size() && start_ <= impl_->size() - size_;
  }

  // checked element access:
  /// @return i-th element by value, or T{} if index is out of bounds
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
