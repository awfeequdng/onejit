/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
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

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_ASM_HPP
