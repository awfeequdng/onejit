/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * syntax.hpp
 *
 *  Created on Feb 19, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_SYNTAX_HPP
#define ONEJIT_IR_SYNTAX_HPP

#include <cstdint>

namespace onejit {
namespace ir {

// Syntax passed to to_string(Node) and Node::format()
enum class Syntax : uint8_t {
  Default = 0,
  CapstoneCompat = 1, // compatible with capstone disassembly
};

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_SYNTAX_HPP
