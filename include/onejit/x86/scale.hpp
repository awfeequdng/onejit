/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * scale.hpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X86_SCALE_HPP
#define ONEJIT_X86_SCALE_HPP

#include <onejit/x64/scale.hpp>

namespace onejit {
namespace x86 {

using x64::eScale;
using x64::Scale;

using x64::Scale0;
using x64::Scale1;
using x64::Scale2;
using x64::Scale4;

using x64::eScale0;
using x64::eScale1;
using x64::eScale2;
using x64::eScale4;

} // namespace x86
} // namespace onejit

#endif // ONEJIT_X86_SCALE_HPP
