/*
 * onestl - Tiny STL C++ library
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
 * check.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_CHECK_HPP
#define ONESTL_CHECK_HPP

#include <onestl/check_tiny.hpp>
#include <onestl/fmt.hpp>
#include <onestl/fwd.hpp>
#include <onestl/string.hpp>

#include <utility> // std::move()

#define ONESTL_BOUNDS(lhs, op, rhs)                                                                \
  (((lhs) /**/ op /**/ (rhs)) ? (void)0 : ONESTL_THROW_ERROR(lhs, op, rhs, throw_bounds_failed))

#define ONESTL_CHECK(lhs, op, rhs)                                                                 \
  (((lhs) /**/ op /**/ (rhs)) ? (void)0 : ONESTL_THROW_ERROR(lhs, op, rhs, throw_check_failed))

#define ONESTL_TEST(lhs, op, rhs)                                                                  \
  (((lhs) /**/ op /**/ (rhs)) ? (void)0 : ONESTL_THROW_ERROR_FULL(lhs, op, rhs, throw_check_failed))

#ifdef __OPTIMIZE__
#define ONESTL_THROW_ERROR(lhs, op, rhs, throw_func)                                               \
  ONESTL_THROW_ERROR_TINY(lhs, op, rhs, throw_func)
#else
#define ONESTL_THROW_ERROR(lhs, op, rhs, throw_func)                                               \
  ONESTL_THROW_ERROR_FULL(lhs, op, rhs, throw_func)
#endif

#define ONESTL_THROW_ERROR_FULL(lhs, op, rhs, throw_func)                                          \
  (::onestl::Failed(::onestl::to_string(lhs), ::onestl::to_string(rhs), #op, #lhs, #rhs, __FILE__, \
                    __LINE__)                                                                      \
       .throw_func())

namespace onestl {

class Failed {
public:
  Failed(String &&lhs, String &&rhs,                            //
         const char *opstr, const char *lstr, const char *rstr, //
         const char *file, int line) noexcept;

  void ONESTL_NORETURN throw_check_failed() const;
  void ONESTL_NORETURN throw_bounds_failed() const;

private:
  String lhs_;
  String rhs_;
  const char *op_;
  const char *lstr_;
  const char *rstr_;
  const char *file_;
  int line_;
};

template <class T> String to_string(const T &val) {
  String str;
  Fmt{&str} << val;
  return str;
}

} // namespace onestl

#endif // ONESTL_CHECK_HPP
