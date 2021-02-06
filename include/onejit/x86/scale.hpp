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
