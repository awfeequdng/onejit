/*
 * onestl - tiny STL C++ library
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
