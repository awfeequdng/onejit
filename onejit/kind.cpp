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
 *  Created on Jan 08, 2020
 *      Author Massimiliano Ghilardi
 */

#include "onejit/kind.hpp"
#include "onejit/chars.hpp"

namespace onejit {

static const Chars kstring[] = {
    "void",    "bool",                                 //
    "int8",    "int16",     "int32",     "int64",      //
    "uint8",   "uint16",    "uint32",    "uint64",     //
    "float32", "float64",   "complex64", "complex128", //
    "ptr",     "archflags", "?",                       //
};

static const Bits kbits[] = {
    Bits(0),  Bits(1),                       // Void, Bool
    Bits(8),  Bits(16), Bits(32), Bits(64),  // Int*
    Bits(8),  Bits(16), Bits(32), Bits(64),  // Uint*
    Bits(32), Bits(64), Bits(64), Bits(128), // Float* Complex*
    Bits(64),                                // Ptr
    Bits(64),                                // ArchFlags
};

static const Group kgroup[] = {
    gVoid,  gBool,                      // Void, Bool
    gInt,   gInt,   gInt,     gInt,     // Int*
    gUint,  gUint,  gUint,    gUint,    // Uint*
    gFloat, gFloat, gComplex, gComplex, // Float* Complex*
    gPtr,                               // Ptr
    gArch,                              // ArchFlags
};

const Chars &Kind::string() const {
  enum _ { n = sizeof(kstring) / sizeof(kstring[0]) };
  uint8_t i = val_;
  if (i >= n) {
    i = n - 1;
  }
  return kstring[i];
}

Bits Kind::bits() const {
  uint8_t i = val_;
  if (i >= sizeof(kbits) / sizeof(kbits[0])) {
    i = 0;
  }
  return kbits[i];
}

Group Kind::group() const {
  uint8_t i = val_;
  if (i >= sizeof(kgroup) / sizeof(kgroup[0])) {
    i = 0;
  }
  return kgroup[i];
}

std::ostream &operator<<(std::ostream &out, Kind kind) {
  return out << kind.string();
}

} // namespace onejit
