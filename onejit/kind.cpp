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

#include <vector>

namespace onejit {

static const std::vector<std::string> kstring{
    "void",      "bool",                                             //
    "",          "int8",    "int16",     "int32",      "int64",      //
    "",          "uint8",   "uint16",    "uint32",     "uint64", "", //
    "float32",   "float64", "complex64", "complex128",               //
    "",          "",        "",          "",           "",           //
    "ptr",                                                           //
    "",          "",        "",          "",                         //
    "archflags",
};

static const Size ksize[] = {
    Size(0), Size(1),                                     // Void, Bool
    Size(),  Size(1), Size(2), Size(4),  Size(8),         // Int*
    Size(),  Size(1), Size(2), Size(4),  Size(8), Size(), // Uint*
    Size(4), Size(8), Size(8), Size(16),                  // Float* Complex*
    Size(),  Size(),  Size(),  Size(),   Size(),          //
    Size(8),                                              // Ptr
    Size(),  Size(),  Size(),  Size(),                    //
    Size(8),                                              // ArchFlags
};

static const Group kgroup[] = {
    gVoid,  gBool,                                    // Void, Bool
    gVoid,  gInt,   gInt,     gInt,     gInt,         // Int*
    gVoid,  gUint,  gUint,    gUint,    gUint, gVoid, // Uint*
    gFloat, gFloat, gComplex, gComplex,               // Float* Complex*
    gVoid,  gVoid,  gVoid,    gVoid,    gVoid,        //
    gPtr,                                             // Ptr
    gVoid,  gVoid,  gVoid,    gVoid,                  //
    gArch,                                            // ArchFlags
};

const std::string &Kind::string() const {
  uint8_t i = val_;
  if (i >= kstring.size()) {
    i = 0;
  }
  return kstring[i];
}

Size Kind::size() const {
  uint8_t i = val_;
  if (i >= sizeof(ksize) / sizeof(ksize[0])) {
    i = 0;
  }
  return ksize[i];
}

Group Kind::group() const {
  uint8_t i = val_;
  if (i >= sizeof(kgroup) / sizeof(kgroup[0])) {
    i = 0;
  }
  return kgroup[i];
}

} // namespace onejit
