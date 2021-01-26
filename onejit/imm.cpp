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
 * imm.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/imm.hpp>
#include <onestl/chars.hpp>

namespace onejit {

Imm Imm::parse_indirect(Kind kind, Offset offset, const Code *holder) noexcept {
  uint64_t bits;
  if (kind.bits().val() == 64) {
    bits = holder->uint64(offset);
  } else {
    bits = holder->uint32(offset);
  }
  return Imm{kind, bits};
}

Code &Imm::write_indirect(Code *holder) const noexcept {
  if (kind().bits().val() == 64) {
    return holder->add_uint64(uint64());
  } else {
    return holder->add_uint32(uint32());
  }
}

const Fmt &operator<<(const Fmt &out, const Imm &imm) {
  const Kind kind = imm.kind();
  switch (kind.nosimd().val()) {
  case kVoid:
    out << Chars("void");
    break;
  case kBool:
    out << (imm.boolean() ? Chars("true") : Chars("false"));
    break;
  case kInt8:
  case kInt16:
  case kInt32:
  case kInt64:
    out << imm.int64();
    break;
  case kUint8:
  case kUint16:
  case kUint32:
  case kUint64:
  case kArchFlags:
    out << imm.uint64();
    break;
  case kFloat32:
    out << imm.float32();
    break;
  case kFloat64:
    out << imm.float64();
    break;
  case kPtr:
    out << imm.ptr();
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
