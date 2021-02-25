/*
 * onestl - tiny STL C++ library
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
 * string.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_STRING_HPP
#define ONESTL_STRING_HPP

#include <onestl/chars.hpp>
#include <onestl/array.hpp>

namespace onestl {

/** resizeable vector of char */
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
  explicit String(const View<T> &other) noexcept : Base{other} {
  }
  explicit String(const Span<T> &other) noexcept : Base{other} {
  }
  explicit String(const Array<T> &other) noexcept : Base{other} {
  }
  explicit String(const String &other) noexcept : Base{other} {
  }
  ~String() noexcept = default;

  String(String &&other) noexcept = default;

  String &operator=(String &&other) noexcept {
    Base::operator=(std::move(other));
    return *this;
  }

  Chars view(size_t start, size_t end) const noexcept {
    return Chars(Base::view(start, end));
  }

  // ensure data() is terminated by '\0' then return it
  const char *c_str() noexcept;

  using Base::dup;
  bool dup(const Chars &other) noexcept {
    return Base::dup(other);
  }
}; // class String

// -------------- Chars methods requiring complete type String -----------------

constexpr Chars::Chars(const String &other) noexcept //
    : Base{other} {
}

} // namespace onestl

#endif // ONESTL_STRING_HPP
