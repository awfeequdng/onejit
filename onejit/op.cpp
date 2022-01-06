/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

static const char op1string[] = "\1?\0\0\0\1^\0\0\0\1!\0\0\0\1-\0\0\0\4cast\7bitcopy";

const Chars to_string(Op1 op) noexcept {
  size_t i = 0; // "?"
  if (op <= BITCOPY) {
    i = op;
  }
  const char *addr = op1string + i * 5;
  return Chars{addr + 1, uint8_t(addr[0])};
}

const Fmt &operator<<(const Fmt &fmt, Op1 op) {
  return fmt << to_string(op);
}

// ============================  Op2  ==========================================

// change < to >, <= to >=, > to < and >= to <=
Op2 swap_comparison(Op2 op) noexcept {
  switch (op) {
  case LSS:
    return GTR;
  case LEQ:
    return GEQ;
  case GTR:
    return LSS;
  case GEQ:
    return LEQ;
  default:
    return op;
  }
}

Op2 not_comparison(Op2 op) noexcept {
  switch (op) {
  case LSS:
    return GEQ;
  case LEQ:
    return GTR;
  case NEQ:
    return EQL;
  case EQL:
    return NEQ;
  case GTR:
    return LEQ;
  case GEQ:
    return LSS;
  default:
    return op;
  }
}

static const char op2string[] = "\1?\0\1-\0\1/\0\1%\0\2<<\2>>\2&&\2||\1<\0\2<=\2!=\2==\1>\0\2>=";

const Chars to_string(Op2 op) noexcept {
  size_t i = 0; // "?"
  if (op <= GEQ) {
    i = op;
  }
  const char *addr = op2string + i * 3;
  return Chars{addr + 1, uint8_t(addr[0])};
}

const Fmt &operator<<(const Fmt &fmt, Op2 op) {
  return fmt << to_string(op);
}

// ============================  OpN  ==========================================

static const Chars opnstring[] = {"max",     "min",     "comma",     "call",     "mem",
                                  "mir_mem", "x86_mem", "arm64_mem", "mir_args", "mir_rets"};

const Chars to_string(OpN op) noexcept {
  if (op >= MAX && op - MAX < ONEJIT_N_OF(opnstring)) {
    return opnstring[op - MAX];
  }
  if (op > XOR) {
    op = BADN;
  }
  return Chars{&"?+*&|^"[op], 1};
}

const Fmt &operator<<(const Fmt &fmt, OpN op) {
  return fmt << to_string(op);
}

} // namespace onejit
