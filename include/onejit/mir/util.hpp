/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * util.hpp
 *
 *  Created on Jan 05, 2022
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_MIR_UTIL_HPP
#define ONEJIT_MIR_UTIL_HPP

#include <onejit/fwd.hpp>
#include <onejit/kind.hpp>

namespace onejit {
namespace mir {

enum mKind : uint8_t { mInt32, mUint32, mInt64, mUint64, mFloat32, mFloat64, mFloat128 };

// convert Kind to mKind
mKind mir_kind(Kind kind) noexcept;

// return MIR_*MOV* instruction appropriate for kind
OpStmt2 mir_mov(Kind kind) noexcept;

// return the MIR_*NEG* instruction appropriate for kind
OpStmt2 mir_neg(Kind kind) noexcept;

// return the MIR_*2* conversion instruction appropriate for converting kind from -> to
OpStmt2 mir_cast(Kind to, Kind from) noexcept;

// convert Op2 arithmetic instruction to MIR_* instruction
OpStmt3 mir_arith(Op2 op, Kind kind) noexcept;

// convert OpN arithmetic instruction to MIR_* instruction
OpStmt3 mir_arith(OpN op, Kind kind) noexcept;

// convert OpStmt2 *_ASSIGN instruction to MIR_* instruction
OpStmt3 mir_arith(OpStmt2 op, Kind kind) noexcept;

// convert Op2 comparison instruction to MIR_* instruction
OpStmt3 mir_compare(Op2 op, Kind kind) noexcept;

// convert Op2 comparison instruction to MIR_* conditional jump
OpStmt3 mir_jump(Op2 op, Kind kind) noexcept;

// convert OpStmt3 ASM_J* conditional jump to MIR_* conditional jump
OpStmt3 mir_jump(OpStmt3 op, Kind kind) noexcept;

} // namespace mir
} // namespace onejit

#endif // ONEJIT_MIR_UTIL_HPP
