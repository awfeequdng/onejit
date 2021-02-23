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
 * node.hpp - include all onejit/node/ headers in one fell swoop
 *
 *  Created on Feb 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_HPP
#define ONEJIT_NODE_HPP

// #include <onejit/node/allow.hpp>      // redundant
#include <onejit/node/binary.hpp>
#include <onejit/node/call.hpp>
#include <onejit/node/childrange.hpp>
#include <onejit/node/comma.hpp>
#include <onejit/node/const.hpp>
// #include <onejit/node/expr.hpp>       // redundant
#include <onejit/node/functype.hpp>
#include <onejit/node/label.hpp>
#include <onejit/node/mem.hpp>
// #include <onejit/node/node.hpp>       // redundant
// #include <onejit/node/nodeheader.hpp> // redundant
// #include <onejit/node/stmt.hpp>       // redundant
#include <onejit/node/stmt0.hpp>
#include <onejit/node/stmt1.hpp>
#include <onejit/node/stmt2.hpp>
#include <onejit/node/stmt3.hpp>
#include <onejit/node/stmt4.hpp>
#include <onejit/node/stmtn.hpp>
// #include <onejit/node/syntax.hpp>     // redundant
// #include <onejit/node/tuple.hpp>      // redundant
#include <onejit/node/unary.hpp>
#include <onejit/node/var.hpp>

#endif // ONEJIT_NODE_HPP
