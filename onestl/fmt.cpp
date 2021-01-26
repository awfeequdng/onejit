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
 * fmt.cpp
 *
 *  Created on Jan 25, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/fmt.hpp>

#include <cstdio>  // std::snprintf()
#include <cstring> // std::strlen()

namespace onestl {

ONESTL_NOINLINE const Fmt &Fmt::write(const char *chars, size_t n) const noexcept {
  if (n != 0 && err_ == 0) {
    err_ = writer_.write(chars, n);
  }
  return *this;
}

ONESTL_NOINLINE const Fmt &operator<<(const Fmt &fmt, bool val) noexcept {
  return fmt << (val ? Chars("true") : Chars("false"));
}

ONESTL_NOINLINE const Fmt &operator<<(const Fmt &fmt, char ch) noexcept {
  return fmt.write(&ch, 1);
}

ONESTL_NOINLINE const Fmt &operator<<(const Fmt &fmt, int64_t val) noexcept {
  if (val < 0) {
    fmt.write("-", 1);
    val = -val;
  }
  return fmt << uint64_t(val);
}

ONESTL_NOINLINE const Fmt &operator<<(const Fmt &fmt, uint64_t val) noexcept {
  if (!fmt || !fmt.writer().func()) {
    return fmt;
  }
  char buf[24]; // [21] would suffice too
  unsigned i = sizeof(buf);
  while (i) {
    buf[--i] = '0' + (val % 10);
    val /= 10;
    if (val == 0) {
      break;
    }
  }
  return fmt.write(buf + i, sizeof(buf) - i);
}

ONESTL_NOINLINE const Fmt &operator<<(const Fmt &fmt, double val) noexcept {
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

const Fmt &operator<<(const Fmt &fmt, const void *val) noexcept {
  return fmt << "0x" << Hex{size_t(val)};
}

const Fmt &operator<<(const Fmt &fmt, std::nullptr_t) noexcept {
  return fmt.write("null", 4);
}

// c_str must be '\0' terminated
const Fmt &operator<<(const Fmt &fmt, const char *c_str) noexcept {
  return fmt.write(c_str, c_str ? std::strlen(c_str) : 0);
}

const Fmt &operator<<(const Fmt &fmt, Hex arg) noexcept {
  if (!fmt || !fmt.writer().func()) {
    return fmt;
  }
  char buf[16];
  uint64_t val = arg.val();
  unsigned i = sizeof(buf);
  while (i) {
    buf[--i] = "0123456789abcdef"[val & 0x0F];
    val >>= 4;
    if (val == 0) {
      break;
    }
  }
  return fmt.write(buf + i, sizeof(buf) - i);
}

} // namespace onestl
