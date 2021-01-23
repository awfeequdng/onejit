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
 * const.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/const.hpp>
#include <onestl/chars.hpp>

namespace onejit {

Const Const::parse_indirect(Kind kind, Offset offset, const Code *holder) noexcept {
  uint64_t bits;
  if (kind.bits().val() == 64) {
    bits = holder->uint64(offset);
  } else {
    bits = holder->uint32(offset);
  }
  return Const{kind, bits};
}

Code &Const::write_indirect(Code *holder) const noexcept {
  if (kind().bits().val() == 64) {
    return holder->add_uint64(uint64());
  } else {
    return holder->add_uint32(uint32());
  }
}

std::ostream &operator<<(std::ostream &out, const Const &c) {
  const Kind kind = c.kind();
  switch (kind.nosimd().val()) {
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
  out << '_' << kind.stringsuffix();
  const size_t n = kind.simdn().val();
  if (n != 1) {
    out << 'x' << n;
  }
  return out;
}

} // namespace onejit
