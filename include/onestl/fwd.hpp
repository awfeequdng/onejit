/*
 * onestl - Tiny STL C++ library
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
 * fwd.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONESTL_FWD_HPP
#define ONESTL_FWD_HPP

#include <cstddef> // size_t
#include <cstdint> // uint*_t

#ifdef __GNUC__
#define ONESTL_NOINLINE __attribute__((noinline))
#define ONESTL_NORETURN __attribute__((noreturn))
#else
#define ONESTL_NOINLINE
#define ONESTL_NORETURN
#endif

namespace onestl {

class Chars;
class Fmt;
class Hex;
class String;
class ArrayHelper;
class Writer;

template <class T> class Array;
template <class T> class Buffer;
template <class T> class CRange;
template <class T> class Range;
template <class T> class Span;
template <class T> class View;

typedef View<uint8_t> Bytes;

} // namespace onestl

#endif // ONESTL_FWD_HPP
