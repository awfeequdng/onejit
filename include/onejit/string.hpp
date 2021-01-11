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
 * chars.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_STRING_HPP
#define ONEJIT_STRING_HPP

#include <onejit/vector.hpp>

#include <ostream>

namespace onejit {

/** resizeable vector of char */
class String : public Vector<char> {
private:
  typedef char T;
  typedef Vector<char> Base;

public:
  constexpr String() : Base() {
  }
  template <size_t N> constexpr String(const T (&addr)[N]) : Base(addr, N - 1) {
  }
  constexpr String(const T *addr, size_t n) : Base(addr, n) {
  }
  explicit String(const View<T> &other) : Base(other) {
  }
  explicit String(const Span<T> &other) : Base(other) {
  }
  explicit String(const Vector<T> &other) : Base(other) {
  }
  explicit String(const String &other) : Base(other) {
  }

  Chars view(size_t start, size_t end) const {
    return Chars(Base::view(start, end));
  }

  using Base::ref;
  void ref(const Chars &other) {
    Base::ref(other);
  }

}; // namespace onejit

inline std::ostream &operator<<(std::ostream &out, const String &string) {
  return out.write(chars.data(), chars.size());
}

} // namespace onejit

#endif /* ONEJIT_STRING_HPP */
