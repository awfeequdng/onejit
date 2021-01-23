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
 * kind.cpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/kind.hpp>
#include <onestl/chars.hpp>

namespace onejit {

static const Chars kstring[] = {
    "bad",     "void",      "bool",              //
    "int8",    "int16",     "int32",   "int64",  //
    "uint8",   "uint16",    "uint32",  "uint64", //
    "float16", "float32",   "float64",           //
    "ptr",     "archflags", "?",                 //
};

static const Chars kstringsuffix[] = {
    "?",  "v",   "e",        //
    "b",  "s",   "i",  "l",  //
    "ub", "us",  "ui", "ul", //
    "hf", "f",   "lf",       //
    "p",  "cmp",             //
};

static const Bits kbits[] = {
    Bits(0),  Bits(0),  Bits(1),            // Bad, Void, Bool
    Bits(8),  Bits(16), Bits(32), Bits(64), // Int*
    Bits(8),  Bits(16), Bits(32), Bits(64), // Uint*
    Bits(16), Bits(32), Bits(64),           // Float*
    Bits(64), Bits(64),                     // Ptr, ArchFlags
};

static const Group kgroup[] = {
    gVoid,  gVoid,  gBool,         // Bad, Void, Bool
    gInt,   gInt,   gInt,   gInt,  // Int*
    gUint,  gUint,  gUint,  gUint, // Uint*
    gFloat, gFloat, gFloat,        // Float*
    gPtr,   gArch,                 // Ptr, ArchFlags
};

const Chars &Kind::string() const noexcept {
  enum _ { n = ONEJIT_N_OF(kstring) };
  uint8_t i = val_ & 0xF;
  if (i >= n) {
    i = n - 1; // "?"
  }
  return kstring[i];
}

const Chars &Kind::stringsuffix() const noexcept {
  enum _ { n = ONEJIT_N_OF(kstringsuffix) };
  uint8_t i = val_ & 0xF;
  if (i >= n) {
    i = 0;
  }
  return kstringsuffix[i];
}

Bits Kind::bits() const noexcept {
  uint8_t i = val_ & 0xF;
  if (i >= ONEJIT_N_OF(kbits)) {
    i = 0;
  }
  return kbits[i] * simdn();
}

Group Kind::group() const noexcept {
  uint8_t i = val_ & 0xF;
  if (i >= ONEJIT_N_OF(kgroup)) {
    i = 0;
  }
  return kgroup[i];
}

std::ostream &operator<<(std::ostream &out, Kind kind) {
  out << kind.string();
  const size_t n = kind.simdn().val();
  if (n != 1) {
    out << 'x' << n;
  }
  return out;
}

} // namespace onejit
