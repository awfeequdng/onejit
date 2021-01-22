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
 * chars.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_STRING_HPP
#define ONESTL_STRING_HPP

#include <onestl/chars.hpp>
#include <onestl/vector.hpp>

#include <ostream>

namespace onestl {

/** resizeable vector of char */
class String : public Vector<char> {
private:
  typedef char T;
  typedef Vector<char> Base;

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
  explicit String(const Vector<T> &other) noexcept : Base{other} {
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

  using Base::dup;
  bool dup(const Chars &other) noexcept {
    return Base::dup(other);
  }

}; // class String

inline std::ostream &operator<<(std::ostream &out, const String &str) {
  return out.write(str.data(), str.size());
}

// -------------- Chars methods requiring complete type String -----------------

constexpr Chars::Chars(const String &other) noexcept //
    : Base{other} {
}

} // namespace onestl

#endif /* ONESTL_STRING_HPP */
