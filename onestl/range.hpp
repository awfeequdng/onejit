/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * range.hpp
 *
 *  Created on Feb 18, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONESTL_RANGE_HPP
#define ONESTL_RANGE_HPP

#include <onestl/crange.hpp>

namespace onestl {

////////////////////////////////////////////////////////////////////////////////
/**
 * range of elements inside a Array<T> or Span<T>.
 * it remains valid even after the Array is resized or changes capacity,
 * or after the Span is reassigned with ref(), swap() or operator=
 *
 * Range start and size can be changed at any time with set_bounds(),
 * even to values out of bounds from the underlying Span or Array.
 *
 * Range operator bool, operator[], set(), span() and view() will check if start and size
 * are out of bounds with respect to the underlying Span or Array size *in that moment*
 * (the size of underlying Span can change by reassigning it as described above,
 * and the size of underlying Array can change by resizing it).
 */
template <class T> class Range : protected CRange<T> {
  typedef CRange<T> Base;

  template <class T2> friend class CRange;

public:
  // create an invalid Range. to construct a valid Range,
  // use one of the other constructors
  constexpr Range() noexcept : Base{} {
  }

  constexpr explicit Range(Span<T> *span) noexcept : Base{span} {
  }
  // constexpr Range(const Range<T> &range) noexcept = default;

  constexpr Range(Span<T> *span, size_t a_start, size_t a_end) noexcept //
      : Base{span, a_start, a_end} {
  }
  constexpr Range(const Range<T> &range, size_t a_start, size_t a_end) noexcept //
      : Base{range.impl_, range.start_ + a_start, range.start_ + a_end} {
  }

  using Base::set_bounds;
  using Base::size;
  using Base::start;
  using Base::operator bool;
  using Base::operator[];
  using Base::view;

  // checked element overwrite:
  // set i-th element, or do nothing if index is out of bounds
  void set(size_t index, T elem) noexcept {
    if (impl_ && index < size_) {
      impl()->set(index + start_, elem);
    }
  }

  // convert this Range to a Span. Note: the returned object WILL BE INVALIDATED
  // if the underlying Span is reassigned with ref(), swap() or operator=
  // or the underlying Array is resized or changes capacity
  Span<T> span() noexcept {
    return bool(*this) ? Span<T>{const_cast<T *>(impl_->data()) + start_, size_} : Span<T>{};
  }

protected:
  using Base::impl_;
  using Base::size_;
  using Base::start_;

  constexpr Span<T> *impl() const noexcept {
    return const_cast<Span<T> *>(static_cast<const Span<T> *>(impl_));
  }
};

// requires Range type to be complete
template <class T>
constexpr CRange<T>::CRange(const Range<T> &range) noexcept
    : impl_{range.impl_}, start_{range.start_}, size_{range.size_} {
}

// requires Range type to be complete
template <class T>
constexpr CRange<T>::CRange(const Range<T> &range, size_t a_start, size_t a_end) noexcept
    : impl_{range.impl_}, start_{range.start_ + a_start}, //
      size_{a_end > a_start ? a_end - a_start : 0} {
}

} // namespace onestl

#endif // ONESTL_RANGE_HPP
