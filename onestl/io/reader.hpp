/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * reader.hpp
 *
 *  Created on Jul 09, 2022
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_IO_READER_HPP
#define ONESTL_IO_READER_HPP

#include <cstddef> // size_t
#include <cstdint> // uint64_t

namespace onestl {
namespace io {

////////////////////////////////////////////////////////////////////////////////
class Reader final {
  typedef char T;

public:
  using func_type = int (*)(const void *handle, uint64_t offset, char *chars, size_t n);

  // construct null Reader, that will always return 0 bytes i.e. EOF
  constexpr Reader() noexcept : handle_{}, offset_{}, func_{} {
  }

  // calls Reader::make(obj, offset)
  template <class T>
  explicit Reader(const T *obj, uint64_t offset = 0) noexcept //
      : Reader{make(obj, offset)} {
  }

  constexpr Reader(const void *handle, uint64_t offset, func_type read_func) noexcept //
      : handle_{handle}, offset_{offset}, func_{read_func} {
  }

  constexpr const void *handle() const noexcept {
    return handle_;
  }

  constexpr uint64_t offset() const noexcept {
    return offset_;
  }

  constexpr func_type func() const noexcept {
    return func_;
  }

  // read at most n chars from handle and return number of chars actually read,
  // or 0 if EOF, or < 0 if I/O error
  int read(char *chars, size_t n) const;

private:
  // users can define explicit specializations,
  // they will be picked up by Reader{obj, offset} and Scan{obj, offset}
  template <class T> static Reader make(const T *obj, uint64_t offset) noexcept;

  const void *handle_;
  mutable uint64_t offset_;
  func_type func_;
};

} // namespace io
} // namespace onestl

#endif // ONESTL_IO_READER_HPP
