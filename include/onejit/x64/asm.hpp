/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * asm.hpp
 *
 *  Created on Feb 01, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_ASM_HPP
#define ONEJIT_X64_ASM_HPP

#include <onejit/fwd.hpp>
#include <onejit/x64/fwd.hpp>

namespace onejit {
namespace x64 {

////////////////////////////////////////////////////////////////////////////////
class Asm0 {
  friend class onejit::Assembler;

private:
  static Assembler &emit(Assembler &dst, const Stmt0 &st) noexcept;
  static Assembler &emit(Assembler &dst, const Inst0 &inst) noexcept;
  static const Inst0 &find(OpStmt0 op) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
class Asm1 {
  friend class onejit::Assembler;

private:
  static Assembler &emit(Assembler &dst, const Stmt1 &st) noexcept;
  static Assembler &emit(Assembler &dst, const Stmt1 &st, const Inst1 &inst) noexcept;
  static const Inst1 &find(OpStmt1 op) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
class Asm2 {
  friend class onejit::Assembler;

private:
  static Assembler &emit(Assembler &dst, const Stmt2 &st) noexcept;
  static Assembler &emit(Assembler &dst, const Stmt2 &st, const Inst2 &inst) noexcept;
  static const Inst2 &find(OpStmt2 op) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
class Asm3 {
  friend class onejit::Assembler;

private:
  static Assembler &emit(Assembler &dst, const Stmt3 &st) noexcept;
  static Assembler &emit(Assembler &dst, const Stmt3 &st, const Inst3 &inst) noexcept;
  static const Inst3 &find(OpStmt3 op) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
class AsmN {
  friend class onejit::Assembler;

private:
  static Assembler &emit(Assembler &dst, const StmtN &st) noexcept;
  static Assembler &emit(Assembler &dst, const InstN &inst) noexcept;
  static const InstN &find(OpStmtN op) noexcept;
};

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_ASM_HPP
