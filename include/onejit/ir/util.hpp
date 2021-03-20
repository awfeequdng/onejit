/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
bool is_return(Node node) noexcept;

// If node is a jump, return its destination label.
// Note: RETURN, X86_RET, ARM64_RET etc. are jumps but have no destination label
Label jump_label(Node node) noexcept;

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_UTIL_HPP
