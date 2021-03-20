/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * sspan.hpp
 *
 *  Created on Jan 11, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_SSPAN_HPP
#define ONESTL_SSPAN_HPP

#include <onestl/view.hpp>

namespace onestl {

/** non-owning, read-write span of T[]. subclass of Span<T>, exposes operator= */
template <class T> class SSpan final : public Span<T> {
private:
  typedef Span<T> Base;

public:
  constexpr SSpan() noexcept : Base{} {
  }
  constexpr SSpan(T *addr, size_t n) noexcept : Base{addr, n} {
  }

  constexpr SSpan(const SSpan<T> &other) noexcept = default;
  constexpr SSpan(const Span<T> &other) noexcept : Base{other} {
  }
  constexpr SSpan(SSpan<T> &&other) noexcept = default;
  constexpr SSpan(Span<T> &&other) noexcept : Base{std::move(other)} {
  }

  ~SSpan() noexcept = default;

  // here we can expose assignment operator. reason: SSpan is final,
  // no subclass with different ownership semantic can exist
  SSpan<T> &operator=(const SSpan<T> &other) noexcept = default;
  SSpan<T> &operator=(SSpan<T> &&other) noexcept = default;
};

} // namespace onestl

#endif // ONESTL_SSPAN_HPP
