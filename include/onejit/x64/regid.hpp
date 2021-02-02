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
#ifndef ONEJIT_X64_REGID_HPP
#define ONEJIT_X64_REGID_HPP

#include <onejit/x86/regid.hpp>

namespace onejit {
namespace x64 {

using x86::RegId;

// clang-format off
using x86::RAX;
using x86::RCX;
using x86::RDX;
using x86::RBX;
using x86::RSP;
using x86::RBP;
using x86::RSI;
using x86::RDI;
using x86::R8;
using x86::R9;
using x86::R10;
using x86::R11;
using x86::R12;
using x86::R13;
using x86::R14;
using x86::R15;

using x86::RIP;

using x86::XMM0;
using x86::XMM1;
using x86::XMM2;
using x86::XMM3;
using x86::XMM4;
using x86::XMM5;
using x86::XMM6;
using x86::XMM7;
using x86::XMM8;
using x86::XMM9;
using x86::XMM10;
using x86::XMM11;
using x86::XMM12;
using x86::XMM13;
using x86::XMM14;
using x86::XMM15;

using x86::ZMM16;
using x86::ZMM17;
using x86::ZMM18;
using x86::ZMM19;
using x86::ZMM20;
using x86::ZMM21;
using x86::ZMM22;
using x86::ZMM23;
using x86::ZMM24;
using x86::ZMM25;
using x86::ZMM26;
using x86::ZMM27;
using x86::ZMM28;
using x86::ZMM29;
using x86::ZMM30;
using x86::ZMM31;

using x86::rlo;
using x86::rhi;

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_REGID_HPP
