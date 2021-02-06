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
 * regid.hpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X86_REGID_HPP
#define ONEJIT_X86_REGID_HPP

#include <onejit/x64/regid.hpp>

namespace onejit {
namespace x86 {

using x64::RegId;

// clang-format off
using EAX = x64::RAX;
using ECX = x64::RCX;
using EDX = x64::RDX;
using EBX = x64::RBX;
using ESP = x64::RSP;
using EBP = x64::RBP;
using ESI = x64::RSI;
using EDI = x64::RDI;

using x64::XMM0;
using x64::XMM1;
using x64::XMM2;
using x64::XMM3;
using x64::XMM4;
using x64::XMM5;
using x64::XMM6;
using x64::XMM7;

} // namespace x86
} // namespace onejit

#endif // ONEJIT_X86_REGID_HPP
