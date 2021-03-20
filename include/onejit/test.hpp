/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test.hpp
 *
 *  Created on Jan 19, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_TEST_HPP
#define ONEJIT_TEST_HPP

#include <onestl/test.hpp>

#define ONEJIT_BOUNDS(lhs, op, rhs) ONESTL_BOUNDS(lhs, op, rhs)
#define ONEJIT_CHECK(lhs, op, rhs) ONESTL_CHECK(lhs, op, rhs)
#define ONEJIT_TEST(lhs, op, rhs) ONESTL_TEST(lhs, op, rhs)

#define ONEJIT_THROW_BOUNDS_FAILED() ONESTL_THROW_BOUNDS_FAILED()
#define ONEJIT_THROW_CHECK_FAILED() ONESTL_THROW_CHECK_FAILED()

#endif // ONEJIT_TEST_HPP
