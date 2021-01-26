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
 * opstmt.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPSTMT_HPP
#define ONEJIT_OPSTMT_HPP

#include <onejit/fmt.hpp>
#include <onejit/fwd.hpp>

#include <cstdint> // uint8_t

namespace onejit {

enum OpStmt0 : uint16_t {
  BAD = 0,
  BREAK = 1,
  CONTINUE = 2,
  FALLTHROUGH = 3,
};

enum OpStmt1 : uint16_t {
  BAD_ST1 = 0,
  GOTO = 1,
  INC = 2, // ++
  DEC = 3, // --
};

enum OpStmt2 : uint16_t {
  BAD_ST2 = 0,
  // intentionally same values as Op2
  ADD_ASSIGN,     // +=
  SUB_ASSIGN,     // -=
  MUL_ASSIGN,     // *=
  QUO_ASSIGN,     // /=
  REM_ASSIGN,     // %=
  AND_ASSIGN,     // &=
  OR_ASSIGN,      // |=
  XOR_ASSIGN,     // ^=
  SHL_ASSIGN,     // <<=
  SHR_ASSIGN,     // >>=
  AND_NOT_ASSIGN, // &^=
  ASSIGN,         // =

  CASE,
  DEFAULT,
  JUMP_IF,
};

enum OpStmt3 : uint16_t {
  BAD_ST3 = 0,
  IF = 1,
};

enum OpStmt4 : uint16_t {
  BAD_ST4 = 0,
  FOR = 1,
};

enum OpStmtN : uint16_t {
  BAD_STN = 0,
  ASSIGN_CALL = 1,
  BLOCK = 2,
  COND = 3, // n-ary IF ... [ELSEIF* ... [ELSE ...]]
  RETURN = 4,
  SWITCH = 5,
};

const Chars &to_string(OpStmt0 op) noexcept;
const Chars &to_string(OpStmt1 op) noexcept;
const Chars &to_string(OpStmt2 op) noexcept;
const Chars &to_string(OpStmt3 op) noexcept;
const Chars &to_string(OpStmt4 op) noexcept;
const Chars &to_string(OpStmtN op) noexcept;

const Fmt &operator<<(const Fmt &out, OpStmt0 op);
const Fmt &operator<<(const Fmt &out, OpStmt1 op);
const Fmt &operator<<(const Fmt &out, OpStmt2 op);
const Fmt &operator<<(const Fmt &out, OpStmt3 op);
const Fmt &operator<<(const Fmt &out, OpStmt4 op);
const Fmt &operator<<(const Fmt &out, OpStmtN op);

} // namespace onejit

#endif // ONEJIT_OPSTMT_HPP
