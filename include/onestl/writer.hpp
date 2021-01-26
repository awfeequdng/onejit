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
 * writer.hpp
 *
 *  Created on Jan 25, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_WRITER_HPP
#define ONESTL_WRITER_HPP

#include <onestl/chars.hpp>

namespace onestl {

////////////////////////////////////////////////////////////////////////////////
class Writer final {
  typedef char T;

public:
  using func_type = int (*)(void *handle, const char *chars, size_t n);

  // construct null Writer, that will ignore all chars passed to write()
  constexpr Writer() noexcept : handle_{}, func_{} {
  }

  // calls Writer::make(obj)
  template <class T> explicit Writer(T obj) noexcept : Writer{make(obj)} {
  }

  constexpr Writer(void *handle, func_type write_func) noexcept //
      : handle_{handle}, func_{write_func} {
  }

  constexpr void *handle() const noexcept {
    return handle_;
  }

  constexpr func_type func() const noexcept {
    return func_;
  }

  // write n chars to handle, return I/O error
  int write(const char *chars, size_t n) const noexcept {
    return func_ ? func_(handle_, chars, n) : 0;
  }

private:
  // users can define explicit specializations,
  // they will be picked up by Writer{obj} and Fmt{obj}
  template <class T> static Writer make(T obj) noexcept;

  void *handle_;
  func_type func_;
}; // namespace onestl

} // namespace onestl

#endif // ONESTL_WRITER_HPP
