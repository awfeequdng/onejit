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
  const eKind ekind = kind.nosimd().val();
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
  } else if (ekind == eFloat128) {
    mkind = mFloat128;
  } else {
    mkind = mUint32;
  }
  return mkind;
}

// return the MIR_*MOV* instruction appropriate for kind
OpStmt2 mir_mov(Kind kind) noexcept {
  switch (kind.nosimd().val()) {
  case eFloat32:
    return MIR_FMOV;
  case eFloat64:
    return MIR_DMOV;
  case eFloat128:
    return MIR_LDMOV;
  default:
    return MIR_MOV; // both 64-bit and 32-bit integer move
  }
}

// return the MIR_*NEG* instruction appropriate for kind
OpStmt2 mir_neg(Kind kind) noexcept {
  switch (kind.nosimd().val()) {
  case eFloat32:
    return MIR_FNEG;
  case eFloat64:
    return MIR_DNEG;
  case eFloat128:
    return MIR_LDNEG;
  default:
    return kind.bits() == Bits64 ? MIR_NEG : MIR_NEGS;
  }
}

// return the MIR_*2* conversion instruction appropriate for converting kind from -> to
OpStmt2 mir_cast(Kind to, Kind from) noexcept {
  switch (from.nosimd().val()) {
  case eFloat32: // float32 -> ?
    switch (to.nosimd().val()) {
    case eFloat32:
      return MIR_FMOV;
    case eFloat64:
      return MIR_F2D;
    case eFloat128:
      return MIR_F2LD;
    default:
      return MIR_F2I;
    }
  case eFloat64: // float64 -> ?
    switch (to.nosimd().val()) {
    case eFloat32:
      return MIR_D2F;
    case eFloat64:
      return MIR_DMOV;
    case eFloat128:
      return MIR_D2LD;
    default:
      return MIR_D2I;
    }
  case eFloat128: // float128 -> ?
    switch (to.nosimd().val()) {
    case eFloat32:
      return MIR_LD2F;
    case eFloat64:
      return MIR_LD2D;
    case eFloat128:
      return MIR_LDMOV;
    default:
      return MIR_LD2I;
    }
  default: // *int* -> ?
    switch (to.nosimd().val()) {
    case eFloat32:
      return MIR_I2F;
    case eFloat64:
      return MIR_I2D;
    case eFloat128:
      return MIR_I2LD;
    default:
      break; // *int* -> *int* widen or narrow, done below
    }
  }
  const bool sign_extend = to.is_signed();
  const eBits ebits = from.bits() < to.bits() ? from.ebits() : to.ebits();
  switch (ebits) {
  case eBits16:
    return sign_extend ? MIR_EXT16 : MIR_UEXT16;
  case eBits32:
    return sign_extend ? MIR_EXT32 : MIR_UEXT32;
  default:
    if (ebits <= eBits8) {
      return sign_extend ? MIR_EXT8 : MIR_UEXT8;
    }
    return MIR_MOV;
  }
}

// convert Op2 arithmetic instruction to MIR_* OpStmt3 instruction
OpStmt3 mir_arith(Op2 op, Kind kind) noexcept {
  static const OpStmt2 st2_ops[] = {SUB_ASSIGN, QUO_ASSIGN, SHL_ASSIGN, SHR_ASSIGN};
  if (op < SUB || op > SHR) {
    return BAD_ST3;
  }
  return mir_arith(st2_ops[op - SUB], kind);
}

// convert OpN arithmetic instruction to MIR_* OpStmt3 instruction
OpStmt3 mir_arith(OpN op, Kind kind) noexcept {
  static const OpStmt2 st2_ops[] = {ADD_ASSIGN, MUL_ASSIGN, AND_ASSIGN, OR_ASSIGN, XOR_ASSIGN};
  if (op < ADD || op > XOR) {
    return BAD_ST3;
  }
  return mir_arith(st2_ops[op - ADD], kind);
}

// convert OpStmt2 *_ASSIGN instruction to MIR_* OpStmt3 instruction
OpStmt3 mir_arith(OpStmt2 op, Kind kind) noexcept {
  enum mIndex2 : size_t { mInt32, mUint32, mInt64, mUint64, mFloat32, mFloat64, mFloat128 };
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

// convert Op2 comparison instruction to MIR_* instruction
OpStmt3 mir_compare(Op2 op, Kind kind) noexcept {
  static const OpStmt3 cmp[][7] = {
      {MIR_LTS, MIR_ULTS, MIR_LT, MIR_ULT, MIR_FLT, MIR_DLT, MIR_LDLT}, // LSS
      {MIR_LES, MIR_ULES, MIR_LE, MIR_ULE, MIR_FLE, MIR_DLE, MIR_LDLE}, // LEQ
      {MIR_NES, MIR_NES, MIR_NE, MIR_NE, MIR_FNE, MIR_DNE, MIR_LDNE},   // NEQ
      {MIR_EQS, MIR_EQS, MIR_EQ, MIR_EQ, MIR_FEQ, MIR_DEQ, MIR_LDEQ},   // EQL
      {MIR_GTS, MIR_UGTS, MIR_GT, MIR_UGT, MIR_FGT, MIR_DGT, MIR_LDGT}, // GTR
      {MIR_GES, MIR_UGES, MIR_GE, MIR_UGE, MIR_FGE, MIR_DGE, MIR_LDGE}, // GEQ
  };
  if (op < LSS || op > GEQ) {
    return BAD_ST3;
  }
  return cmp[op - LSS][mir_kind(kind)];
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
  static const OpStmt3 jump_float128[] = {MIR_LDBGT, MIR_LDBGE, MIR_LDBLT, MIR_LDBLE, MIR_LDBEQ,
                                          MIR_LDBGT, MIR_LDBGE, MIR_LDBLT, MIR_LDBLE, MIR_LDBNE};
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
  case eFloat128:
    jump_table = jump_float128;
    break;
  }
  return jump_table[op - ASM_JA];
}

} // namespace mir
} // namespace onejit
