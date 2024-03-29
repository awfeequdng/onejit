/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

const Fmt &operator<<(const Fmt &fmt, OpStmt0 op) {
  return fmt << to_string(op);
}

// ============================  OpStmt1  ======================================

static const Chars op_stmt_1_string[] = { //
    "?", "goto", "++", "--",

#define ONEJIT_X(NAME, name) "mir_" #name,
    ONEJIT_OPSTMT1_MIR(ONEJIT_X)
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

const Fmt &operator<<(const Fmt &fmt, OpStmt1 op) {
  return fmt << to_string(op);
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

/// @return the *_ASSIGN corresponding to Op2
OpStmt2 to_assign_op(Op2 op) noexcept {
  switch (op) {
  case SUB:
    return SUB_ASSIGN;
  case QUO:
    return QUO_ASSIGN;
  case REM:
    return REM_ASSIGN;
  case SHL:
    return SHL_ASSIGN;
  case SHR:
    return SHR_ASSIGN;
  default:
    return BAD_ST2;
  }
}

/// @return the *_ASSIGN corresponding to OpN
OpStmt2 to_assign_op(OpN op) noexcept {
  switch (op) {
  case ADD:
    return ADD_ASSIGN;
  case MUL:
    return MUL_ASSIGN;
  case AND:
    return AND_ASSIGN;
  case OR:
    return OR_ASSIGN;
  case XOR:
    return XOR_ASSIGN;
  default:
    return BAD_ST2;
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

#define ONEJIT_X(NAME, name) "mir_" #name,
    ONEJIT_OPSTMT2_MIR(ONEJIT_X)
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

const Fmt &operator<<(const Fmt &fmt, OpStmt2 op) {
  return fmt << to_string(op);
}

// ============================  OpStmt3  ======================================

OpStmt3 negate_condjump(OpStmt3 op) noexcept {
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

static const Chars op_stmt_3_string[] = { //
    "?", "if",

#define ONEJIT_X(NAME, name) "asm_" #name,
    ONEJIT_OPSTMT3_ASM(ONEJIT_X)
#undef ONEJIT_X

#define ONEJIT_X(NAME, name) "mir_" #name,
        ONEJIT_OPSTMT3_MIR(ONEJIT_X)
#undef ONEJIT_X

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

const Fmt &operator<<(const Fmt &fmt, OpStmt3 op) {
  return fmt << to_string(op);
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

const Fmt &operator<<(const Fmt &fmt, OpStmt4 op) {
  return fmt << to_string(op);
}

// ============================  OpStmtN  ======================================

static const Chars op_stmt_n_string[] = { //
    "?",
    "=tuple",
    "block",
    "cond",
    "return",
    "switch",
    "_set",
#define ONEJIT_X(NAME, name) "mir_" #name,
    ONEJIT_OPSTMTN_MIR(ONEJIT_X)
#undef ONEJIT_X
#define ONEJIT_X(NAME, name) "x86_" #name,
        ONEJIT_OPSTMTN_X86(ONEJIT_X)
#undef ONEJIT_X
};

const Chars to_string(OpStmtN op) noexcept {
  size_t i = op;
  if (i >= ONEJIT_N_OF(op_stmt_n_string)) {
    i = 0;
  }
  return op_stmt_n_string[i];
}

const Fmt &operator<<(const Fmt &fmt, OpStmtN op) {
  return fmt << to_string(op);
}

} // namespace onejit
