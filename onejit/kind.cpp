/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * kind.cpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/fmt.hpp>
#include <onejit/kind.hpp>

namespace onejit {

static const Chars kstring[] = {
    "bad",     "void",      "bool",              //
    "int8",    "int16",     "int32",   "int64",  //
    "uint8",   "uint16",    "uint32",  "uint64", //
    "float16", "float32",   "float64",           //
    "ptr",     "archflags", "?",                 //
};

static const char kstringsuffix[] = //
    "\1?\0\1v\0\1e\0"
    "\1b\0\1s\0\1i\0\1l\0"
    "\2ub\2us\2ui\2ul"
    "\2hf\1f\0\2lf"
    "\1p\0\003cmp";

static const Bits kbits[] = {
    Bits0,  Bits0,  Bits1,          // Bad, Void, Bool
    Bits8,  Bits16, Bits32, Bits64, // Int*
    Bits8,  Bits16, Bits32, Bits64, // Uint*
    Bits16, Bits32, Bits64,         // Float*
    Bits64, Bits64,                 // Ptr, ArchFlags
};

static const Group kgroup[] = {
    gVoid,  gVoid,  gBool,         // Bad, Void, Bool
    gInt,   gInt,   gInt,   gInt,  // Int*
    gUint,  gUint,  gUint,  gUint, // Uint*
    gFloat, gFloat, gFloat,        // Float*
    gPtr,   gArch,                 // Ptr, ArchFlags
};

const Chars Kind::string() const noexcept {
  enum _ { n = ONEJIT_N_OF(kstring) };
  uint8_t i = val_ & 0xF;
  if (i >= n) {
    i = n - 1; // "?"
  }
  return kstring[i];
}

const Chars Kind::stringsuffix() const noexcept {
  enum _ { n = ONEJIT_N_OF(kstringsuffix) };
  uint8_t i = val_ & 0xF;
  if (i >= n) {
    i = 0;
  }
  const char *str = &kstringsuffix[i * 3];
  return Chars{str + 1, uint8_t(str[0])};
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

const Fmt &operator<<(const Fmt &fmt, Kind kind) {
  fmt << kind.string();
  const size_t n = kind.simdn().val();
  if (n != 1) {
    fmt << 'x' << n;
  }
  return fmt;
}

} // namespace onejit
