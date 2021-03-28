/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test_tiny.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_TEST_TINY_HPP
#define ONESTL_TEST_TINY_HPP

#include <onestl/fwd.hpp>

#define ONESTL_BOUNDS_TINY(lhs, op, rhs)                                                           \
  (((lhs) /**/ op /**/ (rhs)) ? (void)0 : ONESTL_THROW_BOUNDS_FAILED())

#define ONESTL_CHECK_TINY(lhs, op, rhs)                                                            \
  (((lhs) /**/ op /**/ (rhs)) ? (void)0 : ONESTL_THROW_CHECK_FAILED())

#define ONESTL_THROW_BOUNDS_FAILED() (::onestl::throw_bounds_failed())
#define ONESTL_THROW_CHECK_FAILED() (::onestl::throw_check_failed())
#define ONESTL_THROW_ERROR_TINY(lhs, op, rhs, throw_func) (::onestl::throw_func())

namespace onestl {

void ONESTL_NORETURN throw_bounds_failed();
void ONESTL_NORETURN throw_check_failed();

} // namespace onestl

#endif // ONESTL_TEST_TINY_HPP
