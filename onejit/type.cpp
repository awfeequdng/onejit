/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * type.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/fmt.hpp>
#include <onejit/type.hpp>
#include <onestl/chars.hpp>

namespace onejit {

static const int8_t tchildren[] = {
    0, 1, 2,  3, 4,  -1,    // Stmt*
    0, 1, 1,  2, -1, 0,  0, // Expr
    0, 0, -1,               // FuncType, Name
};

static const Chars tstring[] = {
    "stmt0", "stmt1", "stmt2", "stmt3",  "stmt4", "stmtn",          // Stmt*
    "var",   "mem",   "unary", "binary", "tuple", "label", "const", // *Expr
    "ftype", "name",  "?",                                          // FuncType, Name
};

uint32_t to_children(Type t) noexcept {
  const uint8_t n = ONEJIT_N_OF(tchildren);
  uint8_t i = uint8_t(t);
  if (i >= n) {
    i = n - 1;
  }
  return (uint32_t)tchildren[i];
}

const Chars to_string(Type t) noexcept {
  const uint8_t n = ONEJIT_N_OF(tstring);
  uint8_t i = uint8_t(t);
  if (i >= n) {
    i = n - 1;
  }
  return tstring[i];
}

const Fmt &operator<<(const Fmt &fmt, Type t) {
  return fmt << to_string(t);
}

} // namespace onejit
