/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * reader_chars.hpp
 *
 *  Created on Jul 09, 2022
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_IO_READER_CHARS_HPP
#define ONESTL_IO_READER_CHARS_HPP

#include <onestl/chars.hpp>
#include <onestl/io/reader.hpp>

namespace onestl {
namespace io {

// wrap a CHARS inside a Reader
template <> Reader Reader::make<Chars>(const Chars *src, uint64_t offset) noexcept;

} // namespace io
} // namespace onestl

#endif // ONESTL_READER_CHARS_HPP
