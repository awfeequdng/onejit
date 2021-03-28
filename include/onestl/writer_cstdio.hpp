/*
 * onestl - tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * writer_cstdio.hpp
 *
 *  Created on Jan 26, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONESTL_WRITER_CSTDIO_HPP
#define ONESTL_WRITER_CSTDIO_HPP

#include <onestl/writer.hpp>

#include <cstdio>

namespace onestl {

template <> Writer Writer::make<FILE *>(FILE *file) noexcept;

} // namespace onestl

#endif // ONESTL_WRITER_CSTDIO_HPP
