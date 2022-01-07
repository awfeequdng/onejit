/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * fmt.cpp
 *
 *  Created on Jan 25, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/fmt.hpp>

#include <cstdio>  // std::snprintf()
#include <cstring> // std::strlen()

namespace onestl {

ONESTL_NOINLINE const Fmt &Fmt::write(const char *chars, size_t n) const {
  if (n != 0 && err_ == 0) {
    err_ = writer_.write(chars, n);
  }
  return *this;
}

const Fmt &operator<<(const Fmt &fmt, bool val) {
  return fmt << (val ? Chars("true") : Chars("false"));
}

const Fmt &operator<<(const Fmt &fmt, char ch) {
  return fmt.write(&ch, 1);
}

const Fmt &operator<<(const Fmt &fmt, int64_t val) {
  if (val < 0) {
    fmt.write("-", 1);
    val = -val;
  }
  return fmt << uint64_t(val);
}

const Fmt &operator<<(const Fmt &fmt, uint64_t val) {
  if (!fmt || !fmt.writer().func()) {
    return fmt;
  }
  char buf[24]; // [21] would suffice too
  unsigned i = sizeof(buf);
  do {
    buf[--i] = '0' + (val % 10);
    val /= 10;
  } while (i && val);
  return fmt.write(buf + i, sizeof(buf) - i);
}

const Fmt &operator<<(const Fmt &fmt, double val) {
  if (!fmt || !fmt.writer().func()) {
    return fmt;
  }
  enum _ : int { cap = 24 };
  char buf[cap];
  int n = std::snprintf(buf, cap, "%g", val);
  if (n > cap) {
    n = cap;
  }
  return fmt.write(buf, n);
}

const Fmt &operator<<(const Fmt &fmt, std::nullptr_t) {
  return fmt.write("null", 4);
}

// c_str must be '\0' terminated
const Fmt &operator<<(const Fmt &fmt, const char *c_str) {
  return fmt.write(c_str, c_str ? std::strlen(c_str) : 0);
}

const Fmt &operator<<(const Fmt &fmt, Hex arg) {
  if (!fmt || !fmt.writer().func()) {
    return fmt;
  }
  char buf[16];
  uint64_t val = arg.val();
  unsigned i = sizeof(buf);
  do {
    buf[--i] = "0123456789abcdef"[val & 0x0F];
    val >>= 4;
  } while (i && val);
  return fmt.write(buf + i, sizeof(buf) - i);
}

} // namespace onestl
