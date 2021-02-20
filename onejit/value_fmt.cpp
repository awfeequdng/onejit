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
 * value_fmt.cpp
 *
 *  Created on Feb 07, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/fmt.hpp>
#include <onejit/value.hpp>
#include <onestl/chars.hpp>

namespace onejit {

const Fmt &operator<<(const Fmt &out, const Value &value) {
  const Kind kind = value.kind();
  switch (kind.nosimd().val()) {
  case eVoid:
    out << "void";
    break;
  case eBool:
    out << (value.boolean() ? Chars("true") : Chars("false"));
    break;
  case eInt8:
  case eInt16:
  case eInt32:
  case eInt64:
    out << value.int64();
    break;
  case eUint8:
  case eUint16:
  case eUint32:
  case eUint64:
  case eArchFlags:
    out << value.uint64();
    break;
  case eFloat32:
    out << value.float32();
    break;
  case eFloat64:
    out << value.float64();
    break;
  case ePtr:
    out << "0x" << Hex{value.ptr()};
    break;
  default:
    out << '?';
    break;
  }
  const size_t n = kind.simdn().val();
  if (n > 1) {
    out << '_' << kind.stringsuffix() << 'x' << n;
  }
  return out;
}

} // namespace onejit
