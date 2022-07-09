/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * scan.cpp
 *
 *  Created on Jul 09, 2022
 *      Author Massimiliano Ghilardi
 */

#include <onestl/io/scan.hpp>

#include <cerrno> // ENOENT

namespace onestl {
namespace io {

ONESTL_NOINLINE int Scan::peek() const {
  if (next_ >= 0) {
    return next_;
  } else if (err_ < 0) {
    return err_;
  }
  int err;
  char ch = 0;
  err = reader_.read(&ch, 1);
  if (err < 0) {
    err_ = err;
  } else if (err == 0) {
    err_ = err = -ENOENT; // EOF
  } else {
    next_ = err = (unsigned char)ch; // success, return char read
  }
  return err;
}

ONESTL_NOINLINE int Scan::next() const {
  if (next_ >= 0) {
    int err = next_;
    next_ = -1;
    return err;
  } else if (err_ < 0) {
    return err_;
  }
  int err;
  char ch = 0;
  err = reader_.read(&ch, 1);
  if (err < 0) {
    err_ = err;
  } else if (err == 0) {
    err_ = err = -ENOENT; // EOF
  } else {
    err = (unsigned char)ch; // success, return char read
  }
  return err;
}

ONESTL_NOINLINE void Scan::skip_whitespace() const {
  int err;
  while ((err = peek()) >= 0 && err <= ' ') {
    next(); // skip one whitespace character
  }
}

bool Scan::eof() const noexcept {
  return next_ < 0 && err_ == -ENOENT;
}

const Scan &operator>>(const Scan &scan, int64_t &val) {
  uint64_t uval = 0;
  bool negative = false;
  scan.skip_whitespace();
  if (scan.peek() == '-') {
    negative = true;
    (void)scan.next();
  }
  scan >> uval;
  val = int64_t(negative ? -uval : uval);
  return scan;
}

const Scan &operator>>(const Scan &scan, uint64_t &val) {
  val = 0;
  scan.skip_whitespace();
  int err;
  while ((err = scan.peek()) >= '0' && err <= '9') {
    val = val * 10 + (err - '0');
    scan.next();
  }
  return scan;
}

} // namespace io
} // namespace onestl
