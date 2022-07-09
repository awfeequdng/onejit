/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * onejit.hpp - include all onejit headers in one fell swoop
 *
 *  Created on Jan 25, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ONEJIT_HPP
#define ONEJIT_ONEJIT_HPP

#include <onejit/archid.hpp>
#include <onejit/code.hpp>
#include <onejit/codeparser.hpp>
#include <onejit/compiler.hpp>
#include <onejit/endian.hpp>
#include <onejit/error.hpp>
#include <onejit/eval.hpp>
#include <onejit/fmt.hpp>
#include <onejit/func.hpp>
#include <onejit/mem.hpp>
#include <onejit/ir.hpp>       // includes all onejit/ir/
#include <onejit/ir/const.hpp> // redundant
#include <onejit/test.hpp>
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

#include <onestl/array.hpp>
#include <onestl/chars.hpp>
#include <onestl/string.hpp>

#endif // ONEJIT_ONEJIT_HPP
