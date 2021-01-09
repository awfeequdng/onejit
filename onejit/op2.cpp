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
 * op2.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#include "onejit/op2.hpp"

#include <ostream>

namespace onejit {

static const Chars op2string[] = {
    "+",  "-",  "*",  "/",  "%",  "&",  "|",  "^",  "<<",  ">>",  "&^",  //
    "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "&^=", //
    "&&", "||", "?",  "?",  "?",                                         //
    "==", "<",  ">",  "=",  "?",                                         //
    "!=", "<=", ">=",                                                    //
};

static const Chars op2string_other[] = {"?", "goto"};

const Chars &to_string(Op2 op) {
  if (op >= ADD && op <= GEQ) {
    return op2string[op - ADD];
  }
  uint8_t i = (op == GOTO) ? 1 : 0;
  return op2string_other[i];
}

std::ostream &operator<<(std::ostream &out, Op2 op) {
  return out << to_string(op);
}

} // namespace onejit
