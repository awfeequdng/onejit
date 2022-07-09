/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * fmt.hpp
 *
 *  Created on Jan 25, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_IO_FMT_HPP
#define ONESTL_IO_FMT_HPP

#include <onestl/chars.hpp>
#include <onestl/io/writer.hpp>

namespace onestl {
namespace io {

////////////////////////////////////////////////////////////////////////////////
class Fmt {
  typedef char T;

public:
  using func_type = Writer::func_type;

  // construct null Fmt, that will ignore all chars passed to write() or operator<<(Fmt, /**/)
  constexpr Fmt() noexcept : writer_{}, err_{} {
  }

  constexpr Fmt(void *handle, func_type write_func) noexcept //
      : writer_{handle, write_func}, err_{} {
  }

  // calls Writer::make(obj)
  template <class T>
  explicit Fmt(T obj) noexcept //
      : writer_{obj}, err_{} {
  }

  Writer &writer() noexcept {
    return writer_;
  }

  constexpr const Writer &writer() const noexcept {
    return writer_;
  }

  constexpr int err() const noexcept {
    return err_;
  }

  constexpr explicit operator bool() const noexcept {
    return err_ == 0;
  }

  const Fmt &write(const char *chars, size_t n) const;

private:
  Writer writer_;
  mutable int err_;
};

////////////////////////////////////////////////////////////////////////////////
class Hex {
public:
  constexpr explicit Hex(uint64_t val) noexcept : val_{val} {
  }

  explicit Hex(const void *val) noexcept //
      : val_{reinterpret_cast<size_t>(val)} {
  }

  constexpr uint64_t val() const noexcept {
    return val_;
  }

private:
  uint64_t val_;
};

////////////////////////////////////////////////////////////////////////////////
const Fmt &operator<<(const Fmt &fmt, bool arg);
const Fmt &operator<<(const Fmt &fmt, char arg);
const Fmt &operator<<(const Fmt &fmt, int64_t arg);
const Fmt &operator<<(const Fmt &fmt, uint64_t arg);
const Fmt &operator<<(const Fmt &fmt, double arg);
const Fmt &operator<<(const Fmt &fmt, Hex arg);
const Fmt &operator<<(const Fmt &fmt, std::nullptr_t);

/**
 * operator<<(Fmt, const void *) is not defined.
 * to print an address in hexadecimal format, use: fmt << "0x" << Hex(address)
 * to print the content of a C string (i.e. const char *) use: fmt << Chars::of(c_str)
 */
const Fmt &operator<<(const Fmt &fmt, const void *arg) = delete;

inline const Fmt &operator<<(const Fmt &fmt, int8_t arg) {
  return fmt << int64_t(arg);
}
inline const Fmt &operator<<(const Fmt &fmt, int16_t arg) {
  return fmt << int64_t(arg);
}
inline const Fmt &operator<<(const Fmt &fmt, int32_t arg) {
  return fmt << int64_t(arg);
}
inline const Fmt &operator<<(const Fmt &fmt, uint8_t arg) {
  return fmt << uint64_t(arg);
}
inline const Fmt &operator<<(const Fmt &fmt, uint16_t arg) {
  return fmt << uint64_t(arg);
}
inline const Fmt &operator<<(const Fmt &fmt, uint32_t arg) {
  return fmt << uint64_t(arg);
}
inline const Fmt &operator<<(const Fmt &fmt, float arg) {
  return fmt << double(arg);
}

template <size_t N> inline const Fmt &operator<<(const Fmt &fmt, const char (&addr)[N]) {
  return fmt.write(addr, N - 1);
}

inline const io::Fmt &operator<<(const io::Fmt &fmt, View<char> arg) {
  return fmt.write(arg.data(), arg.size());
}
inline const io::Fmt &operator<<(const io::Fmt &fmt, View<int8_t> arg) {
  return fmt.write(reinterpret_cast<const char *>(arg.data()), arg.size());
}
inline const io::Fmt &operator<<(const io::Fmt &fmt, View<uint8_t> arg) {
  return fmt.write(reinterpret_cast<const char *>(arg.data()), arg.size());
}

} // namespace io
} // namespace onestl

#endif // ONESTL_IO_FMT_HPP
