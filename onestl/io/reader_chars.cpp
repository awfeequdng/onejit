/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * reader_chars.cpp
 *
 *  Created on Jul 09, 2022
 *      Author Massimiliano Ghilardi
 */

#include <onestl/algorithm.hpp>
#include <onestl/chars.hpp>
#include <onestl/io/reader.hpp>
#include <onestl/io/reader_chars.hpp>

#include <climits>
#include <cstring>

namespace onestl {
namespace io {

static int read_from_chars(const void *handle, uint64_t offset, char *chars, size_t n) noexcept {
  const Chars *src = reinterpret_cast<const Chars *>(handle);
  size_t read_n = 0;
  if (src && offset < src->size()) {
    read_n = min3(INT_MAX, n, src->size() - (size_t)offset);
    if (read_n != 0) {
      std::memcpy(chars, src->data() + (size_t)offset, read_n);
    }
  }
  return (int)read_n;
}

template <> Reader Reader::make<Chars>(const Chars *src, uint64_t offset) noexcept {
  return Reader{src, offset, read_from_chars};
}

} // namespace io
} // namespace onestl
