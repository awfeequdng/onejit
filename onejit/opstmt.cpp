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
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/opstmt.hpp>
#include <onestl/chars.hpp>

#include <ostream>

namespace onejit {

// ============================  OpStmt0  ======================================

static const Chars op_stmt_0_string[] = {"bad", "break", "continue", "fallthrough", "?"};

const Chars &to_string(OpStmt0 op) noexcept {
  uint8_t i = op;
  if (i >= ONEJIT_N_OF(op_stmt_0_string)) {
    i = ONEJIT_N_OF(op_stmt_0_string) - 1;
  }
  return op_stmt_0_string[i];
}

std::ostream &operator<<(std::ostream &out, OpStmt0 op) {
  return out << to_string(op);
}

// ============================  OpStmt1  ======================================

static const Chars op_stmt_1_string[] = {"?", "goto"};

const Chars &to_string(OpStmt1 op) noexcept {
  uint8_t i = op;
  if (i >= ONEJIT_N_OF(op_stmt_1_string)) {
    i = 0;
  }
  return op_stmt_1_string[i];
}

std::ostream &operator<<(std::ostream &out, OpStmt1 op) {
  return out << to_string(op);
}

// ============================  OpStmt2  ======================================

static const Chars op_stmt_2_string[] = {"?", "case", "default", "jump_if"};

const Chars &to_string(OpStmt2 op) noexcept {
  uint8_t i = op;
  if (i >= ONEJIT_N_OF(op_stmt_2_string)) {
    i = 0;
  }
  return op_stmt_2_string[i];
}

std::ostream &operator<<(std::ostream &out, OpStmt2 op) {
  return out << to_string(op);
}

// ============================  OpStmt3  ======================================

static const Chars op_stmt_3_string[] = {"?", "if"};

const Chars &to_string(OpStmt3 op) noexcept {
  uint8_t i = op;
  if (i >= ONEJIT_N_OF(op_stmt_3_string)) {
    i = 0;
  }
  return op_stmt_3_string[i];
}

std::ostream &operator<<(std::ostream &out, OpStmt3 op) {
  return out << to_string(op);
}

// ============================  OpStmt4  ======================================

static const Chars op_stmt_4_string[] = {"?", "for"};

const Chars &to_string(OpStmt4 op) noexcept {
  uint8_t i = op;
  if (i >= ONEJIT_N_OF(op_stmt_4_string)) {
    i = 0;
  }
  return op_stmt_4_string[i];
}

std::ostream &operator<<(std::ostream &out, OpStmt4 op) {
  return out << to_string(op);
}

// ============================  OpStmtN  ======================================

static const Chars op_stmt_n_string[] = {"?", "=tuple", "block", "cond", "return", "switch"};

const Chars &to_string(OpStmtN op) noexcept {
  uint8_t i = op;
  if (i >= ONEJIT_N_OF(op_stmt_n_string)) {
    i = 0;
  }
  return op_stmt_n_string[i];
}

std::ostream &operator<<(std::ostream &out, OpStmtN op) {
  return out << to_string(op);
}

} // namespace onejit
