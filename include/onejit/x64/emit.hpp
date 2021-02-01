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
 * emit.hpp
 *
 *  Created on Feb 01, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_EMIT_HPP
#define ONEJIT_X64_EMIT_HPP

#include <onejit/assembler.hpp>

namespace onejit {
namespace x64 {

Assembler &emit(Assembler &dst, Node node) noexcept;
Assembler &emit(Assembler &dst, Stmt0 st) noexcept;
Assembler &emit(Assembler &dst, Stmt1 st) noexcept;
Assembler &emit(Assembler &dst, Stmt2 st) noexcept;
Assembler &emit(Assembler &dst, Stmt3 st) noexcept;

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_EMIT_HPP
