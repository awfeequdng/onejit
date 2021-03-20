/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * chars.cpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/chars.hpp>

#include <cstring>

namespace onestl {

// construct Chars from '\0' terminated C string
Chars::Chars(const T *c_str) noexcept //
    : Base{c_str, c_str ? std::strlen(c_str) : 0} {
}

} // namespace onestl
