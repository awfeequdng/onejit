/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * endian.hpp
 *
 *  Created on Jan 21, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ENDIAN_HPP

/**
 * this can detect endianness only on some compilers, including gcc/g++ and clang/clang++
 * not a problem, as it's only used to help debugging, showing more readable values
 * in debuggers as gdb when printing onejit::Header and onejit::Local values
 * on little endian machines.
 *
 * thus there is no need to check for <endian.h>, include it if exists, and use its macros.
 */
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) &&                                 \
    __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define ONEJIT_LITTLE_ENDIAN
#else
#undef ONEJIT_LITTLE_ENDIAN
#endif

#define ONEJIT_ENDIAN_HPP

#endif // ONEJIT_ENDIAN_HPP
