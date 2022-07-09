/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * fwd.hpp
 *
 *  Created on Feb 02, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_X86_FWD_HPP
#define ONEJIT_X86_FWD_HPP

#include <onejit/x64/fwd.hpp>

namespace onejit {
namespace x86 {

using x64::Inst;
using x64::Inst0;
using x64::Inst1;
using x64::Inst2;
using x64::Inst3;
using x64::Mem;
using x64::Reg;

} // namespace x86
} // namespace onejit

#endif // ONEJIT_X86_FWD_HPP
