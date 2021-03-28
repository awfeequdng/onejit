/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * writer_cstdio.cpp
 *
 *  Created on Jan 26, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/writer.hpp>
#include <onestl/writer_cstdio.hpp>

#include <cerrno>
#include <cstdio>

namespace onestl {

static int write_to_file(void *handle, const char *chars, size_t n) noexcept {
  FILE *dst = reinterpret_cast<FILE *>(handle);
  int err = 0;
  if (!dst) {
    err = EINVAL;
  } else if (std::fwrite(chars, 1, n, dst) != n) {
    err = errno;
  }
  return err;
}

template <> Writer Writer::make<FILE *>(FILE *dst) noexcept {
  return Writer{dst, write_to_file};
}

} // namespace onestl
