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
 *  Created on Jan 28, 2020
 *      Author Massimiliano Ghilardi
 */

#include "onejit/kind.hpp"

#include <vector>

namespace onejit {

static const std::vector<std::string> kstring{
    "void",      "bool",    "int",     "int8",      "int16",      "int32",
    "int64",     "uint",    "uint8",   "uint16",    "uint32",     "uint64",
    "uintptr",   "float32", "float64", "complex64", "complex128",
    "", // "array",
    "", // "chan",
    "", // "func",
    "", // "interface",
    "", // "map",
    "ptr",
    "", // "slice",
    "", // "string",
    "", // "struct",
    "", // "unsafe.Pointer",
    "archflags",
};

static Size ksize[] = {
    Size(0), Size(1),                                      //
    Size(8), Size(1), Size(2), Size(4),  Size(8),          // int*
    Size(8), Size(1), Size(2), Size(4),  Size(8), Size(8), // uint*
    Size(4), Size(8), Size(8), Size(16),                   // float* complex*
    Size(0), Size(0), Size(0), Size(0),  Size(0),          //
    Size(8),                                               // ptr
    Size(0), Size(0), Size(0), Size(0),                    //
    Size(8),                                               // archflags
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

} // namespace onejit
