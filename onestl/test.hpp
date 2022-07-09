/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_TEST_HPP
#define ONESTL_TEST_HPP

#include <onestl/fmt.hpp>
#include <onestl/fwd.hpp>
#include <onestl/string.hpp>
#include <onestl/test_tiny.hpp>

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

  void ONESTL_NORETURN throw_bounds_failed() const;
  void ONESTL_NORETURN throw_check_failed() const;

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

inline String to_string(const String &val) {
  return String{val};
}

inline String to_string(String &&val) noexcept {
  return std::move(val);
}

} // namespace onestl

#endif // ONESTL_TEST_HPP
