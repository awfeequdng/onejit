/*
 * onejit - JIT compiler in C++
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
 * endian.hpp
 *
 *  Created on Jan 21, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ENDIAN_HPP

/**
 * this can detect endianness only on some compilers, including gcc/g++ and clang/clang++
 * not a problem, as it's only used to help debugging, showing more readable values
 * in debuggers as gdb when printing onejit::NodeHeader and onejit::Var values
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
