/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * reader.cpp
 *
 *  Created on Jul 09, 2022
 *      Author Massimiliano Ghilardi
 */

#include <onestl/io/reader.hpp>

namespace onestl {
namespace io {

////////////////////////////////////////////////////////////////////////////////
int Reader::read(char *chars, size_t n) const {
  int err = 0;
  if (func_) {
    err = func_(handle_, offset_, chars, n);
    if (err > 0) {
      offset_ += (uint64_t)err;
    }
  }
  return err;
}

} // namespace io
} // namespace onestl
