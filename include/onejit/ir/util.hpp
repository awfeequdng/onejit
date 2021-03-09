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
 * node.hpp
 *
 *  Created on Mar 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_UTIL_HPP
#define ONEJIT_IR_UTIL_HPP

#include <onejit/ir/fwd.hpp>

namespace onejit {
namespace ir {

bool is_jump(Node node) noexcept;
bool is_cond_jump(Node node) noexcept;
bool is_uncond_jump(Node node) noexcept;

// If node is a jump, return its destination label.
// Note: RETURN, X86_RET, ARM64_RET etc. are jumps but have no destination label
Label jump_label(Node node) noexcept;

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_UTIL_HPP
