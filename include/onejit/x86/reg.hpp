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
 * reg.hpp
 *
 *  Created on Feb 02, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X86_REG_HPP
#define ONEJIT_X86_REG_HPP

#include <onejit/x64/reg.hpp>
#include <onejit/x64/regid.hpp>

namespace onejit {
namespace x86 {

using x64::Reg;

} // namespace x86
} // namespace onejit

#endif // ONEJIT_X86_REG_HPP
