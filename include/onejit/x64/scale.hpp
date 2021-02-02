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
 * scale.hpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_SCALE_HPP
#define ONEJIT_X64_SCALE_HPP

#include <onejit/x86/scale.hpp>

namespace onejit {
namespace x64 {

using x86::eScale;
using x86::Scale;

using x86::Scale0;
using x86::Scale1;
using x86::Scale2;
using x86::Scale4;
using x86::Scale8;

using x86::eScale0;
using x86::eScale1;
using x86::eScale2;
using x86::eScale4;
using x86::eScale8;

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_SCALE_HPP
