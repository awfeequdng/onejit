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
 * test_tiny.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_TEST_TINY_HPP
#define ONESTL_TEST_TINY_HPP

#include <onestl/fwd.hpp>

#define ONESTL_BOUNDS_TINY(lhs, op, rhs)                                                           \
  (((lhs) /**/ op /**/ (rhs)) ? (void)0                                                            \
                              : ONESTL_THROW_ERROR_TINY(lhs, op, rhs, throw_bounds_failed))

#define ONESTL_CHECK_TINY(lhs, op, rhs)                                                            \
  (((lhs) /**/ op /**/ (rhs)) ? (void)0 : ONESTL_THROW_ERROR_TINY(lhs, op, rhs, throw_check_failed))

#define ONESTL_THROW_ERROR_TINY(lhs, op, rhs, throw_func) (::onestl::throw_func())

namespace onestl {

void ONESTL_NORETURN throw_check_failed();
void ONESTL_NORETURN throw_bounds_failed();

} // namespace onestl

#endif // ONESTL_TEST_TINY_HPP
