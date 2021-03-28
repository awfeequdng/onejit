/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * string.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_STRING_HPP
#define ONESTL_STRING_HPP

#include <onestl/array.hpp>
#include <onestl/chars.hpp>

namespace onestl {

/** resizeable array of char */
class String : public Array<char> {
private:
  typedef char T;
  typedef Array<char> Base;

public:
  constexpr String() noexcept : Base{} {
  }
  template <size_t N> String(const T (&addr)[N]) noexcept : Base{addr, N - 1} {
  }
  String(const T *addr, size_t n) noexcept : Base{addr, n} {
  }
  // also catches Span<T> and Chars
  explicit String(const View<T> &other) noexcept : Base{other} {
  }
  explicit String(const String &other) noexcept : Base{other} {
  }
  String(String &&other) noexcept = default;

  ~String() noexcept = default;

  String &operator=(String &&other) noexcept {
    Base::operator=(std::move(other));
    return *this;
  }

  Chars view(size_t start, size_t end) const noexcept {
    return Chars{Base::view(start, end)};
  }

  // ensure data() is terminated by '\0' then return it
  const char *c_str() noexcept;

  using Base::dup;
}; // class String

} // namespace onestl

#endif // ONESTL_STRING_HPP
