/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * chars.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_CHARS_HPP
#define ONESTL_CHARS_HPP

#include <onestl/array.hpp>

namespace onestl {

/** non-owning, read-only view of char[] */
class Chars final : public View<char> {
private:
  typedef char T;
  typedef View<T> Base;

public:
  constexpr Chars() noexcept : Base{} {
  }

  // construct Chars from literal string constant
  template <size_t N> constexpr Chars(const T (&addr)[N]) noexcept : Base{addr, N - 1} {
  }

  constexpr Chars(const T *addr, size_t n) noexcept : Base{addr, n} {
  }
  constexpr Chars(View<T> other) noexcept : Base{other} {
  }

  constexpr Chars(const Chars &) noexcept = default;
  constexpr Chars(Chars &&) noexcept = default;

  // construct Chars from '\0' terminated C string
  static Chars of(const T *c_str) noexcept;

  // Chars is final: no subclass can change the ownership semantic of data_,
  // so exposing assignment operator is safe.
  //
  // Unlike View<T>, which has the subclass Array<T> which changes the semantic
  // of data_ from non-owned to owned.
  Chars &operator=(const Chars &) noexcept = default;
  Chars &operator=(Chars &&) noexcept = default;

  Chars view(size_t start, size_t end) const noexcept {
    return Chars{Base::view(start, end)};
  }
};

} // namespace onestl

#endif // ONESTL_CHARS_HPP
