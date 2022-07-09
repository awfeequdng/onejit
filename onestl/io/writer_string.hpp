/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * writer_string.hpp
 *
 *  Created on Jan 26, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_IO_WRITER_STRING_HPP
#define ONESTL_IO_WRITER_STRING_HPP

#include <onestl/io/writer.hpp>
#include <onestl/string.hpp>

namespace onestl {
namespace io {

// wrap a String inside a Writer
template <> Writer Writer::make<String *>(String *dst) noexcept;

} // namespace io
} // namespace onestl

#endif // ONESTL_WRITER_STRING_HPP
