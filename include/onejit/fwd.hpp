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
 * fwd.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FWD_HPP
#define ONEJIT_FWD_HPP

#include <cstdint> // uint*_t

namespace onejit {

class ArchId;
class Bits;
class Break;
class Code;
class Continue;
class Chars;
class Fallthrough;
class Kind;
class Node;
typedef uint32_t NodeHeader;
typedef uint32_t Offset;
template <class T> class Span;
class Stmt0;
class Stmt1;
class String;
template <class T> class Vector;
template <class T> class View;

typedef View<uint8_t> Bytes;
typedef Span<char> CharSpan;
typedef View<NodeHeader> CodeView;

} // namespace onejit

#endif // ONEJIT_FWD_HPP
