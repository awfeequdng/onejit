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
using Labels = View<Label>;
using Nodes = View<Node>;
using Vars = View<Var>;

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_FWD_HPP
