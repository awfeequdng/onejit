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
 * inst.hpp
 *
 *  Created on Jan 26, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X86_INST_X64_HPP
#define ONEJIT_X86_INST_X64_HPP

#include <onejit/fwd.hpp>
#include <onejit/x86/inst.hpp>

namespace onejit {
namespace x64 {

using x86::Inst;
using x86::Inst0;
using x86::Inst1;
using x86::Inst2;
using x86::Inst3;

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X86_INST_X64_HPP
