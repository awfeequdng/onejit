/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * util.cpp
 *
 *  Created on Jan 05, 2022
 *      Author Massimiliano Ghilardi
 */

#include <onejit/mir/util.hpp>
#include <onejit/op.hpp>
#include <onejit/opstmt2.hpp>
#include <onejit/opstmt3.hpp>

namespace onejit {
namespace mir {

// convert Kind to mKind
mKind mir_kind(Kind kind) noexcept {
  const eKind ekind = kind.val();
  mKind mkind;
  if (ekind >= eInt8 && kind <= eInt32) {
    mkind = mInt32;
  } else if (ekind == eInt64) {
    mkind = mInt64;
  } else if (ekind == eUint64 || ekind == ePtr) {
    mkind = mUint64;
  } else if (ekind == eFloat32) {
    mkind = mFloat32;
  } else if (ekind == eFloat64) {
    mkind = mFloat64;
  } else {
    mkind = mUint32;
  }
  return mkind;
}

OpStmt2 mir_mov(Kind kind) noexcept {
  switch (kind.val()) {
  case eFloat32:
    return MIR_FMOV;
  case eFloat64:
    return MIR_DMOV;
  default:
    return MIR_MOV;
  }
}

// convert OpStmt3 ASM_J* conditional jump to MIR_* instruction
OpStmt3 mir_jump(OpStmt3 op, Kind kind) noexcept {
  static const OpStmt3 jump_int32[] = {MIR_UBGTS, MIR_UBGES, MIR_UBLTS, MIR_UBLES, MIR_BEQS,
                                       MIR_BGTS,  MIR_BGES,  MIR_BLTS,  MIR_BLES,  MIR_BNES};
  static const OpStmt3 jump_int64[] = {MIR_UBGT, MIR_UBGE, MIR_UBLT, MIR_UBLE, MIR_BEQ,
                                       MIR_BGT,  MIR_BGE,  MIR_BLT,  MIR_BLE,  MIR_BNE};
  static const OpStmt3 jump_float32[] = {MIR_FBGT, MIR_FBGE, MIR_FBLT, MIR_FBLE, MIR_FBEQ,
                                         MIR_FBGT, MIR_FBGE, MIR_FBLT, MIR_FBLE, MIR_FBNE};
  static const OpStmt3 jump_float64[] = {MIR_DBGT, MIR_DBGE, MIR_DBLT, MIR_DBLE, MIR_DBEQ,
                                         MIR_DBGT, MIR_DBGE, MIR_DBLT, MIR_DBLE, MIR_DBNE};
  if (op < ASM_JA || op > ASM_JNE) {
    return BAD_ST3;
  }
  const OpStmt3 *jump_table;
  switch (kind.val()) {
  default:
    jump_table = jump_int32;
    break;
  case eInt64:
  case eUint64:
  case ePtr:
    jump_table = jump_int64;
    break;
  case eFloat32:
    jump_table = jump_float32;
    break;
  case eFloat64:
    jump_table = jump_float64;
    break;
  }
  return jump_table[op - ASM_JA];
}

OpStmt3 mir_arith(OpStmt2 op, Kind kind) noexcept {
  enum mIndex2 : size_t { mInt32, mUint32, mInt64, mUint64, mFloat32, mFloat64, mFloat80 };
  static const OpStmt3 ops[][7] = {
      {MIR_ADDS, MIR_ADDS, MIR_ADD, MIR_ADD, MIR_FADD, MIR_DADD, MIR_LDADD},   // ADD_ASSIGN
      {MIR_SUBS, MIR_SUBS, MIR_SUB, MIR_SUB, MIR_FSUB, MIR_DSUB, MIR_LDSUB},   // SUB_ASSIGN
      {MIR_MULS, MIR_MULS, MIR_MUL, MIR_MUL, MIR_FMUL, MIR_DMUL, MIR_LDMUL},   // MUL_ASSIGN
      {MIR_DIVS, MIR_UDIVS, MIR_DIV, MIR_UDIV, MIR_FDIV, MIR_DDIV, MIR_LDDIV}, // QUO_ASSIGN
      {MIR_MODS, MIR_UMODS, MIR_MOD, MIR_UMOD, BAD_ST3, BAD_ST3, BAD_ST3},     // REM_ASSIGN
      {MIR_ANDS, MIR_ANDS, MIR_AND, MIR_AND, BAD_ST3, BAD_ST3, BAD_ST3},       // AND_ASSIGN
      {MIR_ORS, MIR_ORS, MIR_OR, MIR_OR, BAD_ST3, BAD_ST3, BAD_ST3},           // OR_ASSIGN
      {MIR_XORS, MIR_XORS, MIR_XOR, MIR_XOR, BAD_ST3, BAD_ST3, BAD_ST3},       // XOR_ASSIGN
      {MIR_LSHS, MIR_LSHS, MIR_LSH, MIR_LSH, BAD_ST3, BAD_ST3, BAD_ST3},       // SHL_ASSIGN
      {MIR_RSHS, MIR_URSHS, MIR_RSH, MIR_URSH, BAD_ST3, BAD_ST3, BAD_ST3},     // SHR_ASSIGN
  };
  if (op < ADD_ASSIGN || op > SHR_ASSIGN) {
    return BAD_ST3;
  }
  return ops[op - ADD_ASSIGN][mir_kind(kind)];
}

} // namespace mir
} // namespace onejit
