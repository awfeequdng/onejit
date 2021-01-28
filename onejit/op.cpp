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
 * op.cpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/fmt.hpp>
#include <onejit/op.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// ============================  Op1  ==========================================

static const Chars op1string[] = {"?", "^", "!", "-", "convert", "bitcopy"};

const Chars &to_string(Op1 op) noexcept {
  size_t i = op;
  if (i >= ONEJIT_N_OF(op1string)) {
    i = 0;
  }
  return op1string[i];
}

const Fmt &operator<<(const Fmt &out, Op1 op) {
  return out << to_string(op);
}

// ============================  Op2  ==========================================

static const char op2string[] = //
  "\1? \1+ \1- \1* \1/ \1% \1& \1| \1^ " //
  "\2<<\2>>\2&^\2&&\2||" //
  "\1< \2<=\2!=\2==\1> \2>=";


const Chars to_string(Op2 op) noexcept {
  size_t i = 0; // "?"
  if (op <= GEQ) {
    i = op * 3;
  }
  const char * addr = op2string + i;
  return Chars{addr + 1, uint8_t(addr[0])};
}

const Fmt &operator<<(const Fmt &out, Op2 op) {
  return out << to_string(op);
}

// ============================  OpN  ==========================================

static const Chars opnstring[] = {"?", "call"};

const Chars to_string(OpN op) noexcept {
  size_t i = op;
  if (i >= ONEJIT_N_OF(opnstring)) {
    i = 0;
  }
  return opnstring[i];
}

const Fmt &operator<<(const Fmt &out, OpN op) {
  return out << to_string(op);
}

} // namespace onejit
