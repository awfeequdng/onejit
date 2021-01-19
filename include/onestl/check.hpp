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
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_CHECK_HPP
#define ONESTL_CHECK_HPP

#include <onestl/fwd.hpp>

#include <ostream>
#include <sstream>
#include <utility>

#define ONESTL_BOUNDS(lhs, op, rhs)                                                                \
  (((lhs) /**/ op /**/ (rhs)) ? (void)0 : ONESTL_THROW_ERROR(lhs, op, rhs, throw_bounds_failed))

#define ONESTL_CHECK(lhs, op, rhs)                                                                 \
  (((lhs) /**/ op /**/ (rhs)) ? (void)0 : ONESTL_THROW_ERROR(lhs, op, rhs, throw_check_failed))

#define ONESTL_TEST(lhs, op, rhs)                                                                  \
  (((lhs) /**/ op /**/ (rhs)) ? (void)0 : ONESTL_THROW_ERROR_FULL(lhs, op, rhs, throw_check_failed))

#ifdef __OPTIMIZE__
#define ONESTL_THROW_ERROR(lhs, op, rhs, throw_func)                                               \
  ONESTL_THROW_ERROR_SHORT(lhs, op, rhs, throw_func)
#else
#define ONESTL_THROW_ERROR(lhs, op, rhs, throw_func)                                               \
  ONESTL_THROW_ERROR_FULL(lhs, op, rhs, throw_func)
#endif

#define ONESTL_THROW_ERROR_SHORT(lhs, op, rhs, throw_func) (::onestl::throw_func())
#define ONESTL_THROW_ERROR_FULL(lhs, op, rhs, throw_func)                                          \
  (::onestl::Error(::onestl::to_string(lhs), ::onestl::to_string(rhs), #op, #lhs, #rhs, __FILE__,  \
                   __LINE__)                                                                       \
       .throw_func())

namespace onestl {

class Error {
public:
  Error(std::string &&lhs, std::string &&rhs,                  //
        const char *opstr, const char *lstr, const char *rstr, //
        const char *file, int line);

  void ONESTL_NORETURN throw_check_failed() const;
  void ONESTL_NORETURN throw_bounds_failed() const;

private:
  std::string lhs_;
  std::string rhs_;
  const char *op_;
  const char *lstr_;
  const char *rstr_;
  const char *file_;
  int line_;
};

void ONESTL_NORETURN throw_check_failed();
void ONESTL_NORETURN throw_bounds_failed();

std::string to_string(std::nullptr_t);
std::string to_string(bool val);
std::string to_string(const void *val);

template <class T> std::string to_string(const T *val) {
  return to_string(static_cast<const void *>(val));
}

template <class T> std::string to_string(const T &val) {
  std::stringstream buf;
  buf << val;
  return buf.str();
}

} // namespace onestl

#endif /* ONESTL_CHECK_HPP */
