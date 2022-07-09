/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

class BitSet;
class Chars;
class Fmt;
class Graph;
class Hex;
class String;
class ArrayHelper;
class Writer;

template <class T> class Array;
template <class T> class Buffer;
template <class T> class CRange;
template <class T> class Range;
template <class T> class SSpan;
template <class T> class Span;
template <class T> class View;

typedef View<uint8_t> Bytes;

namespace graph {
// the following three typedefs are guaranteed to be the same type
typedef uint32_t Degree;
typedef uint32_t Node;
typedef uint32_t Size;

enum : Size {
  // be careful: graph::NoPos is uint32_t(-1),
  // while BitSet::NoPos is size_t(-1)
  NoPos = Size(-1),
};

} // namespace graph

} // namespace onestl

#endif // ONESTL_FWD_HPP
