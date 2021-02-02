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
 * fwd.hpp
 *
 *  Created on Feb 02, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_X64_FWD_HPP
#define ONEJIT_X64_FWD_HPP

#include <onejit/x86/fwd.hpp>

namespace onejit {
namespace x64 {

using x86::Addr;
using x86::Inst;
using x86::Inst0;
using x86::Inst1;
using x86::Inst2;
using x86::Inst3;
using x86::Mem;
using x86::Reg;

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_FWD_HPP
