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
 * fmt.hpp
 *
 *  Created on Jan 25, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_FMT_HPP
#define ONESTL_FMT_HPP

#include <onestl/chars.hpp>

#include <cstddef>
#include <cstdio>

namespace onestl {

using fmt_write_func = int (*)(void *handle, const char *chars, size_t n);

inline fmt_write_func to_fmt_write_func(std::nullptr_t) noexcept {
  return nullptr;
}
fmt_write_func to_fmt_write_func(String *dst) noexcept;
fmt_write_func to_fmt_write_func(FILE *dst) noexcept;

////////////////////////////////////////////////////////////////////////////////
class Hex {
public:
  constexpr explicit Hex(uint64_t val) noexcept : val_{val} {
  }

  constexpr uint64_t val() const noexcept {
    return val_;
  }

private:
  uint64_t val_;
};

////////////////////////////////////////////////////////////////////////////////
class Fmt {
  typedef char T;

public:
  constexpr Fmt() noexcept : handle_{}, write_{}, err_{} {
  }

  constexpr Fmt(void *handle, fmt_write_func func) noexcept
      : handle_{handle}, write_{func}, err_{} {
  }

  template <class T>
  explicit Fmt(T *obj) noexcept(noexcept(to_fmt_write_func(obj))) //
      : handle_{obj}, write_{to_fmt_write_func(obj)}, err_{} {
  }

  constexpr void *handle() const noexcept {
    return handle_;
  }

  constexpr int err() const noexcept {
    return err_;
  }

  constexpr explicit operator bool() const noexcept {
    return err_ == 0;
  }

  const Fmt &write(const char *chars, size_t n) const noexcept;

  const Fmt &append(char ch) const noexcept;
  const Fmt &append(int64_t val) const noexcept;
  const Fmt &append(uint64_t val) const noexcept;
  const Fmt &append(float val) const noexcept {
    return append(double(val));
  }
  const Fmt &append(double val) const noexcept;
  const Fmt &append(View<T> chars) const noexcept {
    return write(chars.data(), chars.size());
  }

private:
  void *handle_;
  fmt_write_func write_;
  mutable int err_;
};

const Fmt &operator<<(const Fmt &fmt, bool arg) noexcept;

inline const Fmt &operator<<(const Fmt &fmt, char arg) noexcept {
  return fmt.append(arg);
}
inline const Fmt &operator<<(const Fmt &fmt, int8_t arg) noexcept {
  return fmt.append(int64_t(arg));
}
inline const Fmt &operator<<(const Fmt &fmt, int16_t arg) noexcept {
  return fmt.append(int64_t(arg));
}
inline const Fmt &operator<<(const Fmt &fmt, int32_t arg) noexcept {
  return fmt.append(int64_t(arg));
}
inline const Fmt &operator<<(const Fmt &fmt, int64_t arg) noexcept {
  return fmt.append(arg);
}
inline const Fmt &operator<<(const Fmt &fmt, uint8_t arg) noexcept {
  return fmt.append(uint64_t(arg));
}
inline const Fmt &operator<<(const Fmt &fmt, uint16_t arg) noexcept {
  return fmt.append(uint64_t(arg));
}
inline const Fmt &operator<<(const Fmt &fmt, uint32_t arg) noexcept {
  return fmt.append(uint64_t(arg));
}
inline const Fmt &operator<<(const Fmt &fmt, uint64_t arg) noexcept {
  return fmt.append(arg);
}
inline const Fmt &operator<<(const Fmt &fmt, float arg) noexcept {
  return fmt.append(arg);
}
inline const Fmt &operator<<(const Fmt &fmt, double arg) noexcept {
  return fmt.append(arg);
}
inline const Fmt &operator<<(const Fmt &fmt, View<char> arg) noexcept {
  return fmt.append(arg);
}
const Fmt &operator<<(const Fmt &fmt, const void *arg) noexcept;
const Fmt &operator<<(const Fmt &fmt, std::nullptr_t) noexcept;
const Fmt &operator<<(const Fmt &fmt, Hex arg) noexcept;

template <size_t N> const Fmt &operator<<(const Fmt &fmt, const char (&addr)[N]) noexcept {
  return fmt.write(addr, N - 1);
}

} // namespace onestl

#endif // ONESTL_FMT_HPP
