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
 * onejit.hpp - include all onejit headers in one fell swoop
 *
 *  Created on Jan 25, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ONEJIT_HPP
#define ONEJIT_ONEJIT_HPP

#include <onejit/archid.hpp>
#include <onejit/check.hpp>
#include <onejit/code.hpp>
#include <onejit/codeparser.hpp>
#include <onejit/compiler.hpp>
#include <onejit/endian.hpp>
#include <onejit/error.hpp>
#include <onejit/eval.hpp>
#include <onejit/fmt.hpp>
#include <onejit/func.hpp>
#include <onejit/node.hpp>       // includes all onejit/node/
#include <onejit/node/const.hpp> // redundant
// #include <onejit/group.hpp>   // redundant
// #include <onejit/imm.hpp>     // redundant
// #include <onejit/kind.hpp>    // redundant
// #include <onejit/local.hpp>   // redundant
// #include <onejit/bits.hpp>    // redundant
#include <onejit/math.hpp>
// #include <onejit/op.hpp>      // redundant
// #include <onejit/opstmt.hpp>  // redundant
// #include <onejit/type.hpp>    // redundant
#include <onejit/x64.hpp> //     // includes all onejit/x64/

#include <onestl/chars.hpp>
#include <onestl/string.hpp>
#include <onestl/vector.hpp>

#endif // ONEJIT_ONEJIT_HPP
