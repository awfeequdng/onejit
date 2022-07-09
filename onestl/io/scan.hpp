/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * scan.hpp
 *
 *  Created on Jul 09, 2022
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_IO_SCAN_HPP
#define ONESTL_IO_SCAN_HPP

#include <onestl/chars.hpp>
#include <onestl/io/reader.hpp>

namespace onestl {
namespace io {

////////////////////////////////////////////////////////////////////////////////
class Scan {
  typedef char T;

public:
  using func_type = Reader::func_type;

  // construct null Scan, that will always return EOF
  constexpr Scan() noexcept : reader_{}, next_{-1}, err_{} {
  }

  constexpr Scan(const void *handle, uint64_t offset, func_type read_func) noexcept //
      : reader_{handle, offset, read_func}, next_{-1}, err_{} {
  }

  // calls Reader::make(obj, offset)
  template <class T>
  explicit Scan(T obj, uint64_t offset = 0) noexcept //
      : reader_{obj, offset}, next_{-1}, err_{} {
  }

  Reader &reader() noexcept {
    return reader_;
  }

  constexpr const Reader &reader() const noexcept {
    return reader_;
  }

  constexpr int err() const noexcept {
    return err_;
  }

  // return false if I/O error
  explicit operator bool() const noexcept {
    return err_ == 0 || eof(); // EOF is not an I/O error
  }

  // return true if EOF was reached and there are no more buffered characters to read
  bool eof() const noexcept;

  // return the next char casted to (unsigned char) then to (int)
  // without consuming it, or < 0 if error.
  // if EOF, returns -ENOENT
  int peek() const;

  // read and consume the next chars and return it casted to
  // (unsigned char) then to (int), or < 0 if error.
  // if EOF, returns -ENOENT
  int next() const;

  // skip characters 0..32 i.e. whitespace
  void skip_whitespace() const;

private:
  Reader reader_;
  mutable int next_; // < 0 if no buffered next char
  mutable int err_;
};

////////////////////////////////////////////////////////////////////////////////
const Scan &operator>>(const Scan &scan, int64_t &arg);
const Scan &operator>>(const Scan &scan, uint64_t &arg);
const Scan &operator>>(const Scan &scan, double &arg);

/**
 * operator>>(Scan,  void *) is not defined.
 */
const Scan &operator>>(const Scan &scan, void *arg) = delete;

} // namespace io
} // namespace onestl

#endif // ONESTL_IO_SCAN_HPP
