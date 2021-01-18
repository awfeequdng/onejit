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
 * varexpr.cpp
 *
 *  Created on Jan 13, 2020
 *      Author Massimiliano Ghilardi
 */

#include "onejit/const.hpp"
#include "onejit/code.hpp"
#include "onestl/chars.hpp"

namespace onejit {

Const Const::parse_indirect(Kind kind, Offset offset, const Code *holder) {
  uint64_t bits;
  if (kind.bits().val() <= 32) {
    bits = holder->uint32(offset);
  } else {
    bits = holder->uint64(offset);
  }
  return Const{kind, bits};
}

Code &Const::write_indirect(Code *holder) const {
  if (kind().bits().val() <= 32) {
    holder->add(uint32());
  } else {
    holder->add(uint64());
  }
  return *holder;
}

std::ostream &operator<<(std::ostream &out, const Const &c) {
  /// TODO: pretty-print constants with SimdN > 1
  switch (c.kind().nosimd().val()) {
  case kVoid:
    out << Chars("void");
    break;
  case kBool:
    out << (c.boolean() ? Chars("true") : Chars("false"));
    break;
  case kInt8:
  case kInt16:
  case kInt32:
  case kInt64:
    out << c.int64();
    break;
  case kUint8:
  case kUint16:
  case kUint32:
  case kUint64:
  case kArchFlags:
    out << c.uint64();
    break;
  case kFloat32:
    out << c.float32();
    break;
  case kFloat64:
    out << c.float64();
    break;
  case kPtr:
    out << c.ptr();
    break;
  default:
    out << '?';
    break;
  }
  return out;
}

} // namespace onejit
