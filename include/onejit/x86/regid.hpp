/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
