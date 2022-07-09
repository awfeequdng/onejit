/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * writer_string.cpp
 *
 *  Created on Jan 26, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/io/writer.hpp>
#include <onestl/io/writer_string.hpp>
#include <onestl/string.hpp>

#include <cerrno> // ENOMEM

namespace onestl {
namespace io {

static int write_to_string(void *handle, const char *chars, size_t n) noexcept {
  String *dst = reinterpret_cast<String *>(handle);
  int err = 0;
  if (!dst) {
    err = EINVAL;
  } else if (!dst->append(View<char>{chars, n})) {
    err = ENOMEM;
  }
  return err;
}

template <> Writer Writer::make<String *>(String *dst) noexcept {
  return Writer{dst, write_to_string};
}

} // namespace io
} // namespace onestl
