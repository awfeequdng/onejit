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

#include <onejit/fmt.hpp>
#include <onejit/op.hpp>
#include <onejit/opstmt.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// ============================  OpStmt0  ======================================

static const Chars op_stmt_0_string[] = {
    //
    "_", "break", "continue", "fallthrough",

#define ONEJIT_X(NAME, name) "x86_" #name,
    ONEJIT_OPSTMT0_X86(ONEJIT_X)
#undef ONEJIT_X

        "?" //
};

const Chars to_string(OpStmt0 op) noexcept {
  size_t i = op;
  if (i >= ONEJIT_N_OF(op_stmt_0_string)) {
    i = ONEJIT_N_OF(op_stmt_0_string) - 1;
  }
  return op_stmt_0_string[i];
}

const Fmt &operator<<(const Fmt &out, OpStmt0 op) {
  return out << to_string(op);
}

// ============================  OpStmt1  ======================================

OpStmt1 negate_condjump(OpStmt1 op) noexcept {
  switch (op) {
  case ASM_JA:
    op = ASM_JBE;
    break;
  case ASM_JAE:
    op = ASM_JB;
    break;
  case ASM_JB:
    op = ASM_JAE;
    break;
  case ASM_JBE:
    op = ASM_JA;
    break;
  case ASM_JE:
    op = ASM_JNE;
    break;
  case ASM_JG:
    op = ASM_JLE;
    break;
  case ASM_JGE:
    op = ASM_JL;
    break;
  case ASM_JNE:
    op = ASM_JE;
    break;
  default:
    break;
  }
  return op;
}

static const Chars op_stmt_1_string[] = { //
    "?", "goto", "++", "--",

#define ONEJIT_X(NAME, name) "asm_" #name,
    ONEJIT_OPSTMT1_ASM(ONEJIT_X)
#undef ONEJIT_X

#define ONEJIT_X(NAME, name) "x86_" #name,
        ONEJIT_OPSTMT1_X86(ONEJIT_X)
#undef ONEJIT_X

};

const Chars to_string(OpStmt1 op) noexcept {
  size_t i = 0;
  if (op < ONEJIT_N_OF(op_stmt_1_string)) {
    i = op;
  }
  return op_stmt_1_string[i];
}

const Fmt &operator<<(const Fmt &out, OpStmt1 op) {
  return out << to_string(op);
}

// ============================  OpStmt2  ======================================

// if OpStmt2 is *_ASSIGN, return the corresponding Op2
Op2 to_op2(OpStmt2 op) noexcept {
  switch (op) {
  case SUB_ASSIGN:
    return SUB;
  case QUO_ASSIGN:
    return QUO;
  case REM_ASSIGN:
    return REM;
  case SHL_ASSIGN:
    return SHL;
  case SHR_ASSIGN:
    return SHR;
  default:
    return BAD2;
  }
}
// if OpStmt2 is *_ASSIGN, return the corresponding OpN
OpN to_opn(OpStmt2 op) noexcept {
  switch (op) {
  case ADD_ASSIGN:
    return ADD;
  case MUL_ASSIGN:
    return MUL;
  case AND_ASSIGN:
    return AND;
  case OR_ASSIGN:
    return OR;
  case XOR_ASSIGN:
    return XOR;
  default:
    return BADN;
  }
}

static const Chars op_stmt_2_string[] = { //
    "?",
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
    "&=",
    "|=",
    "^=",
    "<<=",
    ">>=",
    "=",
    "case",
    "default",
    "jump_if",

#define ONEJIT_X(NAME, name) "asm_" #name,
    ONEJIT_OPSTMT2_ASM(ONEJIT_X)
#undef ONEJIT_X

#define ONEJIT_X(NAME, name) "x86_" #name,
        ONEJIT_OPSTMT2_X86(ONEJIT_X)
#undef ONEJIT_X
};

const Chars to_string(OpStmt2 op) noexcept {
  size_t i = 0;
  if (op < ONEJIT_N_OF(op_stmt_2_string)) {
    i = op;
  }
  return op_stmt_2_string[i];
}

const Fmt &operator<<(const Fmt &out, OpStmt2 op) {
  return out << to_string(op);
}

// ============================  OpStmt3  ======================================

static const Chars op_stmt_3_string[] = { //
    "?", "if",

#define ONEJIT_X(NAME, name) "x86_" #name,
    ONEJIT_OPSTMT3_X86(ONEJIT_X)
#undef ONEJIT_X
};

const Chars to_string(OpStmt3 op) noexcept {
  size_t i = 0;
  if (op < ONEJIT_N_OF(op_stmt_3_string)) {
    i = op;
  }
  return op_stmt_3_string[i];
}

const Fmt &operator<<(const Fmt &out, OpStmt3 op) {
  return out << to_string(op);
}

// ============================  OpStmt4  ======================================

static const Chars op_stmt_4_string[] = {"?", "for"};

const Chars to_string(OpStmt4 op) noexcept {
  size_t i = 0;
  if (op < ONEJIT_N_OF(op_stmt_4_string)) {
    i = op;
  }
  return op_stmt_4_string[i];
}

const Fmt &operator<<(const Fmt &out, OpStmt4 op) {
  return out << to_string(op);
}

// ============================  OpStmtN  ======================================

static const Chars op_stmt_n_string[] = {"?", "=tuple", "block", "cond", "return", "switch"};

const Chars to_string(OpStmtN op) noexcept {
  size_t i = op;
  if (i >= ONEJIT_N_OF(op_stmt_n_string)) {
    i = 0;
  }
  return op_stmt_n_string[i];
}

const Fmt &operator<<(const Fmt &out, OpStmtN op) {
  return out << to_string(op);
}

} // namespace onejit
