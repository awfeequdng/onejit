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
 * ir.hpp - include all onejit/ir/ headers in one fell swoop
 *
 *  Created on Feb 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_HPP
#define ONEJIT_IR_HPP

// #include <onejit/ir/allow.hpp>      // redundant
#include <onejit/ir/binary.hpp>
#include <onejit/ir/call.hpp>
#include <onejit/ir/childrange.hpp>
#include <onejit/ir/comma.hpp>
#include <onejit/ir/const.hpp>
// #include <onejit/ir/expr.hpp>       // redundant
#include <onejit/ir/functype.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/mem.hpp>
// #include <onejit/ir/node.hpp>       // redundant
// #include <onejit/ir/header.hpp>     // redundant
// #include <onejit/ir/stmt.hpp>       // redundant
#include <onejit/ir/stmt0.hpp>
#include <onejit/ir/stmt1.hpp>
#include <onejit/ir/stmt2.hpp>
#include <onejit/ir/stmt3.hpp>
#include <onejit/ir/stmt4.hpp>
#include <onejit/ir/stmtn.hpp>
// #include <onejit/ir/syntax.hpp>     // redundant
// #include <onejit/ir/tuple.hpp>      // redundant
#include <onejit/ir/unary.hpp>
#include <onejit/ir/var.hpp>

#endif // ONEJIT_IR_HPP
