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
 *  Created on Mar 08, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_REG_FWD_HPP
#define ONEJIT_REG_FWD_HPP

#include <onestl/fwd.hpp>

namespace onejit {
namespace reg {

using Degree = ::onestl::graph::Degree;
using Reg = ::onestl::graph::Node;
using Size = ::onestl::graph::Size;
using Color = Reg;

enum : Size { NoPos = ::onestl::graph::NoPos };
enum : Reg { NoReg = NoPos };
enum : Color { NoColor = NoPos };

class Allocator;
class Liveness;

} // namespace reg
} // namespace onejit

#endif // ONEJIT_REG_FWD_HPP
