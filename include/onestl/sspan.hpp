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
