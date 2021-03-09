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
 * fwd.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_FWD_HPP
#define ONEJIT_IR_FWD_HPP

#include <onestl/fwd.hpp>

#include <cstdint> // uint*_t

#define ONEJIT_NOINLINE ONESTL_NOINLINE
#define ONEJIT_NORETURN ONESTL_NORETURN

#define ONEJIT_N_OF(array) (sizeof(array) / sizeof(array[0]))

namespace onejit {
namespace ir {

template <class T> using View = ::onestl::View<T>;

enum Allow : uint8_t;
class Assign;
class AssignCall;
class Block;
class Binary;
class Break;
class Call;
class Case;
class ChildRange;
class Comma;
class Cond;
class Const;
class Continue;
class Default;
class Expr;
class Fallthrough;
class For;
class FuncType;
class Goto;
class If;
class JumpIf;
class Label;
class Mem;
class Name;
class Node;
class Header;
class Return;
class Stmt0;
class Stmt1;
class Stmt2;
class Stmt3;
class Stmt4;
class StmtN;
class Switch;
enum class Syntax : uint8_t;
class Tuple;
class Unary;
class Var;
class VarHelper;

using ChildRanges = View<ChildRange>;
using Cases = View<Case>;
using Exprs = View<Expr>;
using Nodes = View<Node>;
using Vars = View<Var>;

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_FWD_HPP
