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
 * opstmt.cpp
 *
 *  Created on Jan 18, 2020
 *      Author Massimiliano Ghilardi
 */

#include "onejit/opstmt.hpp"
#include "onestl/chars.hpp"

#include <ostream>

#define N_OF(array) (sizeof(array) / sizeof(array[0]))

namespace onejit {

// ============================  OpStmt0  ======================================

static const Chars opstmt0string[] = {"?", "break", "continue", "fallthrough"};

const Chars &to_string(OpStmt0 op) {
  uint8_t i = op;
  if (i >= N_OF(opstmt0string)) {
    i = 0;
  }
  return opstmt0string[i];
}

std::ostream &operator<<(std::ostream &out, OpStmt0 op) {
  return out << to_string(op);
}

// ============================  OpStmt1  ======================================

static const Chars opstmt1string[] = {"?", "default"};

const Chars &to_string(OpStmt1 op) {
  uint8_t i = op;
  if (i >= N_OF(opstmt1string)) {
    i = 0;
  }
  return opstmt1string[i];
}

std::ostream &operator<<(std::ostream &out, OpStmt1 op) {
  return out << to_string(op);
}

// ============================  OpStmt2  ======================================

static const Chars opstmt2string[] = {"?", "case"};

const Chars &to_string(OpStmt2 op) {
  uint8_t i = op;
  if (i >= N_OF(opstmt2string)) {
    i = 0;
  }
  return opstmt2string[i];
}

std::ostream &operator<<(std::ostream &out, OpStmt2 op) {
  return out << to_string(op);
}

} // namespace onejit
