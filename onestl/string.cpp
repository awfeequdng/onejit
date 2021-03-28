/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * string.cpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/string.hpp>

namespace onestl {

template class Array<char>;

// ensure data() is terminated by '\0' then return it
const char *String::c_str() noexcept {
  if (cap_ > size_) {
    data()[size_] = '\0';
    return data_;
  } else if (cap_ == 0 || !append('\0')) {
    return "";
  }
  truncate(size_ - 1);
  return data_;
}

} // namespace onestl
