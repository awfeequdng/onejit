/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * writer.hpp
 *
 *  Created on Jan 25, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_WRITER_HPP
#define ONESTL_WRITER_HPP

#include <cstddef> // size_t

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
  int write(const char *chars, size_t n) const {
    return func_ ? func_(handle_, chars, n) : 0;
  }

private:
  // users can define explicit specializations,
  // they will be picked up by Writer{obj} and Fmt{obj}
  template <class T> static Writer make(T obj) noexcept;

  void *handle_;
  func_type func_;
};

} // namespace onestl

#endif // ONESTL_WRITER_HPP
